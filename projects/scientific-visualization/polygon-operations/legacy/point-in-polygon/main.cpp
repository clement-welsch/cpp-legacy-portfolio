
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

Polygone polygone;
vector<Polygone> vecPoly;
int mode = 1;  //1=saisie points, 2=saisie trous, 3=déplacement
int indicePoint;
int indiceTrou;
int indicePoly;

bool clique = false;

int indiceDep;     // indice du polygone à déplacer
Polygone DepPoly; //polygone contenant la différence entre les pts du polygone et la souris lors du déplacement

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
	
	glutCreateWindow("Polygon creator");
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
				printf("exterieur \n");
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
	glutAddMenuEntry("Effacer", 4);
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
		case 4: vecPoly.clear();
			vecPoly.push_back(polygone);
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
