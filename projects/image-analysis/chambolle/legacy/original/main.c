#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "OutilsPGM.h"

#define VRAI 0
#define FAUX 255
#define NB_NV_GRIS 10000

Image Im, Im2, Im3;          	     //Images originales
GLubyte *I1=NULL,*I2=NULL,*I3=NULL, *I4=NULL;   //Images à afficher

int f1,f2,f3, f4;

int height_global, width_global;

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
void Divergence ( double * P1, double * P2, double *U)
{
	int i,j;
	double x=0,y=0;
	for(i=0; i<width_global; i++)
	{
		for(j=0; j<height_global; j++)
		{
			if (i > 1 && i < width_global)
				x = P1[i + j * width_global] - P1[(i-1) + j * width_global];
			else if(i==1)
				x = P1[i + j * width_global];
			else if ( i == width_global)
				x = -P1[(i-1) + j * width_global];
			if (j > 1 && j < height_global)
				y = P2[i + j * width_global] - P2[i + (j-1) * width_global];
			else if(j==1)
				y = P2[i + j * width_global];
			else if ( j == height_global)
				y = -P2[i + (j-1) * width_global];
			
			U[i + j * width_global] = x + y;
		}
		printf ("P1 [%i] = %f \n", i,P1 [i] );
	}

}
void Gradient ( double * U, double * G1, double * G2 )
{
	int i,j;
	for(i=0; i<width_global; i++)
	{
		for(j=0; j<height_global; j++)
		{
			if (i < width_global)
				G1 [i + j * width_global] = U[(i+1) + j * width_global] - U[i + j * width_global];
			else
				G1 [i + j * width_global] = 0;
			
		}
		//printf ("G1 [%i] = %f \n", i,G1 [i] );
	}
	for(i=0; i<width_global; i++)
	{
		for(j=0; j<height_global; j++)
		{
			if (i < width_global)
				G2 [i + j * width_global] = U[(i+1) + j * width_global] - U[i + j * width_global];
			else
				G2 [i + j * width_global] = 0;
		}
		//printf ("G2 [%i] = %f \n", i,G2 [i] );
	}
}
void Chambolle (Image *I, Image *J, int n, double tau, double lambda)
{// 0<= tau <= 1/8.
	int t, i;
	double norm;
	double P1 [I->size], P2[I->size];
	double G1 [I->size], G2[I->size];
	double U [I->size];
	
	width_global = I->width;
	height_global = I->height;
	
	//1
	for(i=0; i<I->size; i++)
	{
		P1[i] = 0;
		P2[i] = 0;
	}
	for(t=0; t<n; t++)
	{
		//2
		Divergence(P1, P2, U);
		//3
		for(i=0; i<I->size; i++)
			U[i] = U[i] - (I->data[i] / lambda);
		//4
		Gradient (U, G1, G2);
		//5
		for(i=0; i<I->size; i++)
		{
			norm = sqrt(pow (G1 [i], 2) + pow (G2 [i], 2));
			P1 [i] = (P1[i] + tau *  (G1[i] /(1 + tau *norm)));
			P2 [i] = (P2[i] + tau *  (G2[i] /(1 + tau *norm)));
		}
	}
	Divergence(P1, P2, U);
	for(i=0; i<I->size; i++)
	{
		J->data [i] = (int) ( I->data[i] - lambda * U[i]);
	}
}
void AfficheChambolle(Image *in, Image *out)
{
	int n = 20;
	double tau = 1/8.;
	//printf ("tau = %f \n", tau);
	double lambda = 20.;
	
	Chambolle (in, out, n, tau, lambda);
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
			AfficheChambolle (&Im,&Im2);
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
	glutAddMenuEntry("Chambolle",7);
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
	f2=glutCreateWindow("Fenetre 2 : Chambolle");
	glutDisplayFunc(affichage2);
	glutReshapeFunc(redim);
	initGL();

	
	glutMainLoop();
	return 0;
} 
