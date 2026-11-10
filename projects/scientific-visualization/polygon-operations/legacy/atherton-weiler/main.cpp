#include <cstring>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <GL/glut.h>
#include "polygone.h"

using namespace std;

int OHeight = 0;

void redim(int width, int height);
void affichage();
void clavier(unsigned char key, int x, int y);
void souris(int button, int state, int x, int y);
void CreerMenu();
void ChoixMenuPrincipal(int value);
bool TestInterieur(int x, int y);
void deplacement(int x, int y);
void afficheTabInter(vector<INTERSECT> _vec);
void CalculTabInter();
void Difference();
void Union();
void Intersection();

Polygone polygone;
vector<Polygone> vecPoly;
int mode = 1;  //1=saisie points, 2=saisie trous, 3=déplacement
int indicePoint;
int indiceTrou;
int indicePoly;

bool clique = false;

int indiceDep;     // indice du polygone à déplacer
Polygone DepPoly; //polygone contenant la différence entre les pts du polygone et la souris lors du déplacement
POINT pInter(-1, -1);	  // Point d'intersection entre 2 segments;
vector<INTERSECT> vecInter; // tableau contenat les points d'intersection entre 2 polygones

Polygone p1, p2, p3;

int main(int argc, char ** argv)
{
	vecPoly.push_back(polygone);
	indicePoint = 0;
	indiceTrou = 0;
	indicePoly = 0;
	indiceDep = -1;
	glutInit(&argc, argv);
	glutInitWindowSize(800,800);
	glutInitWindowPosition(200,100);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	
	glutCreateWindow("Tp 2 Atherton et Weiler");
	glutDisplayFunc(affichage);
	glutReshapeFunc(redim);

	glutKeyboardFunc(clavier);
	glutMouseFunc(souris);
	glutMotionFunc(deplacement);

	CreerMenu();

	glutMainLoop();
	return 0;
}
void affichage()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 1);

	for(int i=0; i<= indicePoly; i++)
	{
		/************* Affichage contours ************/
		glBegin(GL_LINE_STRIP);
		glColor3f(0.0, 0.0, 0.0);
		for(int j=0; j<vecPoly[i].vecPoints.size(); j++)
			glVertex2f(vecPoly[i].vecPoints[j].x, vecPoly[i].vecPoints[j].y);
		if(vecPoly[i].vecPoints.size() != 0)
			glVertex2f(vecPoly[i].vecPoints[0].x, vecPoly[i].vecPoints[0].y);
		glEnd();
	
		glPointSize(10.0);
		glBegin(GL_POINTS);
		glColor3f(1.0, 0.0, 0.0);
		for(int j=0; j<vecPoly[i].vecPoints.size(); j++)
			glVertex2f(vecPoly[i].vecPoints[j].x, vecPoly[i].vecPoints[j].y);
		glEnd();

		glPointSize(3.0);
		glBegin(GL_POINTS);
		glColor3f(0.0, 0.0, 1.0);
		for(int j=0; j<p3.vecPoints.size(); j++)
			glVertex2f(p3.vecPoints[j].x, p3.vecPoints[j].y);
		glEnd();
		glPointSize(2.0);
		glBegin(GL_LINE_STRIP);
		glColor3f(1.0, 0.0, 0.0);
		for(int j=0; j<p3.vecPoints.size(); j++)
			glVertex2f(p3.vecPoints[j].x, p3.vecPoints[j].y);
		if(p3.vecPoints.size() != 0)
			glVertex2f(p3.vecPoints[0].x, p3.vecPoints[0].y);
		glEnd();

		/************* Affichage trous ************/
		glBegin(GL_LINE_STRIP);
		glColor3f(0.0, 1.0, 0.0);
		for(int j=0; j<vecPoly[i].vecTrou.size(); j++)
			glVertex2f(vecPoly[i].vecTrou[j].x, vecPoly[i].vecTrou[j].y);
		if(vecPoly[i].vecTrou.size() != 0)
			glVertex2f(vecPoly[i].vecTrou[0].x, vecPoly[i].vecTrou[0].y);
		glEnd();

		glPointSize(3.0);
		glBegin(GL_POINTS);
		glColor3f(0.0, 0.0, 1.0);
		for(int j=0; j<vecPoly[i].vecTrou.size(); j++)
			glVertex2f(vecPoly[i].vecTrou[j].x, vecPoly[i].vecTrou[j].y);
		glEnd();
	}
	/*for (unsigned int i = 0; i < vecPoly[i].vecPoints.size()-1 && vecPoly[i].vecPoints.size()> 1 ; i++)
    	{
        if ( vecPoly[i].vecPoints.size() > 1 && vecPoly[i].vecPoints.size() != i)
		    bresenham(vecPoly[i].vecPoints[i].x, vecPoly[i].vecPoints[i].y, vecPoly[i].vecPoints[i+1].x, vecPoly[i].vecPoints[i+1].y);
 	}*/
	afficheTabInter(vecInter);
	glutSwapBuffers();
	glutPostRedisplay();
}
void redim (int width, int height)
{
	OHeight = height;
	glViewport(0,0,width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void clavier (unsigned char key, int x, int y)
{
	switch(key)
	{
		/*case 'z' : break;
		case 't' : break;		
		case 'e' : break;	
		case 'c' : break;
		case 'p' : break;*/
		case 27 : exit(0); break;
	}
	glutPostRedisplay();
}
void souris (int button, int state, int x, int y)
{
	int indiceSuppr = -1;

	/********* Mode Saisie Points**************/
	if(mode == 1)
	{
		if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			for(int i=0; i<vecPoly[indicePoly].vecPoints.size(); i++)
				if((x > vecPoly[indicePoly].vecPoints[i].x-5) && (x < vecPoly[indicePoly].vecPoints[i].x+5) && (OHeight - y > vecPoly[indicePoly].vecPoints[i].y-5) && (OHeight - y < vecPoly[indicePoly].vecPoints[i].y+5))
					indiceSuppr = i;

			if(indiceSuppr != -1)
			{
				vecPoly[indicePoly].supprimerPoints(indiceSuppr);
				indicePoint --;
			}
			else
			{
				vecPoly[indicePoly].ajouterPoints(indicePoint, POINT(x, OHeight - y));
				indicePoint++;
			}
		}
		
		if(button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) 
			for(int i = 0; i<vecPoly[indicePoly].vecPoints.size();i++)
				if((x > vecPoly[indicePoly].vecPoints[i].x-5) && (x < vecPoly[indicePoly].vecPoints[i].x+5) && (OHeight - y > vecPoly[indicePoly].vecPoints[i].y-5) && (OHeight - y < vecPoly[indicePoly].vecPoints[i].y+5))
					indicePoint = i+1;
	}

	/********* Mode Saisie Trous**************/
	if(mode == 2)
	{
		if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			for(int i=0; i<vecPoly[indicePoly].vecTrou.size(); i++)
				if((x > vecPoly[indicePoly].vecTrou[i].x-5) && (x < vecPoly[indicePoly].vecTrou[i].x+5) && (OHeight - y > vecPoly[indicePoly].vecTrou[i].y-5) && (OHeight - y < vecPoly[indicePoly].vecTrou[i].y+5))
					indiceSuppr = i;

			if(indiceSuppr != -1)
			{
				vecPoly[indicePoly].supprimerPointsTrou(indiceSuppr);
				indiceTrou --;
			}
			else
			{
				vecPoly[indicePoly].ajouterPointsTrou(indiceTrou, POINT(x, OHeight - y));
				indiceTrou++;
			}
		}
		
		if(button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) 
			for(int i = 0; i<vecPoly[indicePoly].vecTrou.size();i++)
				if((x > vecPoly[indicePoly].vecTrou[i].x-5) && (x < vecPoly[indicePoly].vecTrou[i].x+5) && (OHeight - y > vecPoly[indicePoly].vecTrou[i].y-5) && (OHeight - y < vecPoly[indicePoly].vecTrou[i].y+5))
					indiceTrou = i+1;
	}

	/********* Mode Deplacement **************/
	if(mode == 3)
	{
		if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			if(!TestInterieur(x, y))
				std::cout<<"exterieur"<<std::endl;
			else
			{
				for(int i=0; i<vecPoly[indiceDep].vecPoints.size(); i++)
					DepPoly.ajouterPoints(i, POINT((int)(x-vecPoly[indiceDep].vecPoints[i].x), (int)(OHeight-y-vecPoly[indiceDep].vecPoints[i].y))); 
				for(int i=0; i<vecPoly[indiceDep].vecTrou.size(); i++)
					DepPoly.ajouterPointsTrou(i, POINT((int)(x-vecPoly[indiceDep].vecTrou[i].x), (int)(OHeight-y-vecPoly[indiceDep].vecTrou[i].y)));
				clique = true;
			}
			
		}
		if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
			clique = false;
	}


	indiceSuppr = -1;
	glutPostRedisplay();
}
void CreerMenu()
{
	glutCreateMenu(ChoixMenuPrincipal);
	glutAddMenuEntry("Saisie des contours", 1);
	glutAddMenuEntry("Saisie du trou", 2);
	glutAddMenuEntry("Deplacement", 3);
	glutAddMenuEntry("Nouveau Polygone", 4);
	glutAddMenuEntry("Points d'Intersection", 5);
	glutAddMenuEntry("Difference", 6);
	glutAddMenuEntry("Union", 7);
	glutAddMenuEntry("Intersection", 8);
	glutAddMenuEntry("Effacer", 26);
	glutAddMenuEntry("Quitter", 27);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
void ChoixMenuPrincipal(int value)
{
	switch (value)
	{
		case 1: mode = 1; 
		break;
		
		case 2: mode = 2; 
		break;

		case 3: mode = 3; 
		break;
		case 4:
			indicePoly ++;
			indicePoint = 0;
			indiceTrou = 0;
			vecPoly.push_back(polygone);
			mode = 1;
		break;
		case 5:
			vecInter.clear();
			CalculTabInter();
			afficheTabInter(vecInter);
		break;
		case 6:
			vecInter.clear();
			Difference();
			afficheTabInter(vecInter);
		break;
		case 7:
			vecInter.clear();
			Union();
			afficheTabInter(vecInter);
		break;
		case 8:
			vecInter.clear();
			Intersection();
			afficheTabInter(vecInter);
		break;
		case 26: vecPoly.clear();
			vecPoly.push_back(polygone);
			vecInter.clear();
			indicePoint = 0;
			indiceTrou = 0;
			indicePoly = 0;
			mode = 1;

		break;
		case 27: exit(0);
		break;
	}
	glutPostRedisplay();
}
bool TestInterieur(int x, int y)
{
	int nbInter = 0;
	double Xi;
	
	y = OHeight - y;

	// Pour chaque polygone
	for(int i=0; i<vecPoly.size()/*indicePoly*/; i++)
	{
		// On test les intersections avec le contours
		for(int j=0; j<vecPoly[i].vecPoints.size(); j++)
		{
			// Cas simple
			if((vecPoly[i].vecPoints[j].x>x && vecPoly[i].vecPoints[j].y>y && vecPoly[i].vecPoints[(j+1)%vecPoly[i].vecPoints.size()].x>x && vecPoly[i].vecPoints[(j+1)%vecPoly[i].vecPoints.size()].y<y) ||
			(vecPoly[i].vecPoints[j].x>x && vecPoly[i].vecPoints[j].y<y && vecPoly[i].vecPoints[(j+1)%vecPoly[i].vecPoints.size()].x>x && vecPoly[i].vecPoints[(j+1)%vecPoly[i].vecPoints.size()].y>y))
				nbInter++;

			// Cas "diagonal"
			if((vecPoly[i].vecPoints[j].x>x && vecPoly[i].vecPoints[j].y>y && vecPoly[i].vecPoints[(j+1)%vecPoly[i].vecPoints.size()].x<x && vecPoly[i].vecPoints[(j+1)%vecPoly[i].vecPoints.size()].y<y) || 
			(vecPoly[i].vecPoints[j].x>x && vecPoly[i].vecPoints[j].y<y && vecPoly[i].vecPoints[(j+1)%vecPoly[i].vecPoints.size()].x<x && vecPoly[i].vecPoints[(j+1)%vecPoly[i].vecPoints.size()].y>y) ||
			(vecPoly[i].vecPoints[j].x<x && vecPoly[i].vecPoints[j].y<y && vecPoly[i].vecPoints[(j+1)%vecPoly[i].vecPoints.size()].x>x && vecPoly[i].vecPoints[(j+1)%vecPoly[i].vecPoints.size()].y>y) ||
			(vecPoly[i].vecPoints[j].x<x && vecPoly[i].vecPoints[j].y>y && vecPoly[i].vecPoints[(j+1)%vecPoly[i].vecPoints.size()].x>x && vecPoly[i].vecPoints[(j+1)%vecPoly[i].vecPoints.size()].y<y))
			{
				Xi = (y - vecPoly[i].vecPoints[j].y)*(vecPoly[i].vecPoints[j].x - vecPoly[i].vecPoints[(j+1)%vecPoly[i].vecPoints.size()].x) / (vecPoly[i].vecPoints[j].y - vecPoly[i].vecPoints[(j+1)%vecPoly[i].vecPoints.size()].y) + vecPoly[i].vecPoints[j].x;
				if(Xi >= x)
					nbInter++;
			}
		}

		Xi = 0;

		// On test les intersections avec le contours du trou
		for(int j=0; j<vecPoly[i].vecTrou.size(); j++)
		{
			// Cas simple
			if((vecPoly[i].vecTrou[j].x>x && vecPoly[i].vecTrou[j].y>y && vecPoly[i].vecTrou[(j+1)%vecPoly[i].vecTrou.size()].x>x && vecPoly[i].vecTrou[(j+1)%vecPoly[i].vecTrou.size()].y<y) ||
			(vecPoly[i].vecTrou[j].x>x && vecPoly[i].vecTrou[j].y<y && vecPoly[i].vecTrou[(j+1)%vecPoly[i].vecTrou.size()].x>x && vecPoly[i].vecTrou[(j+1)%vecPoly[i].vecTrou.size()].y>y))
				nbInter++;

			// Cas "diagonal"
			if((vecPoly[i].vecTrou[j].x>x && vecPoly[i].vecTrou[j].y>y && vecPoly[i].vecTrou[(j+1)%vecPoly[i].vecTrou.size()].x<x && vecPoly[i].vecTrou[(j+1)%vecPoly[i].vecTrou.size()].y<y) || 
			(vecPoly[i].vecTrou[j].x>x && vecPoly[i].vecTrou[j].y<y && vecPoly[i].vecTrou[(j+1)%vecPoly[i].vecTrou.size()].x<x && vecPoly[i].vecTrou[(j+1)%vecPoly[i].vecTrou.size()].y>y) ||
			(vecPoly[i].vecTrou[j].x<x && vecPoly[i].vecTrou[j].y<y && vecPoly[i].vecTrou[(j+1)%vecPoly[i].vecTrou.size()].x>x && vecPoly[i].vecTrou[(j+1)%vecPoly[i].vecTrou.size()].y>y) ||
			(vecPoly[i].vecTrou[j].x<x && vecPoly[i].vecTrou[j].y>y && vecPoly[i].vecTrou[(j+1)%vecPoly[i].vecTrou.size()].x>x && vecPoly[i].vecTrou[(j+1)%vecPoly[i].vecTrou.size()].y<y))
			{
				Xi = (y - vecPoly[i].vecTrou[j].y)*(vecPoly[i].vecTrou[j].x - vecPoly[i].vecTrou[(j+1)%vecPoly[i].vecTrou.size()].x) / (vecPoly[i].vecTrou[j].y - vecPoly[i].vecTrou[(j+1)%vecPoly[i].vecTrou.size()].y) + vecPoly[i].vecTrou[j].x;
				if(Xi >= x)
					nbInter++;
			}
		}
		if(nbInter%2)
		{
			indiceDep = i;
			return true;
		}
		nbInter = 0;
	}

	indiceDep = -1;
	// Si nb paire d'intersection = exterieur
	return false;
}
void deplacement(int x, int y)
{
	if(clique)
	{	for(int i=0; i<vecPoly[indiceDep].vecPoints.size(); i++)
		{
			vecPoly[indiceDep].vecPoints[i].x = x-DepPoly.vecPoints[i].x;
			vecPoly[indiceDep].vecPoints[i].y = OHeight-y-DepPoly.vecPoints[i].y;
		}
		for(int i=0; i<vecPoly[indiceDep].vecTrou.size(); i++)
		{
			vecPoly[indiceDep].vecTrou[i].x = x-DepPoly.vecTrou[i].x;
			vecPoly[indiceDep].vecTrou[i].y = OHeight-y-DepPoly.vecTrou[i].y;
		}
	}
}
// Test si la ligne du polygone 1 s'intercte avec la ligne du polygone 2
bool testIntersect(POINT ptA, POINT ptB, POINT ptC, POINT ptD)
{
	float yac, yad, ybd, ybc, xca, yca, xcb, ycb, xda, yda, xdb, ydb , xac, xad, xbc, xbd;
	float  prodca_cb, prodda_db, prod1, prod2 , prodac_ad, prodbc_bd;

	xca = ptA.x-ptC.x;
	yca = ptA.y-ptC.y;
	xcb = ptB.x-ptC.x;
	ycb = ptB.y-ptC.y;

	yac = ptC.y - ptA.y;
	yad = ptD.y - ptA.y;
	ybc = ptC.y - ptB.y;
	ybd = ptD.y - ptB.y;

	xac = ptC.x - ptA.x;
	xad = ptD.x - ptA.x;
	xbc = ptC.x - ptB.x;
	xbd = ptD.x - ptB.x;

	xda = ptA.x-ptD.x;
	yda = ptA.y-ptD.y;
	xdb = ptB.x-ptD.x;
	ydb = ptB.y-ptD.y;
	
	prodca_cb = (xca * ycb) - (xcb * yca);
	prodda_db = (xda * ydb) - (xdb * yda);

	prodac_ad = ( xac * yad ) - ( xad * yac );
	prodbc_bd = ( xbc * ybd ) - ( xbd * ybc );
	
	if( prodac_ad * prodbc_bd <0)
	{
		if( prodca_cb * prodda_db <0)
		{
			//cout << "Intersection !!" << endl;
			return true;
		}
		else
		{
			//cout << "pas intersection" << endl;
			return false;
		}
	}
	else
	{
		//cout << "pas intersection" << endl;
		return false;
	}
}
//Calcule du point d'intersection entre les lignes des polygone 1 et 2
void calculIntersect(POINT ptA, POINT ptB, POINT ptC, POINT ptD)
{
	float  a1, a2, b1, b2;

	a1 = (ptA.y-ptB.y)/(ptA.x-ptB.x);
	a2 = (ptC.y-ptD.y)/(ptC.x-ptD.x);
	b1 = ptA.y - (ptA.x * a1);
	b2 = ptC.y - (ptC.x * a2);

	pInter.x = (b2-b1)/(a1-a2);
	pInter.y = a1 * pInter.x + b1;
	//cout << pInter.x << "\t" << pInter.y << "\t" << endl;
	
}
void afficheTabInter(vector<INTERSECT> _vec)
{	
	glPointSize(10.0);
	for(int i=0; i<_vec.size(); i++)
	{
		glColor3f(0.0, 0.0, 1.0);
	  	glPointSize(5.0);
		glBegin(GL_POINTS);
			glVertex2f(_vec[i].x, _vec[i].y);
		glEnd();
	}			
}
void CalculTabInter()
{
	if (indicePoly<1)
	  return;
	//Si intersection entre 2 poly
	//alors créé un point I
	int indice1 = 1, indice2 = 1, cpt1 = 0, cpt2 = 0, cpt3 = 0; 
	bool changePoly = false;
	bool boolMarq = false;
	int max1, max2;
	int maxi = vecPoly[0].vecPoints.size()-1;
	int maxj = vecPoly[1].vecPoints.size()-1;
	int marque1[maxi];
	int marque2[maxj];
	int iM1=0,iM1Max;//compteur marque1
	int iM2=0,iM2Max;//compteur marque2

	POINT depart(-1, -1);
	POINT courant(-1, -1);

	

	/* Initialisation des tableaux de marqueurs*/
	for(int i=0; i<=maxi; i++)
		marque1[i] = 0;
	for(int j=0; j<=maxj; j++)
		marque2[j] = 0;

	/* Remplissage de p1 et p2 */
	/* Copie des deux premiers polygones*/
	for(int i=0; i<=maxi; i++)
		p1.ajouterPoints(i, (vecPoly[0].vecPoints[i]));
	for(int j=0; j<=maxj; j++)
		p2.ajouterPoints(j, (vecPoly[1].vecPoints[j]));
	/* Ajout des intersections au Premier Polygone*/
	for(int i=0; i<=maxi; i++)
	{
		for(int j=0; j<=maxj; j++)
		{
			if(testIntersect(vecPoly[0].vecPoints[i%(maxi+1)], vecPoly[0].vecPoints[(i+1)%(maxi+1)], vecPoly[1].vecPoints[j%(maxj+1)], vecPoly[1].vecPoints[(j+1)%(maxj+1)]))
			{
				calculIntersect(vecPoly[0].vecPoints[i%(maxi+1)], vecPoly[0].vecPoints[(i+1)%(maxi+1)], vecPoly[1].vecPoints[j%(maxj+1)], vecPoly[1].vecPoints[(j+1)%(maxj+1)]);
				vecInter.push_back(INTERSECT(i+1, j+1, pInter.x, pInter.y));
			}
		}
	}
	for(int i=vecInter.size()-1; i>=0; i--)
	{
		POINT p(vecInter[i].x, vecInter[i].y);
		p1.ajouterPoints(vecInter[i].indice1, p);
	}
	/* Ajout des intersections au Deuxième Polygone */
	vecInter.clear();
	for(int j=0; j<=maxj; j++)
		for(int i=0; i<=maxi; i++)
			if(testIntersect(vecPoly[0].vecPoints[i%(maxi+1)], vecPoly[0].vecPoints[(i+1)%(maxi+1)], vecPoly[1].vecPoints[j%(maxj+1)], vecPoly[1].vecPoints[(j+1)%(maxj+1)]))
	{
		calculIntersect(vecPoly[0].vecPoints[i%(maxi+1)], vecPoly[0].vecPoints[(i+1)%(maxi+1)], vecPoly[1].vecPoints[j%(maxj+1)], vecPoly[1].vecPoints[(j+1)%(maxj+1)]);
		vecInter.push_back(INTERSECT(i+1, j+1, pInter.x, pInter.y));
	}
	for(int i=vecInter.size()-1; i>=0; i--)
	{
		POINT p(vecInter[i].x, vecInter[i].y);
		p2.ajouterPoints(vecInter[i].indice2, p);
	}
}
void Difference()
{
	if (indicePoly<1)
	  return;
  
	int indice1 = 1, indice2 = 1, cpt1 = 0, cpt2 = 0, cpt3 = 0; 
	bool changePoly = false;
	int max1, max2;
	int maxi = vecPoly[0].vecPoints.size()-1;
	int maxj = vecPoly[1].vecPoints.size()-1;
	int marque1[maxi];
	int marque2[maxj];

	POINT depart(-1, -1);
	POINT courant(-1, -1);

	

	/* Initialisation des tableaux de marqueurs*/
	for(int i=0; i<=maxi; i++)
		marque1[i] = 0;
	for(int j=0; j<=maxj; j++)
		marque2[j] = 0;

	/* Remplissage de p1 et p2 */
	/* Copie des deux premiers polygones*/
	for(int i=0; i<=maxi; i++)
		p1.ajouterPoints(i, (vecPoly[0].vecPoints[i]));
	for(int j=0; j<=maxj; j++)
		p2.ajouterPoints(j, (vecPoly[1].vecPoints[j]));
	/* Ajout des intersections */
	for(int i=0; i<=maxi; i++)
	{
		for(int j=0; j<=maxj; j++)
		{
			if(testIntersect(vecPoly[0].vecPoints[i%(maxi+1)], vecPoly[0].vecPoints[(i+1)%(maxi+1)], vecPoly[1].vecPoints[j%(maxj+1)], vecPoly[1].vecPoints[(j+1)%(maxj+1)]))
			{
				calculIntersect(vecPoly[0].vecPoints[i%(maxi+1)], vecPoly[0].vecPoints[(i+1)%(maxi+1)], vecPoly[1].vecPoints[j%(maxj+1)], vecPoly[1].vecPoints[(j+1)%(maxj+1)]);
				vecInter.push_back(INTERSECT(i+1, j+1, pInter.x, pInter.y));
			}
		}
	}
	for(int i=vecInter.size()-1; i>=0; i--)
	{
		POINT p(vecInter[i].x, vecInter[i].y);
		p1.ajouterPoints(vecInter[i].indice1, p);
	}

	vecInter.clear();
	for(int j=0; j<=maxj; j++)
		for(int i=0; i<=maxi; i++)
			if(testIntersect(vecPoly[0].vecPoints[i%(maxi+1)], vecPoly[0].vecPoints[(i+1)%(maxi+1)], vecPoly[1].vecPoints[j%(maxj+1)], vecPoly[1].vecPoints[(j+1)%(maxj+1)]))
			{
				calculIntersect(vecPoly[0].vecPoints[i%(maxi+1)], vecPoly[0].vecPoints[(i+1)%(maxi+1)], vecPoly[1].vecPoints[j%(maxj+1)], vecPoly[1].vecPoints[(j+1)%(maxj+1)]);
				vecInter.push_back(INTERSECT(i+1, j+1, pInter.x, pInter.y));
			}
	for(int i=vecInter.size()-1; i>=0; i--)
	{
		POINT p(vecInter[i].x, vecInter[i].y);
		p2.ajouterPoints(vecInter[i].indice2, p);
	}

		
	max1 = p1.vecPoints.size()-1;
	max2 = p2.vecPoints.size()-1;



	/* On prend comme point de départ le premier point du polygone 1 qui n'est pas à l'intérieur du polygone 2 */
	depart.x = p1.vecPoints[0].x;
	depart.y = p1.vecPoints[0].y;

	p3.ajouterPoints(0, depart);
	cpt3++;

		
	
	while((depart.x != courant.x) && (depart.y != courant.y))
	{

		if(!changePoly)
		{
			// pour chaque sommet du poly 1
			for(int i=(cpt1+1)%(max1+1); i<=max1+1 && !changePoly; i++)
			{
				courant.x = p1.vecPoints[i%(max1+1)].x;
				courant.y = p1.vecPoints[i%(max1+1)].y;
				if((depart.x == courant.x) && (depart.y == courant.y))
					break;
					
				// Si ce point est une intersection
				for(int k=0; k<=max2; k++)
					if((courant.x == p2.vecPoints[k].x) && (courant.y == p2.vecPoints[k].y))
					{
						p3.ajouterPoints(cpt3, courant);
						cpt3++;
						
						changePoly = true;
						cpt2 = k;
					}
				
				// Sinon on recopie dans le poly 3
				if(!changePoly)
				{
					p3.ajouterPoints(cpt3, courant);
					cpt3++;
				}
			}

		}
		if(changePoly)
		{
			for(int j=(cpt2+1)%(max2+1); j<=max2+1 && changePoly; j++)
			{
				courant.x = p2.vecPoints[j%(max2+1)].x;
				courant.y = p2.vecPoints[j%(max2+1)].y;
				if((depart.x == courant.x) && (depart.y == courant.y))
					break;
	
				for(int k=0; k<=max1; k++)
					if((courant.x == p1.vecPoints[k].x) && (courant.y == p1.vecPoints[k].y))
					{
						p3.ajouterPoints(cpt3, courant);
						cpt3++;
						
						changePoly = false;
						cpt1 = k;
					}
					
				if(changePoly)
				{
					p3.ajouterPoints(cpt3, courant);
					cpt3++;
				}
			}
		}
	}
}
void Union()
{
	if (indicePoly<1)
		return;
  
	int indice1 = 1, indice2 = 1, cpt1 = 0, cpt2 = 0, cpt3 = 0; 
	bool changePoly = false;
	int max1, max2;
	int maxi = vecPoly[0].vecPoints.size()-1;
	int maxj = vecPoly[1].vecPoints.size()-1;
	int marque1[maxi];
	int marque2[maxj];

	POINT depart(-1, -1);
	POINT courant(-1, -1);

	

	/* Initialisation des tableaux de marqueurs*/
	for(int i=0; i<=maxi; i++)
		marque1[i] = 0;
	for(int j=0; j<=maxj; j++)
		marque2[j] = 0;

	/* Remplissage de p1 et p2 */
	/* Copie des deux premiers polygones*/
	for(int i=0; i<=maxi; i++)
		p1.ajouterPoints(i, (vecPoly[0].vecPoints[i]));
	for(int j=0; j<=maxj; j++)
		p2.ajouterPoints(j, (vecPoly[1].vecPoints[j]));
	/* Ajout des intersections */
	for(int i=0; i<=maxi; i++)
	{
		for(int j=0; j<=maxj; j++)
		{
			if(testIntersect(vecPoly[0].vecPoints[i%(maxi+1)], vecPoly[0].vecPoints[(i+1)%(maxi+1)], vecPoly[1].vecPoints[j%(maxj+1)], vecPoly[1].vecPoints[(j+1)%(maxj+1)]))
			{
				calculIntersect(vecPoly[0].vecPoints[i%(maxi+1)], vecPoly[0].vecPoints[(i+1)%(maxi+1)], vecPoly[1].vecPoints[j%(maxj+1)], vecPoly[1].vecPoints[(j+1)%(maxj+1)]);
				vecInter.push_back(INTERSECT(i+1, j+1, pInter.x, pInter.y));
			}
		}
	}
	for(int i=vecInter.size()-1; i>=0; i--)
	{
		POINT p(vecInter[i].x, vecInter[i].y);
		p1.ajouterPoints(vecInter[i].indice1, p);
	}

	vecInter.clear();
	for(int j=0; j<=maxj; j++)
		for(int i=0; i<=maxi; i++)
			if(testIntersect(vecPoly[0].vecPoints[i%(maxi+1)], vecPoly[0].vecPoints[(i+1)%(maxi+1)], vecPoly[1].vecPoints[j%(maxj+1)], vecPoly[1].vecPoints[(j+1)%(maxj+1)]))
	{
		calculIntersect(vecPoly[0].vecPoints[i%(maxi+1)], vecPoly[0].vecPoints[(i+1)%(maxi+1)], vecPoly[1].vecPoints[j%(maxj+1)], vecPoly[1].vecPoints[(j+1)%(maxj+1)]);
		vecInter.push_back(INTERSECT(i+1, j+1, pInter.x, pInter.y));
	}
	for(int i=vecInter.size()-1; i>=0; i--)
	{
		POINT p(vecInter[i].x, vecInter[i].y);
		p2.ajouterPoints(vecInter[i].indice2, p);
	}

	max1 = p1.vecPoints.size()-1;
	max2 = p2.vecPoints.size()-1;

	/* On prend comme point de départ le premier point du polygone 1 qui n'est pas à l'intérieur du polygone 2 */
	depart.x = p1.vecPoints[0].x;
	depart.y = p1.vecPoints[0].y;

	p3.ajouterPoints(0, depart);
	cpt3++;
	
	while((depart.x != courant.x) && (depart.y != courant.y))
	{
		if(!changePoly)
		{
			// pour chaque sommet du poly 1
			for(int i=(cpt1+1)%(max1+1); i<=max1+1 && !changePoly; i++)
			{
				courant.x = p1.vecPoints[i%(max1+1)].x;
				courant.y = p1.vecPoints[i%(max1+1)].y;
				if((depart.x == courant.x) && (depart.y == courant.y))
					break;
				// Si ce point est une intersection
				for(int k=0; k<=max2; k++)
					if((courant.x == p2.vecPoints[k].x) && (courant.y == p2.vecPoints[k].y))
				{
					p3.ajouterPoints(cpt3, courant);
					cpt3++;
						
					changePoly = true;
					cpt2 = k;
				}
				// Sinon on recopie dans le poly 3
				if(!changePoly)
				{
					p3.ajouterPoints(cpt3, courant);
					cpt3++;
				}
			}
		}
		if(changePoly)
		{
			//for(int j=(cpt2+1)%(max2+1); j<=max2+1 && changePoly; j++)
			for(int j=max2+1; j>=(cpt2+1)%(max2+1) && changePoly; j--)
			{
				courant.x = p2.vecPoints[j%(max2+1)].x;
				courant.y = p2.vecPoints[j%(max2+1)].y;
				if((depart.x == courant.x) && (depart.y == courant.y))
					break;
	
				for(int k=0; k<=max1; k++)
					if((courant.x == p1.vecPoints[k].x) && (courant.y == p1.vecPoints[k].y))
				{
					p3.ajouterPoints(cpt3, courant);
					cpt3++;
						
					changePoly = false;
					cpt1 = k;
				}
					
				if(changePoly)
				{
					p3.ajouterPoints(cpt3, courant);
					cpt3++;
				}
			}
		}
	}
}
void Intersection()
{
	if (indicePoly<1)
		return;
  
	int indice1 = 1, indice2 = 1, cpt1 = 0, cpt2 = 0, cpt3 = 0; 
	bool changePoly = false;
	int max1, max2;
	int maxi = vecPoly[0].vecPoints.size()-1;
	int maxj = vecPoly[1].vecPoints.size()-1;
	int marque1[maxi];
	int marque2[maxj];

	POINT depart(-1, -1);
	POINT courant(-1, -1);

	

	/* Initialisation des tableaux de marqueurs*/
	for(int i=0; i<=maxi; i++)
		marque1[i] = 0;
	for(int j=0; j<=maxj; j++)
		marque2[j] = 0;

	/* Remplissage de p1 et p2 */
	/* Copie des deux premiers polygones*/
	for(int i=0; i<=maxi; i++)
		p1.ajouterPoints(i, (vecPoly[0].vecPoints[i]));
	for(int j=0; j<=maxj; j++)
		p2.ajouterPoints(j, (vecPoly[1].vecPoints[j]));
	/* Ajout des intersections */
	for(int i=0; i<=maxi; i++)
	{
		for(int j=0; j<=maxj; j++)
		{
			if(testIntersect(vecPoly[0].vecPoints[i%(maxi+1)], vecPoly[0].vecPoints[(i+1)%(maxi+1)], vecPoly[1].vecPoints[j%(maxj+1)], vecPoly[1].vecPoints[(j+1)%(maxj+1)]))
			{
				calculIntersect(vecPoly[0].vecPoints[i%(maxi+1)], vecPoly[0].vecPoints[(i+1)%(maxi+1)], vecPoly[1].vecPoints[j%(maxj+1)], vecPoly[1].vecPoints[(j+1)%(maxj+1)]);
				vecInter.push_back(INTERSECT(i+1, j+1, pInter.x, pInter.y));
			}
		}
	}
	for(int i=vecInter.size()-1; i>=0; i--)
	{
		POINT p(vecInter[i].x, vecInter[i].y);
		p1.ajouterPoints(vecInter[i].indice1, p);
	}

	vecInter.clear();
	for(int j=0; j<=maxj; j++)
		for(int i=0; i<=maxi; i++)
			if(testIntersect(vecPoly[0].vecPoints[i%(maxi+1)], vecPoly[0].vecPoints[(i+1)%(maxi+1)], vecPoly[1].vecPoints[j%(maxj+1)], vecPoly[1].vecPoints[(j+1)%(maxj+1)]))
	{
		calculIntersect(vecPoly[0].vecPoints[i%(maxi+1)], vecPoly[0].vecPoints[(i+1)%(maxi+1)], vecPoly[1].vecPoints[j%(maxj+1)], vecPoly[1].vecPoints[(j+1)%(maxj+1)]);
		vecInter.push_back(INTERSECT(i+1, j+1, pInter.x, pInter.y));
	}
	for(int i=vecInter.size()-1; i>=0; i--)
	{
		POINT p(vecInter[i].x, vecInter[i].y);
		p2.ajouterPoints(vecInter[i].indice2, p);
	}

	max1 = p1.vecPoints.size()-1;
	max2 = p2.vecPoints.size()-1;

	/* On prend comme point de départ le premier point du polygone 1 qui n'est pas à l'intérieur du polygone 2 */
	depart.x = p1.vecPoints[0].x;
	depart.y = p1.vecPoints[0].y;

	p3.ajouterPoints(0, depart);
	cpt3++;
	
	while((depart.x != courant.x) && (depart.y != courant.y))
	{
		if(!changePoly)
		{
			// pour chaque sommet du poly 1
			for(int i=max1+1; i>=(cpt1+1)%(max1+1) && !changePoly; i--)
			{
				courant.x = p1.vecPoints[i%(max1+1)].x;
				courant.y = p1.vecPoints[i%(max1+1)].y;
				if((depart.x == courant.x) && (depart.y == courant.y))
					break;
				// Si ce point est une intersection
				for(int k=0; k<=max2; k++)
					if((courant.x == p2.vecPoints[k].x) && (courant.y == p2.vecPoints[k].y))
				{
					p3.ajouterPoints(cpt3, courant);
					cpt3++;
						
					changePoly = true;
					cpt2 = k;
				}
				// Sinon on recopie dans le poly 3
				if(!changePoly)
				{
					p3.ajouterPoints(cpt3, courant);
					cpt3++;
				}
			}
		}
		if(changePoly)
		{
			for(int j=max2+1; j>=(cpt2+1)%(max2+1) && changePoly; j--)
			{
				courant.x = p2.vecPoints[j%(max2+1)].x;
				courant.y = p2.vecPoints[j%(max2+1)].y;
				if((depart.x == courant.x) && (depart.y == courant.y))
					break;
	
				for(int k=0; k<=max1; k++)
					if((courant.x == p1.vecPoints[k].x) && (courant.y == p1.vecPoints[k].y))
				{
					p3.ajouterPoints(cpt3, courant);
					cpt3++;
						
					changePoly = false;
					cpt1 = k;
				}
					
				if(changePoly)
				{
					p3.ajouterPoints(cpt3, courant);
					cpt3++;
				}
			}
		}
	}
}
