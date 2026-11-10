#include <cstring>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <GL/glut.h>
#include "polygone.h"

#define MAX 1000

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
void calculIntersect(POINT ptA, POINT ptB, POINT ptC, POINT ptD);
bool testIntersec(POINT ptA, POINT ptB, POINT ptC, POINT ptD);
void InitBandes();
void Remplissage();


Polygone polygone;
int indicePoint;
int indiceTrou;
int mode = 1;  		//1=saisie points, 2=saisie trous, 3=déplacement
bool clique = false;

int indiceDep;     		// indice du polygone à déplacer
Polygone DepPoly; 		//polygone contenant la différence entre les pts du polygone et la souris lors du déplacement
POINT pInter(-1, -1);	// Point d'intersection entre 2 segments;
double Y[MAX][2]; 		//tableau des bande de niveau, une chaque ordonnée d'un sommet correspond à la borne inférieur d'une bande [0]=num bande, [1]=ordonnée
double bornNiv[MAX];
bool AA[MAX];
int nbBornes;
int nbAA;
vector<POINT> vert;		// Points entre lesquels tracer les vertex pour le remplissage
int bcolor = 0;
int bclique = 0;

int main(int argc, char ** argv)
{
	indicePoint = 0;
	indiceTrou = 0;
	indiceDep = -1;
	glutInit(&argc, argv);
	glutInitWindowSize(800,800);
	glutInitWindowPosition(200,100);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	
	glutCreateWindow("Tp 5 - Remplissage d'un Polygône");
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

	/************* Coloriage ************/
	glBegin(GL_LINES);
	if (bclique == 1)
		glColor3f(0.2, 0.5, 0.5);
	else
		glColor3f(0.2, 0.8, 0.8);
	for(int i=0; i<vert.size(); i++)
		glVertex2f(vert[i].x, vert[i].y);
	glEnd();
	
	/************* Affichage contours ************/
	glLineWidth(2.0);
	glBegin(GL_LINE_STRIP);
	glColor3f(0.8, 0.2, 1.0);
	for(int j=0; j<polygone.vecPoints.size(); j++)
		glVertex2f(polygone.vecPoints[j].x, polygone.vecPoints[j].y);
	if(polygone.vecPoints.size() != 0)
		glVertex2f(polygone.vecPoints[0].x, polygone.vecPoints[0].y);
	glEnd();
	glPointSize(10.0);
	glBegin(GL_POINTS);
	glColor3f(1.0, 1.0, 0.0);
	for(int j=0; j<polygone.vecPoints.size(); j++)
		glVertex2f(polygone.vecPoints[j].x, polygone.vecPoints[j].y);
	glEnd();
	
	/************* Affichage trous ************/
	glBegin(GL_POLYGON);
	glColor3f(1.0, 1.0, 1.0);
	for(int j=0; j<polygone.vecTrou.size(); j++)
		glVertex2f(polygone.vecTrou[j].x, polygone.vecTrou[j].y);
	if(polygone.vecTrou.size() != 0)
		glVertex2f(polygone.vecTrou[0].x, polygone.vecTrou[0].y);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glColor3f(0.0, 1.0, 0.0);
	for(int j=0; j<polygone.vecTrou.size(); j++)
		glVertex2f(polygone.vecTrou[j].x, polygone.vecTrou[j].y);
	if(polygone.vecTrou.size() != 0)
		glVertex2f(polygone.vecTrou[0].x, polygone.vecTrou[0].y);
	glEnd();
	glPointSize(3.0);
	glBegin(GL_POINTS);
	glColor3f(0.0, 0.0, 1.0);
	for(int j=0; j<polygone.vecTrou.size(); j++)
		glVertex2f(polygone.vecTrou[j].x, polygone.vecTrou[j].y);
	glEnd();

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
		case 'z' : break;
		case 't' : break;		
		case 'e' : break;	
		case 'c' : break;
		case 'p' : break;
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
			for(int i=0; i<polygone.vecPoints.size(); i++)
				if((x > polygone.vecPoints[i].x-5) && (x < polygone.vecPoints[i].x+5) && (OHeight - y > polygone.vecPoints[i].y-5) && (OHeight - y < polygone.vecPoints[i].y+5))
					indiceSuppr = i;

			if(indiceSuppr != -1)
			{
				polygone.supprimerPoints(indiceSuppr);
				indicePoint --;
			}
			else
			{
				polygone.ajouterPoints(indicePoint, POINT(x, OHeight - y));
				indicePoint++;
			}
			
		}
		
		if(button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) 
			for(int i = 0; i<polygone.vecPoints.size();i++)
				if((x > polygone.vecPoints[i].x-5) && (x < polygone.vecPoints[i].x+5) && (OHeight - y > polygone.vecPoints[i].y-5) && (OHeight - y < polygone.vecPoints[i].y+5))
					indicePoint = i+1;
		//Recolorer le polygone
		if (bcolor == 1)
		{
			vert.clear();
			Remplissage();
		}
	}
	/********* Mode Saisie Trous**************/
	if(mode == 2)
	{
		if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			for(int i=0; i<polygone.vecTrou.size(); i++)
				if((x > polygone.vecTrou[i].x-5) && (x < polygone.vecTrou[i].x+5) && (OHeight - y > polygone.vecTrou[i].y-5) && (OHeight - y < polygone.vecTrou[i].y+5))
					indiceSuppr = i;

			if(indiceSuppr != -1)
			{
				polygone.supprimerPointsTrou(indiceSuppr);
				indiceTrou --;
			}
			else
			{
				polygone.ajouterPointsTrou(indiceTrou, POINT(x, OHeight - y));
				indiceTrou++;
			}
		}
		
		if(button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) 
			for(int i = 0; i<polygone.vecTrou.size();i++)
				if((x > polygone.vecTrou[i].x-5) && (x < polygone.vecTrou[i].x+5) && (OHeight - y > polygone.vecTrou[i].y-5) && (OHeight - y < polygone.vecTrou[i].y+5))
					indiceTrou = i+1;
	}
	/********* Mode Deplacement **************/
	if(mode == 3)
	{
		if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			if(!TestInterieur(x, y))
				printf("exterieur \n");
			else
			{
				for(int i=0; i<polygone.vecPoints.size(); i++)
					DepPoly.ajouterPoints(i, POINT((int)(x-polygone.vecPoints[i].x), (int)(OHeight-y-polygone.vecPoints[i].y))); 
				for(int i=0; i<polygone.vecTrou.size(); i++)
					DepPoly.ajouterPointsTrou(i, POINT((int)(x-polygone.vecTrou[i].x), (int)(OHeight-y-polygone.vecTrou[i].y)));
				clique = true;
			}
			bclique =1;
		}
		if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		{
			bclique =0;
			clique = false;
		}
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
	glutAddMenuEntry("Remplissage", 4);
	glutAddMenuEntry("Effacer", 6);

	glutAddMenuEntry("Quitter", 27);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
