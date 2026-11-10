#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "OutilsPGM.h"

#define VRAI 0
#define FAUX 255
#define NB_NV_GRIS 255

Image Im,Im2,Im3, Im4;          	     //Images originales
GLubyte *I1=NULL,*I2=NULL,*I3=NULL, *I4=NULL;   //Images à afficher

int f1,f2,f3, f4;
int ordre=1;
int s=159;

typedef struct
{
	int taille;//Ordre
	float * coef;
}Filtre;

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

void initIm ( Image *img, Pixel val) //Vide l'image : remplit l'image 2 en Blanc
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
//1- Copier
void Copier(Image *in, Image *out) //Copier
{
	int x,y;
	initIm ( out, FAUX);
	for(x = 0; x<in->width; x++)
		for(y = 0; y<in->height; y++)
			writePix(out, x, y, readPix(in, x, y));
}
//Filtre Linéaire
void FiltreLineaire(Image *In, Image *out, Filtre *fl)
{
	int y,x,b,n,l,c;
	float s;
	b=1;
	for(y=0;y<In->height;y++)
		for(x=0;x<In->width;x++)
	{
		s=0.;
		n=0;

		for(l=y-b;l<=y+b;l++)
			for(c=x-b;c<=x+b;c++)
		{
			s+=(float) readPix(In,c,l)*fl->coef[n];
			n++;
		}
		out->data[y*In->width+x] = (int) s;
	}
} 
//Expansion
void ExpansionDynamique (Image *IM, Image *IM2, int Bmin, int Bmax)
{
	int i, Amin, Amax;
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
	
/*	a = (Bmin - Bmax)/(( (double)Amin-Amax));
	b = Bmin -(( Bmin-Bmax)/(double)(( Amin - Amax))) * Amax;
	*/	for(i=0;i<IM->size; i++)
		IM2->data[i] = (Pixel) (a * IM->data[i] + b);
}
//Filtre Sobel & Gradient
void FiltreSobel(Image *Im, Image *F, Image *F2, Filtre *fx, Filtre *fy)
{
	Image ImX;
	CreerImagePGM(&ImX, Im->width, Im->height);
	Copier(Im,&ImX);
	Image ImY;
	CreerImagePGM(&ImY, Im->width, Im->height);
	Copier(Im,&ImY);
	
	FiltreLineaire(Im, &ImX, fx);
	FiltreLineaire(Im, &ImY, fy);
	ExpansionDynamique (&ImX, F, 0,255);
	ExpansionDynamique (&ImY, F2, 0,255);
	
	LibererImagePGM(&ImX);
	LibererImagePGM(&ImY);
}
//NORME GRADIENT
void NormeGradient(Image *ImX, Image* ImY, Image* NG)//racine de x² + y²
{
	int i;
	for(i=0;i<NG->size;i++)
		NG->data[i] = (int) sqrt(pow(ImX->data[i],2) + pow(ImY->data[i], 2));
}
void seuillage(Image *Im1, Image *Im2, int s)
{
	int i;
	for(i=0;i<Im1->size;i++)
	{
		if(Im1->data[i]>s)
			Im2->data[i] = 255;
		else
			Im2->data[i] = 0;
	}
}
void FiltreSobel_NG(Image *Im, Image *F, Image *F2, Image *NG, Filtre *fx, Filtre *fy)
{
	Image ImX;
	CreerImagePGM(&ImX, Im->width, Im->height);
	Copier(Im,&ImX);
	Image ImY;
	CreerImagePGM(&ImY, Im->width, Im->height);
	Copier(Im,&ImY);
	Image ImNG;
	CreerImagePGM(&ImNG, Im->width, Im->height);
	
	FiltreLineaire(Im, &ImX, fx);
	FiltreLineaire(Im, &ImY, fy);
	NormeGradient(&ImX, &ImY, NG);
	ExpansionDynamique (&ImX, F, 0,255);
	ExpansionDynamique (&ImY, F2, 0,255);
	
	ExpansionDynamique (NG, &ImNG, 0,255);
	seuillage(&ImNG, NG, s);
	
	LibererImagePGM(&ImX);
	LibererImagePGM(&ImY);
}

