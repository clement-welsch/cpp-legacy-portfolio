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
/*-----------------------*/
/*----------D-----------*/
/*-----------------------*/
void writePixD(ImageD *d,int x,int y,PixelD valeur)
{
	if(x>=0 && y>=0 && x<d->width && y<d->height)
		d->data[y*d->width+x]=valeur;
}
PixelD readPixD(ImageD *d,int x,int y)
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
/*-----------------------*/
void initIm ( Image *img, Pixel val) //Vide l'image : remplit l'image en Blanc
{
	int x, y;
	for(x = 0; x<img->width; x++)
		for(y = 0; y<img->height; y++)
			writePix(img, x, y, val);
}
void initImD ( ImageD *img, PixelD val) //Vide l'image : remplit l'image en Blanc
{
	int x, y;
	for(x = 0; x<img->width; x++)
		for(y = 0; y<img->height; y++)
			writePixD(img, x, y, val);
}
void BasculeImage(Image *image, GLubyte *I)
{
	int i,j;
	for(i=0;i<image->height;i++)
		for(j=0;j<image->width;j++)
			I[(image->height-1-i)*image->width+j]=(GLubyte) image->data[i*image->width+j];
	return;
}
void Copier(Image *in, Image *out) //Copier Image -> Image
{
	int x,y;
	initIm ( out, FAUX);
	for(x = 0; x<in->width; x++)
		for(y = 0; y<in->height; y++)
			writePix(out, x, y, readPix(in, x, y));
}
void CopierD(ImageD *in, ImageD *out) //Copier ImageD -> ImageD
{
	int x,y;
	initImD ( out, (double)FAUX);
	for(x = 0; x<in->width; x++)
		for(y = 0; y<in->height; y++)
			writePixD(out, x, y, readPixD(in, x, y));
}
void CopierversD(Image *in, ImageD *out) //Copier Image -> ImageD
{
	int x,y;
	initImD ( out, (double)FAUX);
	for(x = 0; x<in->width; x++)
		for(y = 0; y<in->height; y++)
			writePixD(out, x, y, (double)readPix(in, x, y));
}
void CopierversS(ImageD *in, Image *out) //Copier ImageD -> Image
{
	int x,y;
	initIm ( out, FAUX);
	for(x = 0; x<in->width; x++)
		for(y = 0; y<in->height; y++)
			writePix(out, x, y, (short)readPixD(in, x, y));
}
//Expansion
void ExpansionDynamique (Image *IM, Image *IM2, int Amin, int Amax, int Bmin, int Bmax)
{
	int i;
	double a, b;
	
	a = (double)(Bmax - Bmin) / (double)(Amax - Amin);
	b = (double)(Bmax - Bmin) * Amin / (double)(Amin - Amax);
	
	for(i=0;i<IM->size; i++)
		IM2->data[i] = (Pixel) (a * IM->data[i] + b);
}
/*-----------------------------------------------------*/
void Laplacien (ImageD * in, ImageD *out)
{
	int i, j;
	PixelD pix;
	
	for(i = 0; i<in->width; i++)
		for(j = 0; j<in->height; j++)
		{
			pix = readPixD(in, i-1,j) + readPixD(in, i+1,j) + readPixD(in, i,j-1) + readPixD(in, i,j+1) - 4 * readPixD(in, i,j);
			writePixD(out,i,j, pix);
		}
}
double Distance (ImageD * in, ImageD *out)
{
	int i, j;
	double somme=0, dist,Temp, Temp2,C;
	
	for(i = 0; i<in->width; i++)
		for(j = 0; j<in->height; j++)
		{
		    Temp = readPixD(in, i,j);
		    Temp2 = readPixD(out, i,j);
		    C = Temp-Temp2;
			somme += pow(C,2);
		}
	  
	dist = sqrt(somme);
	return dist;
}

void Inpainting (Image *dep, Image *mask, Image *res, double dt, double lam)
{
	int i, j;
	int maxi, mini;
	double dist = 100;
	double dTemp, dTemp2, dTemp3, sTemp, sTemp2;
	PixelD pix;
	Image temp;
	CreerImage(&temp, dep->width, dep->height);
	
	ImageD U1, U2, lap;
	CreerImageD(&U1, dep->width, dep->height);
	CreerImageD(&U2, dep->width, dep->height);
	CreerImageD(&lap, dep->width, dep->height);
	CopierversD(dep,&U1);
	
	while (dist > 0.05)
	{
	  printf("dist : %f \n", dist);
		Laplacien (&U1, &lap);
		for(i = 0; i<dep->width; i++)
			for(j = 0; j<dep->height; j++)
			{
				dTemp = readPixD(&U1, i, j);
				sTemp = (double)readPix(mask, i, j);
				sTemp2 = (double)readPix(dep, i, j);
				dTemp2 = readPixD(&U1, i, j);
				dTemp3 = readPixD(&lap, i, j);
				
				pix = dTemp + dt * (2 * sTemp /255 * (sTemp2 - dTemp2) + lam * dTemp3);
				writePixD(&U2, i, j, pix);
			}
		dist = Distance (&U1, &U2);
		CopierD(&U2, &U1);
	}
	CopierversS(&U1, &temp);
	
	mini = dep->data[0];
	maxi = dep->data[0];
	
	for(i=0;i<dep->size; i++)
	{	if(dep->data[i]<mini)
			mini=dep->data[i];
		if(dep->data[i]>maxi)
			maxi=dep->data[i];
	}
	ExpansionDynamique (&temp, res, mini, maxi, 0, 255);
	
	LibererImage(&temp);
	LibererImageD(&U1);
	LibererImageD(&U2);
}
void AfficheInpainting(Image *in, Image *out)
{
	Image res;
	CreerImage(&res, in->width, in->height);
	
	Inpainting (in, out, &res, 10, 100);
	Copier(&res,out);
	
	LibererImage(&res);
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
	glClear(GL_COLOR_BUFFER_BIT);
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
			AfficheInpainting (&Im,&Im2);
			printf("toto");
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
	glutAddMenuEntry("Inpainting",7);
	glutAddMenuEntry("Quitter",27);
	
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
int main(int argc,char **argv)
{
	
	glutInit(&argc,argv);
	
	/* Lecture de l'image pgm*/
	char *nom = argv[1];
	char *cMask = argv[2];
	if (LireImage(nom,&Im)==-1) return -1;
	if (LireImage(cMask,&Im2)==-1) return -1;
	
	/* Allocation de la memoire des images à afficher */
	if ( (I1=(GLubyte *) malloc(sizeof(GLubyte)*Im.width*Im.height)) == NULL)
	{
		printf("Impossible d'allouer l'image I1 à afficher\n");
		return -1;
	}
	
	/*CreerImage(&Im2,Im.width,Im.height);*/
	if ( (I2=(GLubyte *) malloc(sizeof(GLubyte)*Im2.width*Im2.height)) == NULL)
	{
		printf("Impossible d'allouer l'image I2 à afficher\n");
		return -1;
	}

	
    
	/* Transformation de l'image donnée en image à afficher */
	BasculeImage(&Im,I1);
	BasculeImage(&Im2,I2);
	
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
	f2=glutCreateWindow("Fenetre 2");
	glutDisplayFunc(affichage2);
	glutReshapeFunc(redim);
	initGL();

	
	glutMainLoop();
	return 0;
} 
