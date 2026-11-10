#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <unistd.h>
#include <vector>
#include <stdio.h>
#include "OutilsPGM.h"

#define VRAI 0
#define FAUX 255
#define NB_NV_GRIS 10000

Image Im, Im2, Im3;          	     //Images originales
GLubyte *I1=NULL,*I2=NULL,*I3=NULL, *I4=NULL;   //Images à afficher

int f1,f2,f3, f4;

int height_global, width_global;

struct Carre
{
	int _id;
	int _imin;
	int _imax;
	int _jmin;
	int _jmax;
	int _parent;
	std::vector<int> vListeVoisinage;
};
//vListeCarre.insert(int,obj);
//vListeCarre.begin()
//vListeCarre.at(int)
std::vector<Carre> vListeCarre;

//Variable Perso
int iMax, iMin;
int idCarre = 0;
bool bSplit = false;
unsigned int idVoisin = 10;

void writePix(Image *d,int x,int y,Pixel valeur)
{
	if(x>=0 && y>=0 && x<d->width && y<d->height)
		d->data[y*d->width+x]=valeur;
}
short readPix(Image *d,int x,int y)
{
	if(x<0)
		x=abs(x);
	if(x>=d->width)
		x=d->width-(x-d->width)-1;
	if(y<0)
		y=abs(y);
	if(y>=d->height)
		y=d->height-(y-d->height)-1;
	return d->data[y*d->width+x];
}
void initIm ( Image *img, Pixel val) //Vide l'image : remplit l'image en Blanc
{
	int x, y;
	for(x = 0; x<img->width; x++)
		for(y = 0; y<img->height; y++)
			writePix(img, x, y, val);
}
void BasculeImage(Image *image, GLubyte *I)
{
	int i,j;
	for(i=0;i<image->height;i++)
		for(j=0;j<image->width;j++)
			I[(image->height-1-i)*image->width+j]=(GLubyte) image->data[i*image->width+j];
	return;
}
/*-----------------------------------------------------*/
void CopieImage (Image *in, Image *out)
{
	int i,j;
	
	for (i = 0; i < in->width; i++)
		for (j = 0; j < in->height; j++)
			writePix(out, i, j, readPix(in,i,j));
}
void FullSquare(Image *out, int imin, int imax, int jmin, int jmax)
{
	int i,j;
	for (i = imin; i < imax; i++)
		for (j = jmin; j < jmax; j++)
		writePix(out, i,j,255);
}
void Square(Image *out, int imin, int imax, int jmin, int jmax)
{
	int i,j;
	for (i = imin; i < imax; i++)
		writePix(out, i,jmin,255);
	for (j = jmin; j < jmax; j++)
		writePix(out, imin,j,255);
	for (i = imin; i < imax; i++)
		writePix(out, i,jmax-1,255);
	for (j = jmin; j < jmax; j++)
	writePix(out, imax-1,j,255);
}
int Max_Min(Image *in, int imin, int imax, int jmin, int jmax)
{
	int i, j, Min, Max;
	
	Min = 255;
	Max = -1;
	
	for (i = imin; i < imax-1; i++)
	{
		for (j = jmin; j < jmax-1; j++)
		{
			if (readPix(in, i, j)<Min)
			  Min = readPix(in, i, j);
			if (readPix(in, i, j)>Max)
			  Max = readPix(in, i, j);
		}
	}
	if ((Max != -1) && (Min != -1))
	  return Max-Min;
	else
	  return 0;
}
void Split_Rec(Image *in, int imin, int imax, int jmin, int jmax, int idParent)
{
	Carre temp;
	temp._id = idCarre;
	temp._imin = imin;
	temp._imax = imax;
	temp._jmin = jmin;
	temp._jmax = jmax;
	temp._parent = idParent;
	vListeCarre.insert(vListeCarre.begin()+idCarre,temp);
	idCarre ++;
	
	if (Max_Min(in, imin,imax,jmin,jmax) > 100)
	{
		if ((imax >= 10) && (jmax >= 10))
		{
			Split_Rec(in, imin, (imax-imin)/2+imin, jmin, (jmax-jmin)/2+jmin, idCarre-1);
			Split_Rec(in, (imax-imin)/2+imin, imax, jmin, (jmax-jmin)/2+jmin, idCarre-1);
			Split_Rec(in, imin, (imax-imin)/2+imin, (jmax-jmin)/2+jmin, jmax, idCarre-1);
			Split_Rec(in, (imax-imin)/2+imin, imax, (jmax-jmin)/2+jmin, jmax, idCarre-1);
		}
	}
}
void DrawSplit(Image *out)
{
	int i;
	glutSetWindow(f2);
	for (i = 0; i < idCarre; i++)
		Square(out, vListeCarre.at(i)._imin, vListeCarre.at(i)._imax, vListeCarre.at(i)._jmin, vListeCarre.at(i)._jmax);
}
void Voisinage()
{
	int i,j, imin, imax, jmin, jmax, idVoisin;
	unsigned int k, l;
	//on parcours l'ensemble des bords d'un carré
	for (k = 0; k < vListeCarre.size(); k++)
	{
		imin = vListeCarre.at(k)._imin;
		imax = vListeCarre.at(k)._imax;
		jmin = vListeCarre.at(k)._jmin;
		jmax = vListeCarre.at(k)._jmax;
		idVoisin = 0;
		//On parcours le bord du haut
		for (i = imin; i < imax; i++)
			for (l = 0; l < vListeCarre.size(); l++)
			{
				//On cherche le carré auquel appartient le pixel ciblé
				if ((i < vListeCarre.at(l)._imin)&&(i < vListeCarre.at(l)._imax)&&(jmin-1 > vListeCarre.at(l)._jmin)&&(jmin-1 < vListeCarre.at(l)._jmax))
				{
					bool b=true;
					//On cherche si le carré est déjà enregistré dans la liste de Voisinage
					for (unsigned int m = 0; m < vListeCarre.at(k).vListeVoisinage.size(); m++)
					{
						if ((unsigned int)vListeCarre.at(k).vListeVoisinage.at(m) == l)
							b = false;
					}
					//Enregistrement du carré
					if (b == true)
						vListeCarre.at(k).vListeVoisinage.insert(vListeCarre.at(k).vListeVoisinage.begin()+idVoisin, l);
				}
			}
		//On parcours le bord de gauche
		for (j = jmin; j < jmax; j++)
			for (l = 0; l < vListeCarre.size(); l++)
			{
				//On cherche le carré auquel appartient le pixel ciblé
				if ((imin-1 < vListeCarre.at(l)._imin)&&(imin-1 < vListeCarre.at(l)._imax)&&(j > vListeCarre.at(l)._jmin)&&(j < vListeCarre.at(l)._jmax))
				{
					bool b=true;
					//On cherche si le carré est déjà enregistré dans la liste de Voisinage
					for (unsigned int m = 0; m < vListeCarre.at(k).vListeVoisinage.size(); m++)
					{
						if ((unsigned int)vListeCarre.at(k).vListeVoisinage.at(m) == l)
							b = false;
					}
					//Enregistrement du carré
					if (b == true)
						vListeCarre.at(k).vListeVoisinage.insert(vListeCarre.at(k).vListeVoisinage.begin()+idVoisin, l);
				}
			}
		//On parcours le bord du bas
		for (i = imin; i < imax; i++)
			for (l = 0; l < vListeCarre.size(); l++)
			{
				//On cherche le carré auquel appartient le pixel ciblé
				if ((i < vListeCarre.at(l)._imin)&&(i < vListeCarre.at(l)._imax)&&(jmax+1 > vListeCarre.at(l)._jmin)&&(jmax+1 < vListeCarre.at(l)._jmax))
				{
					bool b=true;
					//On cherche si le carré est déjà enregistré dans la liste de Voisinage
					for (unsigned int m = 0; m < vListeCarre.at(k).vListeVoisinage.size(); m++)
					{
						if ((unsigned int)vListeCarre.at(k).vListeVoisinage.at(m) == l)
							b = false;
					}
					//Enregistrement du carré
					if (b == true)
						vListeCarre.at(k).vListeVoisinage.insert(vListeCarre.at(k).vListeVoisinage.begin()+idVoisin, l);
				}
			}
		//On parcours le bord de droite
		for (j = jmin; j < jmax; j++)
			for (l = 0; l < vListeCarre.size(); l++)
			{
				//On cherche le carré auquel appartient le pixel ciblé
				if ((imax+1 < vListeCarre.at(l)._imin)&&(imax+1 < vListeCarre.at(l)._imax)&&(j > vListeCarre.at(l)._jmin)&&(j < vListeCarre.at(l)._jmax))
				{
					bool b=true;
					//On cherche si le carré est déjà enregistré dans la liste de Voisinage
					for (unsigned int m = 0; m < vListeCarre.at(k).vListeVoisinage.size(); m++)
					{
						if ((unsigned int)vListeCarre.at(k).vListeVoisinage.at(m) == l)
							b = false;
					}
					//Enregistrement du carré
					if (b == true)
						vListeCarre.at(k).vListeVoisinage.insert(vListeCarre.at(k).vListeVoisinage.begin()+idVoisin, l);
				}
			}
	}
}
void DrawVoisinage(Image *out)
{
	glutSetWindow(f2);
	int imin, imax, jmin, jmax;
	
	imin = vListeCarre.at(idVoisin)._imin;
	imax = vListeCarre.at(idVoisin)._imax;
	jmin = vListeCarre.at(idVoisin)._jmin;
	jmax = vListeCarre.at(idVoisin)._jmax;
	
	FullSquare(out, imin, imax, jmin, jmax);
	
	for (unsigned int k = 0; k < vListeCarre.at(idVoisin).vListeVoisinage.size(); k++)
	{
		imin = vListeCarre.at(vListeCarre.at(idVoisin).vListeVoisinage.at(k))._imin;
		imax = vListeCarre.at(vListeCarre.at(idVoisin).vListeVoisinage.at(k))._imax;
		jmin = vListeCarre.at(vListeCarre.at(idVoisin).vListeVoisinage.at(k))._jmin;
		jmax = vListeCarre.at(vListeCarre.at(idVoisin).vListeVoisinage.at(k))._jmax;
		
		Square(out, imin, imax, jmin, jmax);
	}
}
// Merger les voisins
void Merge ()
{
	
}
/*-----------------------------------------------------*/
void clavier (unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'a':
		case 'A':
		{
			if ((idVoisin > 0) && (bSplit == true))
			{
				idVoisin--;
				CopieImage (&Im, &Im2);
				DrawVoisinage(&Im2);
				BasculeImage(&Im2,I2);
				glutPostRedisplay();
				
				std::cout<<idVoisin<<std::endl;
			}
		}
		break;
		case 'z':
		case 'Z':
		{
			if ((idVoisin < vListeCarre.size()) && (bSplit == true))
			{
				idVoisin++;
				CopieImage (&Im, &Im2);
				DrawVoisinage(&Im2);
				BasculeImage(&Im2,I2);
				glutPostRedisplay();
				
				std::cout<<idVoisin<<std::endl;
			}
		}
		break;
		case 'd':
		case 'D':
		{
		}
		break;
		case 'e':
		case 'E':
		{
		}
		break;
		case 'w':
		case 'W':
		{
		}
		break;
		case 'c':
		case 'C':
		{
		}
		break;
		case 27 :
			LibererImage(&Im);
			LibererImage(&Im2);
			free(I1);
			free(I2);
			exit(0); /* On quitte */
		break;
	}
}
void initGL(void)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
}
void affichage1(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2i(0,0);
	glDrawPixels(Im.width,Im.height,GL_LUMINANCE,GL_UNSIGNED_BYTE,I1);
	glFlush();
}
void affichage2(void)
{
	/*glClear(GL_COLOR_BUFFER_BIT);*/
	if (bSplit == true)
	{
		DrawVoisinage(&Im2);
	}
	glRasterPos2i(0,0);
	glDrawPixels(Im2.width,Im2.height,GL_LUMINANCE,GL_UNSIGNED_BYTE,I2);
	glutSwapBuffers();
	glFlush();
}
void redim(int width,int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,width,0,height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void ChoixMenuPrincipal(int value)
{
	switch (value)
	{
		case 6 :
		{
			BasculeImage(&Im,I2);
			glutSetWindow(f2);
			glutPostRedisplay();
		}
		break;
		case 7 :
		{
			bSplit = true;
			BasculeImage(&Im,I2);
			CopieImage (&Im, &Im2);
			Split_Rec (&Im, 0, Im.width, 0, Im.height, 0);
			//DrawSplit(&Im2);
			Voisinage();
			BasculeImage(&Im2,I2);
			glutSetWindow(f2);
			glutPostRedisplay();
			printf("----------test 7------------- \n");
		}
		break;
		case 27 : 
		{
			LibererImage(&Im);
			LibererImage(&Im2);
			free(I1);
			free(I2);
			exit(0); /* On quitte */
		}
		break;
	}
}
void CreerMenu(void)
{
	glutCreateMenu(ChoixMenuPrincipal);
	glutAddMenuEntry("BasculeImage",6);
	glutAddMenuEntry("Split",7);
	glutAddMenuEntry("Quitter",27);
	
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
int main(int argc,char **argv)
{
	
	glutInit(&argc,argv);
	
	/* Lecture de l'image pgm*/
	char *nom = argv[1];
	if (LireImage(nom,&Im)==-1) return -1;
	
	/* Allocation de la memoire des images à afficher */
	if ( (I1=(GLubyte *) malloc(sizeof(GLubyte)*Im.width*Im.height)) == NULL)
	{
		printf("Impossible d'allouer l'image I1 à afficher\n");
		return -1;
	}
	
	CreerImage(&Im2,Im.width,Im.height);
	if ( (I2=(GLubyte *) malloc(sizeof(GLubyte)*Im2.width*Im2.height)) == NULL)
	{
		printf("Impossible d'allouer l'image I2 à afficher\n");
		return -1;
	}
    
	/* Transformation de l'image donnée en image à afficher */
	BasculeImage(&Im,I1);
	BasculeImage(&Im,I2);
	
	/*------------------------------------------------------*/
	glutInitWindowSize(Im.width,Im.height);
	glutInitWindowPosition(000,100);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	f1=glutCreateWindow("Fenetre 1");
	glutDisplayFunc(affichage1);
	glutKeyboardFunc(clavier);
	glutReshapeFunc(redim);
	CreerMenu();
	initGL();
	
	/*----------------------------------------------------*/
	glutInitWindowSize(Im2.width,Im2.height);
	glutInitWindowPosition(801,100);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	f2=glutCreateWindow("Fenetre 2 : Split & Merge");
	glutDisplayFunc(affichage2);
	glutReshapeFunc(redim);
	initGL();

	
	glutMainLoop();
	return 0;
}
