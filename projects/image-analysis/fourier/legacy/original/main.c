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
/*----------C-----------*/
void writePixC(ImageC *d,int x,int y,PixelC valeur)
{
	if(x>=0 && y>=0 && x<d->width && y<d->height)
		d->data[y*d->width+x]=valeur;
}
PixelC readPixC(ImageC *d,int x,int y)
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
void BasculeImage(Image *image, GLubyte *I)
{
	int i,j;
	for(i=0;i<image->height;i++)
		for(j=0;j<image->width;j++)
			I[(image->height-1-i)*image->width+j]=(GLubyte) image->data[i*image->width+j];
	return;
}
//Expansion
void ExpansionDynamique (ImageD *IM, Image *IM2, int Bmin, int Bmax)
{
	int i; 
	double Amin, Amax;
	double a, b;
	Amin = IM->data[0];
	Amax = IM->data[0];
	
	for(i=0;i<IM->size; i++)
	{	if(IM->data[i]<Amin)
			Amin=IM->data[i];
		if(IM->data[i]>Amax)
			Amax=IM->data[i];
	}
	a = (double)(Bmax - Bmin) / (double)(Amax - Amin);
	b = (double)(Bmax - Bmin) * Amin / (double)(Amin - Amax);
		
	for(i=0;i<IM->size; i++)
		IM2->data[i] = (Pixel) (a * IM->data[i] + b);
}
/*-----------------------------------------------------*/
void Fourier(Image *in, ImageC *out)
{
	int c, i, j, l, k, t, T;
	PixelC pix_c;
	PixelC pix_cA;
	PixelC pix_cB;
	ImageC ImTempC;
	CreerImageC(&ImTempC, in->width, in->height);
	
	/*Lignes*/
	T = in->width;
	for(l = 0 ; l<in->height; l++)
	{	
		for(k=0;k<T;k++)
		{
			pix_c.reel=0.;
			pix_c.imaginaire=0.;
			for(t=0;t<T;t++)
			{
				pix_c.reel += readPix(in,l,t)*cos((-2*M_PI*k* t) / T);
				pix_c.imaginaire += readPix(in,l,t)*sin((-2*M_PI*k*t) / T);
			}
			writePixC(&ImTempC, l, k, pix_c );
		}
		for(i=0,j=T/2;i<T/2;i++,j++)
		{
			printf("T = %i | i= %i | j = %i\n",T,i,j);
			//a(0)<->a(k)
			//échanger les pixels d'une ligne 0<->k-1
			pix_cA = readPixC(&ImTempC,l,i);
			pix_cB = readPixC(&ImTempC,l,j);
			
			printf("A (%f,%f) | B (%f,%f)\n",pix_cA.reel, pix_cA.imaginaire,pix_cB.reel, pix_cB.imaginaire);
			
			writePixC(&ImTempC, l, i, pix_cB );
			writePixC(&ImTempC, l, j, pix_cA );
			
			pix_cA = readPixC(&ImTempC,l,i);
			pix_cB = readPixC(&ImTempC,l,j);
			
			printf("A (%f,%f) | B (%f,%f)\n",pix_cA.reel, pix_cA.imaginaire,pix_cB.reel, pix_cB.imaginaire);
		}
	}
	
	/*Colones*/
	T = in->height;
	for(c = 0 ; c<in->width; c++)
	{	
		for(k=0;k<T;k++)
		{
			pix_c.reel=0.;
			pix_c.imaginaire=0.;
			for(t=0;t<T;t++)
			{
				pix_cA.reel = readPixC(&ImTempC,t,c).reel;
				pix_cA.imaginaire = readPixC(&ImTempC,t,c).imaginaire;
				
				pix_cB.reel = cos((-2*M_PI*k* t) / T);
				pix_cB.imaginaire = sin((-2*M_PI*k*t) / T);
				//(a1 + i b1) (a2 + ib2)
				//(a1 * a2) + i(b1 * a2 + a1 * b2)
				pix_c.reel += (pix_cA.reel * pix_cB.reel)-(pix_cA.imaginaire*pix_cB.imaginaire);
				pix_c.imaginaire += (pix_cA.imaginaire * pix_cB.reel)+(pix_cA.reel*pix_cB.imaginaire);
			}
			writePixC(out, k, c, pix_c );
		}
		for(i=0,j=T/2;i<T/2;i++,j++)
		{
			//a(0)<->a(k)
			//échanger les pixels d'une ligne 0<->k-1
			pix_cA = readPixC(out,i,c);
			pix_cB = readPixC(out,j,c);
			
			writePixC(out, i, c, pix_cB );
			writePixC(out, j, c, pix_cA );
		}	
	}
}
void AffichageFourier(Image *in, Image *out)
{
	int x, y;
	float a, b;
	PixelD res;
	PixelC pix_c;
	ImageD ImTempD;
	ImageC ImTempC;
	CreerImageD(&ImTempD, in->width, in->height);
	CreerImageC(&ImTempC, in->width, in->height);
	
	Fourier(in, &ImTempC);
	/*Module*//*Log*/
	for(x = 0 ;x<in->width;x++)
		for(y=0;y<in->height;y++)
		{
			pix_c = readPixC( &ImTempC, x, y);
			a = pix_c.reel;
			b = pix_c.imaginaire;
			//printf("a : %f | b : %f \n",a,b);
			a = pow(a,2);
			b = pow(b,2);
			//printf("a² : %f | b² : %f \n",a,b);
			res = log (sqrt(a + b)+1);
			writePixD(&ImTempD, x, y,res);
			
			//printf("Res : %f \n",res);
		}
	/*Expansion dynamique*/
	ExpansionDynamique (&ImTempD, out, 0, 255);
}
void FourierInv(Image *in, ImageC *out)
{
	int c, i, j, l, k, t, T;
	PixelC pix_c;
	PixelC pix_cA;
	PixelC pix_cB;
	ImageC ImTempC;
	CreerImageC(&ImTempC, in->width, in->height);
	
	/*Lignes*/
	T = in->width;
	for(l = 0 ; l<in->height; l++)
	{	
		for(k=0;k<T;k++)
		{
			pix_c.reel=0.;
			pix_c.imaginaire=0.;
			for(t=0;t<T;t++)
			{
				pix_c.reel += readPix(in,l,t)*cos((+2*M_PI*k* t) / T);
				pix_c.imaginaire += readPix(in,l,t)*sin((+2*M_PI*k*t) / T);
			}
			pix_c.reel *= (1/(l<in->height* T));
			pix_c.imaginaire *= (1/(l<in->height* T));
			
			writePixC(&ImTempC, l, k, pix_c );
		}
		for(i=0,j=T/2;i<T/2;i++,j++)
		{
			//a(0)<->a(k)
			//échanger les pixels d'une ligne 0<->k-1
			pix_cA = readPixC(&ImTempC,l,i);
			pix_cB = readPixC(&ImTempC,l,j);
			
			writePixC(&ImTempC, l, i, pix_cB );
			writePixC(&ImTempC, l, j, pix_cA );
			
			pix_cA = readPixC(&ImTempC,l,i);
			pix_cB = readPixC(&ImTempC,l,j);
		}
	}
	/*Colones*/
	T = in->height;
	for(c = 0 ; c<in->width; c++)
	{	
		for(k=0;k<T;k++)
		{
			pix_c.reel=0.;
			pix_c.imaginaire=0.;
			for(t=0;t<T;t++)
			{
				pix_cA.reel = readPixC(&ImTempC,t,c).reel;
				pix_cA.imaginaire = readPixC(&ImTempC,t,c).imaginaire;
				
				pix_cB.reel = cos((-2*M_PI*k* t) / T);
				pix_cB.imaginaire = sin((-2*M_PI*k*t) / T);
				//(a1 + i b1) (a2 + ib2)
				//(a1 * a2) + i(b1 * a2 + a1 * b2)
				pix_c.reel += (pix_cA.reel * pix_cB.reel)-(pix_cA.imaginaire*pix_cB.imaginaire);
				pix_c.imaginaire += (pix_cA.imaginaire * pix_cB.reel)+(pix_cA.reel*pix_cB.imaginaire);
			}
			writePixC(out, k, c, pix_c );
		}
		for(i=0,j=T/2;i<T/2;i++,j++)
		{
			//a(0)<->a(k)
			//échanger les pixels d'une ligne 0<->k-1
			pix_cA = readPixC(out,i,c);
			pix_cB = readPixC(out,j,c);
			
			writePixC(out, i, c, pix_cB );
			writePixC(out, j, c, pix_cA );
		}	
	}
}
void AffichageFourierInv(Image *InOut)
{
	int x, y;
	float a, b;
	PixelD res;
	PixelC pix_c;
	ImageD ImTempD;
	ImageC ImTempC;
	CreerImageD(&ImTempD, InOut->width, InOut->height);
	CreerImageC(&ImTempC, InOut->width, InOut->height);
	
	FourierInv(InOut, &ImTempC);
	/*Module*//*Log*/
	for(x = 0 ;x<InOut->width;x++)
		for(y=0;y<InOut->height;y++)
		{
			pix_c = readPixC( &ImTempC, x, y);
			a = pix_c.reel;
			b = pix_c.imaginaire;
			//printf("a : %f | b : %f \n",a,b);
			a = pow(a,2);
			b = pow(b,2);
			//printf("a² : %f | b² : %f \n",a,b);
			res = log (sqrt(a + b)+1);
			writePixD(&ImTempD, x, y,res);
			
			//printf("Res : %f \n",res);
		}
	/*Expansion dynamique*/
	ExpansionDynamique (&ImTempD, InOut, 0, 255);
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
			AffichageFourier(&Im, &Im2);
			BasculeImage(&Im2,I2);
			glutSetWindow(f2);
			glutPostRedisplay();
		}
		break;
		case 8 :
		{
			
			AffichageFourierInv(&Im2);
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
	glutAddMenuEntry("Rien",6);
	glutAddMenuEntry("Fourier",7);
	glutAddMenuEntry("Fourier Inverse",8);
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
	glutInitWindowPosition(130,100);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	f2=glutCreateWindow("Fenetre 2 : TF");
	glutDisplayFunc(affichage2);
	glutReshapeFunc(redim);
	initGL();

	
	glutMainLoop();
	return 0;
} 
