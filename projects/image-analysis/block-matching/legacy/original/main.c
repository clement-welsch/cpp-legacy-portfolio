#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "OutilsPGM.h"

#define VRAI 0
#define FAUX 255
#define NB_NV_GRIS 10000
#define INT_MAX 9999

Image Im, Im2, Im3;          	     //Images originales
GLubyte *I1=NULL,*I2=NULL,*I3=NULL, *I4=NULL;   //Images à afficher

int f1,f2,f3, f4;

int height_global, width_global;

//Variable Perso
int TailleBloc = 5; // Taille des Blocs
int P=10; // Déplacement maximal autorisé dans la direction x et y


typedef struct
{
	double x;
	double y;
}Vecteur;

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
double Similarite(Image *I1, Image *I2, Vecteur b1, Vecteur b2)
{
	double sim = 0;
	int i, j;
	for(i=0;i<TailleBloc; i++)
		for(j=0;j<TailleBloc; j++)
		{
			sim = sim + pow (I1->data[(int) ((j+b1.y)* I1->width + i+ b1.x)]
					-I2->data[(int)((j+b2.y)* I2->width + i+ b2.x)],2);
		}
	return sim;
}
void Blockmatching(Image *I1, Image * I2, Vecteur **D)
{
	int sim, msim;
	int i, j, u, v, U0=0, V0=0;
	Vecteur b1,b2;
	for(i=0;i<I1->width/TailleBloc; i++)
		for(j=0;j<I1->width/TailleBloc; j++)
		{
			b1.x=i*TailleBloc;
			b1.y=j*TailleBloc;
			msim = INT_MAX;
			for(u=0;u<P; u++)
				for(v=0;v<P; v++)
				{
					b2.x=b1.x + u;
					b2.y=b1.y + v;
					if((b2.x<0)||(b2.y<0) || (b2.x>=I2->width-TailleBloc) 
									       ||(b2.y>=I2->height-TailleBloc))
					{}
					else
					{
						sim = Similarite(I1, I2, b1, b2);
						if (sim <msim)
						{
							msim = sim;
							U0 = u;
							V0 = v;
						}
					}
				}
			D[i][j].x= U0;
			D[i][j].y= V0;
		}
}
void AfficheBlockmatching(Image * in, Image *out)
{
	int i,j;
	Vecteur D[10][10];
	Blockmatching(in, out, D);
	BasculeImage(out,I2);
	glutSetWindow(f2);
	//On trace les vecteurs sur l'image initiale
	for(i=0;i<I1->width/TailleBloc; i++)
		for(j=0;j<I1->width/TailleBloc; j++)
		{
			glBegin(GL_LINES)
					
		}
	
}
/*-----------------------------------------------------*/
void clavier (unsigned char key, int x, int y)
{
	switch(key)
	{
		case 's':
		case 'S':
			
		break;
		case 'q':
		case 'Q':
		{
		}
		break;
		case 'd':
		case 'D':
		{
		}
		break;
		case 'a':
		case 'A':
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
    glRasterPos2i(0,0);
    glDrawPixels(Im2.width,Im2.height,GL_LUMINANCE,GL_UNSIGNED_BYTE,I2);
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
			AfficheBlockmatching (&Im,&Im2);
			BasculeImage(&Im2,I2);
			glutSetWindow(f2);
			glutPostRedisplay();
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
	glutAddMenuEntry("Blockmatching",7);
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
	glutInitWindowPosition(322,100);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	f2=glutCreateWindow("Fenetre 2 : Blockmatching");
	glutDisplayFunc(affichage2);
	glutReshapeFunc(redim);
	initGL();

	
	glutMainLoop();
	return 0;
} 