void ChoixMenuPrincipal(int value)
{
	switch (value)
	{
		case 1:
			mode = 1;
		break;
		case 2:
			mode = 2;
		break;
		case 3:
			mode = 3;
		break;
		case 4: InitBandes();
			Remplissage();
			bcolor = 1;
		break;
		case 6: polygone.vecPoints.clear();
			//polygone.vecTrou.clear();
			vert.clear();
			indicePoint = 0;
			indiceTrou = 0;
			mode = 1;
		break;

		case 27: exit(0);
		break;
	}
}
bool TestInterieur(int x, int y)
{
	int nbInter = 0;
	double Xi;
	
	y = OHeight - y;

		// On test les intersections avec le contours
		for(int j=0; j<polygone.vecPoints.size(); j++)
		{
			// Cas simple
			if((polygone.vecPoints[j].x>x && polygone.vecPoints[j].y>y && polygone.vecPoints[(j+1)%polygone.vecPoints.size()].x>x && polygone.vecPoints[(j+1)%polygone.vecPoints.size()].y<y) ||
			(polygone.vecPoints[j].x>x && polygone.vecPoints[j].y<y && polygone.vecPoints[(j+1)%polygone.vecPoints.size()].x>x && polygone.vecPoints[(j+1)%polygone.vecPoints.size()].y>y))
				nbInter++;

			// Cas "diagonal"
			if((polygone.vecPoints[j].x>x && polygone.vecPoints[j].y>y && polygone.vecPoints[(j+1)%polygone.vecPoints.size()].x<x && polygone.vecPoints[(j+1)%polygone.vecPoints.size()].y<y) || 
			(polygone.vecPoints[j].x>x && polygone.vecPoints[j].y<y && polygone.vecPoints[(j+1)%polygone.vecPoints.size()].x<x && polygone.vecPoints[(j+1)%polygone.vecPoints.size()].y>y) ||
			(polygone.vecPoints[j].x<x && polygone.vecPoints[j].y<y && polygone.vecPoints[(j+1)%polygone.vecPoints.size()].x>x && polygone.vecPoints[(j+1)%polygone.vecPoints.size()].y>y) ||
			(polygone.vecPoints[j].x<x && polygone.vecPoints[j].y>y && polygone.vecPoints[(j+1)%polygone.vecPoints.size()].x>x && polygone.vecPoints[(j+1)%polygone.vecPoints.size()].y<y))
			{
				Xi = (y - polygone.vecPoints[j].y)*(polygone.vecPoints[j].x - polygone.vecPoints[(j+1)%polygone.vecPoints.size()].x) / (polygone.vecPoints[j].y - polygone.vecPoints[(j+1)%polygone.vecPoints.size()].y) + polygone.vecPoints[j].x;
				if(Xi >= x)
					nbInter++;
			}
		}

		Xi = 0;

		// On test les intersections avec le contours du trou
		for(int j=0; j<polygone.vecTrou.size(); j++)
		{
			// Cas simple
			if((polygone.vecTrou[j].x>x && polygone.vecTrou[j].y>y && polygone.vecTrou[(j+1)%polygone.vecTrou.size()].x>x && polygone.vecTrou[(j+1)%polygone.vecTrou.size()].y<y) ||
			(polygone.vecTrou[j].x>x && polygone.vecTrou[j].y<y && polygone.vecTrou[(j+1)%polygone.vecTrou.size()].x>x && polygone.vecTrou[(j+1)%polygone.vecTrou.size()].y>y))
				nbInter++;

			// Cas "diagonal"
			if((polygone.vecTrou[j].x>x && polygone.vecTrou[j].y>y && polygone.vecTrou[(j+1)%polygone.vecTrou.size()].x<x && polygone.vecTrou[(j+1)%polygone.vecTrou.size()].y<y) || 
			(polygone.vecTrou[j].x>x && polygone.vecTrou[j].y<y && polygone.vecTrou[(j+1)%polygone.vecTrou.size()].x<x && polygone.vecTrou[(j+1)%polygone.vecTrou.size()].y>y) ||
			(polygone.vecTrou[j].x<x && polygone.vecTrou[j].y<y && polygone.vecTrou[(j+1)%polygone.vecTrou.size()].x>x && polygone.vecTrou[(j+1)%polygone.vecTrou.size()].y>y) ||
			(polygone.vecTrou[j].x<x && polygone.vecTrou[j].y>y && polygone.vecTrou[(j+1)%polygone.vecTrou.size()].x>x && polygone.vecTrou[(j+1)%polygone.vecTrou.size()].y<y))
			{
				Xi = (y - polygone.vecTrou[j].y)*(polygone.vecTrou[j].x - polygone.vecTrou[(j+1)%polygone.vecTrou.size()].x) / (polygone.vecTrou[j].y - polygone.vecTrou[(j+1)%polygone.vecTrou.size()].y) + polygone.vecTrou[j].x;
				if(Xi >= x)
					nbInter++;
			}
		}
		if(nbInter%2)
			return true;
		nbInter = 0;

	// Si nb paire d'intersection = exterieur
	return false;
}
void deplacement(int x, int y)
{
	if(clique)
	{	for(int i=0; i<polygone.vecPoints.size(); i++)
		{
			polygone.vecPoints[i].x = x-DepPoly.vecPoints[i].x;
			polygone.vecPoints[i].y = OHeight-y-DepPoly.vecPoints[i].y;
		}
		for(int i=0; i<polygone.vecTrou.size(); i++)
		{
			polygone.vecTrou[i].x = x-DepPoly.vecTrou[i].x;
			polygone.vecTrou[i].y = OHeight-y-DepPoly.vecTrou[i].y;
		}
	}
	//Recolorer le polygone
	if (bcolor == 1)
	{
		vert.clear();
		Remplissage();
	}
}
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
void InitBandes()
{
	int i, j;
	int indice;
	double max = 0;
	double borne_sup = MAX;
	double val;
	int cpt = 0;
	
	/* tableau Y */
	for(i=0; i<polygone.vecPoints.size(); i++)
	{
		for(j=0; j<polygone.vecPoints.size(); j++)
		{
			if(polygone.vecPoints[j].y > max && polygone.vecPoints[j].y < borne_sup)
			{
				indice = j;
				max = polygone.vecPoints[j].y;
			}
		}
		Y[i][0] = indice;
		Y[i][1] = max;
		
		borne_sup = max;
		max = 0;
	}
	cout << "Y :" << endl;
	for(i=0; i<polygone.vecPoints.size(); i++)
		cout << Y[i][0] << " " << Y[i][1] << endl;
	
	for(i=0; i<polygone.vecPoints.size(); i++)
		AA[i] = false;
	nbAA = 0;
}
//Etape 1
void MajAA(int i)
{
	// On change l'état des aretes entourants le sommet i
	if(i == 0)
	{
		if(AA[polygone.vecPoints.size()-1] == true)
		{
			AA[polygone.vecPoints.size()-1] = false;
			nbAA--;
		}
		else
		{
			AA[polygone.vecPoints.size()-1] = true;
			nbAA++;
		}
	}
	else
	{
		if(AA[i-1] == true)
		{
			AA[i-1] = false;
			nbAA--;
		}
		else
		{
			AA[i-1] = true;
			nbAA++;
		}
	}
	if(AA[i] == true)
	{
		AA[i] = false;
		nbAA--;
	}
	else
	{
		AA[i] = true;
		nbAA++;
	}
}
//Etape 2
void BandeNiv(int ni)
{
	int i,j,k, l, temp, min, id; 
	int AAX[MAX][2];
	int cpt = 0;
	POINT p1(0,0), p2(500,0);
	//On répertorie les arètes courantes
	for(i = 0; i<polygone.vecPoints.size(); i ++)
	{
		if(AA[i] == true)
		{
			AAX[cpt][0] = i;
			cpt++;
		}
	}
	//Etape 3
	//On Définit les points d'intersections entre les arètes courantes
	for(l=(int) Y[ni][1]; l>=Y[ni+1][1]; l--)
	{
		p1.y = l;
		p2.y = l;
		for(i = 0; i<nbAA; i ++)
		{
			id = AAX[i][0]+1;
			if(id >= polygone.vecPoints.size())
				id = 0;
			calculIntersect(p1, p2, polygone.vecPoints[AAX[i][0]], polygone.vecPoints[id]);
			AAX[i][1] = (int) pInter.x;
		}
		//tri de AAX
		for(i = 0; i<nbAA; i ++)
		{
			min = 100000;
			for ( j = i ; j < nbAA ; j++ )
			{
				if ( AAX[j][1] < min )
				{
					k = j;
					min = AAX[j][1] ;
				}
			}
			temp = AAX[i][1];
			AAX[i][1] = AAX[k][1];
			AAX[k][1] = temp;
		}
		for(i = 0; i<nbAA; i ++)
			vert.push_back(POINT(AAX[i][1], l));
	}
}
void Remplissage()
{
	int sommetcourante;
	InitBandes();
	for(int i = 0; i<polygone.vecPoints.size(); i ++)
	{
		//On définit le Sommet Courant
		sommetcourante = (int) Y[i][0];
		//On définit les arètes actives issut du Sommet Courant
		MajAA(sommetcourante);
		//On définit les bandes de niveaux à partir
		BandeNiv(i);
	}
}
