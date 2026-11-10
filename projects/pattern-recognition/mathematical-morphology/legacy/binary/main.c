#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "OutilsPGM.h"

#define VRAI 0
#define FAUX 255

Image Im,Im2;          	     //Images originales
GLubyte *I1=NULL,*I2=NULL;   //Images à afficher

int f1,f2;
int ordre=1;

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
void Dilatation (Image *in, Image *out, int o) //Dilatation
{ 
	int x,y, i, j;
	initIm(out, FAUX);
	for(x = 0; x<in->width; x++)
		for(y = 0; y<in->height; y++)
		{
			if(readPix(in, x, y) == VRAI)
			{
				for(i = -o; i<o; i++)
					for(j = -o; j<o; j++)
						writePix(out, x+i, y+j, VRAI);
			}
		}
}

void Complementaire(Image *in, Image *out) //Inverser les couleurs - Négatif
{
	int x, y;
	for(x = 0; x<in->width; x++)
		for(y = 0; y<in->height; y++)
		{
			if(readPix(in, x, y) == FAUX)
				writePix(out, x, y, VRAI);
			else if (readPix(in, x, y) == VRAI)
				writePix(out, x, y, FAUX);
		}
}

void Erosion(Image *in, Image *out, int o)
{
	Image TempIm, TempIm2;
	CreerImage(&TempIm, in->width, in->height);
	CreerImage(&TempIm2, in->width, in->height);
	initIm(&TempIm, FAUX);
	Complementaire(in,&TempIm);
	Dilatation (&TempIm, &TempIm2, o);
	Complementaire(&TempIm2, out);
	LibererImage(&TempIm);
	LibererImage(&TempIm2);
}

void Ouverture(Image *in, Image *out, int o)//Erosion - Dilatation
{
	Image TempIm;
	CreerImage(&TempIm, in->width, in->height);
	Erosion(in, &TempIm, o);
	Dilatation(&TempIm, out, o);
	LibererImage(&TempIm);
}

void Fermeture(Image *in, Image *out, int o)//Dilatation - Erosion
{
	Image TempIm;
	CreerImage(&TempIm, in->width, in->height);
	Dilatation(in, &TempIm, o);
	Erosion(&TempIm, out, o);
	LibererImage(&TempIm);
}

void BasculeImage(Image *image, GLubyte *I)
{
	int i,j;
	for(i=0;i<image->height;i++)
		for(j=0;j<image->width;j++)
			I[(image->height-1-i)*image->width+j]=(GLubyte) image->data[i*image->width+j];
	return;
}

void initGL(void)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
}

void affichage1(void)
{
    glRasterPos2i(0,0);
    glDrawPixels(Im.width,Im.height,GL_LUMINANCE,GL_UNSIGNED_BYTE,I1);
    glFlush();
}

void affichage2(void)
{
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

void ChoixSousMenu(int value)
{
	ordre = value;
	printf("Ordre = %d\n", ordre);
}

void ChoixMenuPrincipal(int value)
{
	switch (value)
	{

		case 6: Erosion(&Im,&Im2,ordre);
			BasculeImage(&Im2,I2);
			glutSetWindow(f2);
			glutPostRedisplay();
			break;
		case 7: Dilatation(&Im,&Im2,ordre);
			BasculeImage(&Im2,I2);
			glutSetWindow(f2);
			glutPostRedisplay();
			break;
		case 8: Fermeture(&Im,&Im2,ordre);
			BasculeImage(&Im2,I2);
			glutSetWindow(f2);
			glutPostRedisplay();
			break;
		case 9: Ouverture(&Im,&Im2,ordre);
			BasculeImage(&Im2,I2);
			glutSetWindow(f2);
			glutPostRedisplay();
			break;
		case 10 : EcrireImage("res.pgm",&Im2);
			break;
		case 11 : initIm ( &Im2, FAUX);//INIT
			BasculeImage(&Im2,I2);
			glutSetWindow(f2);
			glutPostRedisplay();
			break;
		case 12 : Complementaire (&Im, &Im2);//NEGATIF
			BasculeImage(&Im2,I2);
			glutSetWindow(f2);
			glutPostRedisplay();
			break;
		case 13 : LibererImage(&Im);
			LibererImage(&Im2);
			free(I1);
			free(I2);
			exit(0); /* On quitte */
			break;
	} 
}


void CreerMenu(void)
{
	int sousMenu;
	
	sousMenu = glutCreateMenu(ChoixSousMenu);
	glutAddMenuEntry("1",1); 
	glutAddMenuEntry("2",2);
	glutAddMenuEntry("3",3);
	glutAddMenuEntry("4",4);
	glutAddMenuEntry("5",5);
	
	glutCreateMenu(ChoixMenuPrincipal);
	glutAddSubMenu("Ordre",sousMenu);
	glutAddMenuEntry("Erosion",6);
	glutAddMenuEntry("Dilatation",7);
	glutAddMenuEntry("Fermeture",8);
	glutAddMenuEntry("Ouverture",9);
	glutAddMenuEntry("EcrireImage",10);
	glutAddMenuEntry("Vider",11);
	glutAddMenuEntry("Negatif",12);
	glutAddMenuEntry("Quitter",13);
	
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
	glutInitWindowPosition(200,100);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	f1=glutCreateWindow("Fenetre 1");
	glutDisplayFunc(affichage1);
	glutReshapeFunc(redim);
	CreerMenu();
	initGL();
	
	/*----------------------------------------------------*/
	glutInitWindowSize(Im2.width,Im2.height);
	glutInitWindowPosition(500,100);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	f2=glutCreateWindow("Fenetre 2");
	glutDisplayFunc(affichage2);
	glutReshapeFunc(redim);
	initGL();
	
	glutMainLoop();
	return 0;
} 