void clavier (unsigned char key, int x, int y)
{
  switch(key)
  {
	case 's':
	case 'S':
	printf("Entrer un seuil : ");
	fflush(stdout);
      	scanf("%d",&s);
	  break;
	case 'q':
	case 'Q':
	{
		if(s>=0)
		{
			s=s-5;
			printf("%d \n",s);
			Filtre fx, fy;
			fx.taille=3;
			fx.coef=malloc(9*sizeof(float));
			fx.coef[0] =-1;
			fx.coef[1] =0;
			fx.coef[2] =1;
			fx.coef[3] =-2;
			fx.coef[4] =0;
			fx.coef[5] =2;
			fx.coef[6] =-1;
			fx.coef[7] =0;
			fx.coef[8] =1;
		
			fy.taille=3;
			fy.coef=malloc(9*sizeof(float));
			fy.coef[0] =-1;
			fy.coef[1] =-2;
			fy.coef[2] =-1;
			fy.coef[3] =0;
			fy.coef[4] =0;
			fy.coef[5] =0;
			fy.coef[6] =1;
			fy.coef[7] =2;
			fy.coef[8] =1;
				
			FiltreSobel_NG(&Im, &Im2, &Im3, &Im4, &fx, &fy);
			BasculeImage(&Im2,I2);
			glutSetWindow(f2);
			glutPostRedisplay();
			BasculeImage(&Im3,I3);
			glutSetWindow(f3);
			glutPostRedisplay();
			BasculeImage(&Im4,I4);
			glutSetWindow(f4);
			glutPostRedisplay();
		}
	}
	break;
	case 'd':
	case 'D':
	{
		if(s<=255)
		{
			s=s+5;
			printf("%d \n",s);
			Filtre fx, fy;
			fx.taille=3;
			fx.coef=malloc(9*sizeof(float));
			fx.coef[0] =-1;
			fx.coef[1] =0;
			fx.coef[2] =1;
			fx.coef[3] =-2;
			fx.coef[4] =0;
			fx.coef[5] =2;
			fx.coef[6] =-1;
			fx.coef[7] =0;
			fx.coef[8] =1;
		
			fy.taille=3;
			fy.coef=malloc(9*sizeof(float));
			fy.coef[0] =-1;
			fy.coef[1] =-2;
			fy.coef[2] =-1;
			fy.coef[3] =0;
			fy.coef[4] =0;
			fy.coef[5] =0;
			fy.coef[6] =1;
			fy.coef[7] =2;
			fy.coef[8] =1;
				
			FiltreSobel_NG(&Im, &Im2, &Im3, &Im4, &fx, &fy);
			BasculeImage(&Im2,I2);
			glutSetWindow(f2);
			glutPostRedisplay();
			BasculeImage(&Im3,I3);
			glutSetWindow(f3);
			glutPostRedisplay();
			BasculeImage(&Im4,I4);
			glutSetWindow(f4);
			glutPostRedisplay();
		}
	}
	break;
  }
}
//-----------------------------------------------------
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
    //glClear(GL_COLOR_BUFFER_BIT);
    glRasterPos2i(0,0);
    glDrawPixels(Im2.width,Im2.height,GL_LUMINANCE,GL_UNSIGNED_BYTE,I2);
    glFlush();
}
void affichage3(void)
{
	//glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2i(0,0);
	glDrawPixels(Im3.width,Im3.height,GL_LUMINANCE,GL_UNSIGNED_BYTE,I3);
	glFlush();
}
void affichage4(void)
{
	//glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2i(0,0);
	glDrawPixels(Im4.width,Im4.height,GL_LUMINANCE,GL_UNSIGNED_BYTE,I4);
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
			BasculeImage(&Im,I2);
			glutSetWindow(f2);
			glutPostRedisplay();
			BasculeImage(&Im,I3);
			glutSetWindow(f3);
			glutPostRedisplay();
			BasculeImage(&Im,I4);
			glutSetWindow(f4);
			glutPostRedisplay();
		break;
		case 7 :
		{
			Filtre fl;
			int ifl;
			fl.taille = 9;
			fl.coef = (float *)malloc(sizeof(float) * fl.taille);
			for(ifl = 0; ifl<fl.taille;ifl++)//Coef de Filtrage
			{fl.coef [ifl]=1.0/( (double) fl.taille); 
			//printf("fl = %f\n",fl.coef [ifl]);
			}
			FiltreLineaire(&Im, &Im2, &fl);
			BasculeImage(&Im2,I2);
			glutSetWindow(f2);
			glutPostRedisplay();
		}
		break;
		case 8 :
		{
			Filtre fx, fy;
			fx.taille=3;
			fx.coef=malloc(9*sizeof(float));
			fx.coef[0] =-1;
			fx.coef[1] =0;
			fx.coef[2] =1;
			fx.coef[3] =-2;
			fx.coef[4] =0;
			fx.coef[5] =2;
			fx.coef[6] =-1;
			fx.coef[7] =0;
			fx.coef[8] =1;
	
			fy.taille=3;
			fy.coef=malloc(9*sizeof(float));
			fy.coef[0] =-1;
			fy.coef[1] =-2;
			fy.coef[2] =-1;
			fy.coef[3] =0;
			fy.coef[4] =0;
			fy.coef[5] =0;
			fy.coef[6] =1;
			fy.coef[7] =2;
			fy.coef[8] =1;
			
			FiltreSobel(&Im, &Im2, &Im3, &fx, &fy);
			BasculeImage(&Im2,I2);
			glutSetWindow(f2);
			glutPostRedisplay();
			BasculeImage(&Im3,I3);
			glutSetWindow(f3);
			glutPostRedisplay();
		}
		break;
		case 10 :
		{
			Filtre fx, fy;
			fx.taille=3;
			fx.coef=malloc(9*sizeof(float));
			fx.coef[0] =-1;
			fx.coef[1] =0;
			fx.coef[2] =1;
			fx.coef[3] =-2;
			fx.coef[4] =0;
			fx.coef[5] =2;
			fx.coef[6] =-1;
			fx.coef[7] =0;
			fx.coef[8] =1;
	
			fy.taille=3;
			fy.coef=malloc(9*sizeof(float));
			fy.coef[0] =-1;
			fy.coef[1] =-2;
			fy.coef[2] =-1;
			fy.coef[3] =0;
			fy.coef[4] =0;
			fy.coef[5] =0;
			fy.coef[6] =1;
			fy.coef[7] =2;
			fy.coef[8] =1;
			
			FiltreSobel_NG(&Im, &Im2, &Im3, &Im4, &fx, &fy);
			BasculeImage(&Im2,I2);
			glutSetWindow(f2);
			glutPostRedisplay();
			BasculeImage(&Im3,I3);
			glutSetWindow(f3);
			glutPostRedisplay();
			BasculeImage(&Im4,I4);
			glutSetWindow(f4);
			glutPostRedisplay();
		}
		break;
		case 11 : LibererImagePGM(&Im);
			LibererImagePGM(&Im2);
			LibererImagePGM(&Im3);
			LibererImagePGM(&Im4);
			free(I1);
			free(I2);
			free(I3);
			free(I4);
			exit(0); /* On quitte */
			break;
	}

}
void CreerMenu(void)
{
	
	glutCreateMenu(ChoixMenuPrincipal);
	glutAddMenuEntry("Bascule Image",6);
	glutAddMenuEntry("Filtre Lineaire",7);
	glutAddMenuEntry("Filtre Sobel",8);
	glutAddMenuEntry("NG",10);
	glutAddMenuEntry("Quitter",11);
	
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
int main(int argc,char **argv)
{
	glutInit(&argc,argv);
	
	/* Lecture de l'image pgm*/
	char *nom = argv[1];
	if (LireImagePGM(&Im,nom)==-1) return -1;
	
	/* Allocation de la memoire des images à afficher */
	if ( (I1=(GLubyte *) malloc(sizeof(GLubyte)*Im.width*Im.height)) == NULL)
	{
		printf("Impossible d'allouer l'image I1 à afficher\n");
		return -1;
	}
	
	CreerImagePGM(&Im2,Im.width,Im.height);
	if ( (I2=(GLubyte *) malloc(sizeof(GLubyte)*Im2.width*Im2.height)) == NULL)
	{
		printf("Impossible d'allouer l'image I2 à afficher\n");
		return -1;
	}
	
	CreerImagePGM(&Im3,Im.width,Im.height);
	if ( (I3=(GLubyte *) malloc(sizeof(GLubyte)*Im3.width*Im3.height)) == NULL)
	{
		printf("Impossible d'allouer l'image I3 à afficher\n");
		return -1;
	}
	CreerImagePGM(&Im4,Im.width,Im.height);
	if ( (I4=(GLubyte *) malloc(sizeof(GLubyte)*Im4.width*Im4.height)) == NULL)
	{
		printf("Impossible d'allouer l'image I4 à afficher\n");
		return -1;
	}
    
	/* Transformation de l'image donnée en image à afficher */
	BasculeImage(&Im,I1);
	BasculeImage(&Im,I2);
	BasculeImage(&Im,I3);
	BasculeImage(&Im,I4);
	
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
	glutInitWindowPosition(800,100);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	f2=glutCreateWindow("Fenetre 2 : X");
	glutDisplayFunc(affichage2);
	glutReshapeFunc(redim);
	initGL();
	/*----------------------------------------------------*/
	glutInitWindowSize(Im3.width,Im3.height);
	glutInitWindowPosition(000,800);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	f3=glutCreateWindow("Fenetre 3 : Y");
	glutDisplayFunc(affichage3);
	glutReshapeFunc(redim);
	initGL();
	/*----------------------------------------------------*/
	glutInitWindowSize(Im4.width,Im4.height);
	glutInitWindowPosition(800,800);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	f4=glutCreateWindow("Fenetre 4 : NG");
	glutDisplayFunc(affichage4);
	glutReshapeFunc(redim);
	initGL();
	
	glutMainLoop();
	return 0;
} 
