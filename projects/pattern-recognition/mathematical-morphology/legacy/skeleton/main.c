#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "OutilsPGM.h"

#define VRAI 0
#define FAUX 255
#define NDEF -1

//  Famille de voisinages destinée à l'amincissement

short vois[8][9]=
{{FAUX,FAUX,FAUX,NDEF,VRAI,NDEF,VRAI,VRAI,VRAI},     // <--- un  premier voisinage de 9 éléments (ordre 1)
{VRAI,NDEF,FAUX,VRAI,VRAI,FAUX,VRAI,NDEF,FAUX},      // <--- un second  voisinage de 9 éléments (ordre 1)
{VRAI,VRAI,VRAI,NDEF,VRAI,NDEF,FAUX,FAUX,FAUX},     // <--- ...
{FAUX,NDEF,VRAI,FAUX,VRAI,VRAI,FAUX,NDEF,VRAI},
{NDEF,FAUX,FAUX,VRAI,VRAI,FAUX,NDEF,VRAI,NDEF},
{NDEF,VRAI,NDEF,VRAI,VRAI,FAUX,NDEF,FAUX,FAUX},
{NDEF,VRAI,NDEF,FAUX,VRAI,VRAI,FAUX,FAUX,NDEF},
{FAUX,FAUX,NDEF,FAUX,VRAI,VRAI,NDEF,VRAI,NDEF}};


//  Famille de voisinages destinée à l'ébarbulage du squelette

short vois2[8][9]=
{{FAUX,FAUX,VRAI,FAUX,VRAI,FAUX,FAUX,FAUX,FAUX},
{FAUX,FAUX,FAUX,FAUX,VRAI,FAUX,FAUX,FAUX,VRAI},
{FAUX,FAUX,FAUX,FAUX,VRAI,FAUX,VRAI,FAUX,FAUX},
{VRAI,FAUX,FAUX,FAUX,VRAI,FAUX,FAUX,FAUX,FAUX},
{FAUX,VRAI,FAUX,FAUX,VRAI,FAUX,FAUX,FAUX,FAUX},
{FAUX,FAUX,FAUX,FAUX,VRAI,VRAI,FAUX,FAUX,FAUX},
{FAUX,FAUX,FAUX,FAUX,VRAI,FAUX,FAUX,VRAI,FAUX},
{FAUX,FAUX,FAUX,VRAI,VRAI,FAUX,FAUX,FAUX,FAUX}};

Image Im,Im2,Im3;          	     //Images originales
GLubyte *I1=NULL,*I2=NULL;   //Images à afficher

int f1,f2;
int ordre=1;

void writePix(Image *d,int x,int y,Pixel valeur)
{
	if(x>=0 && y>=0 && x<d->width && y<d->height)
		d->data[y*d->width+x]=valeur;
}

short readPix( const Image *d,int x,int y)
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
//---------------------------------
//---------------TP2-------------- Im o Vi = Im/Im x Vi 
//------------------------------- / = XOR
//------------------------------ x = TFV
//----------------------------- o = Rond = f(f(x))
//--------------------------- Im o V = ((I o V1) o V2) o V3
//1- Copier
void Copier(Image *in, Image *out) //Copier
{
	int x,y;
	initIm ( out, FAUX);
	for(x = 0; x<in->width; x++)
		for(y = 0; y<in->height; y++)
			writePix(out, x, y, readPix(in, x, y));
}
//2- Idem
short Idem(const Image *in, const Image *out)
{
	int i=0,res=1;
	while ((i<in->size)&&(res == 1))
	{
		if (in->data[i]!=out->data[i])
			res = 0;
		i++;
	}
	return res;
}
//3- XOR
void XOR(const Image *d, const Image *d2, Image *res)
{
	int i;
	for(i = 0; i<d->size; i++)
	{
		if( ((d->data[i])^(d2->data[i])) )
			res->data[i] = VRAI;
		else
			res->data[i] = FAUX;
	}
}
//4- EXISTE VOISINAGE
short ExisteVoisinage(const Image *d, int i, int j, short v[9])
{
	int x,y, k, teste=1;
	k=0;
	for(y=-1;y<=1;y++)
		for(x=-1;x<=1;x++)
			{
				if(v[k]!=NDEF)
					if(readPix(d, i+x,j+y)!=v[k])
						teste = 0;
				k++;
			}
	return teste;
}
//5- TFV
void TFV (const Image *Im, Image *Im2, short v[9])
{
	int i, j;
	
	
	for(i=0;i<Im->width;i++)
		for(j=0;j<Im->height;j++)
	{
		if(ExisteVoisinage(Im,i,j,v))
			Im2->data[j*Im->width+i] = VRAI;
		else
			Im2->data[j*Im->width+i] = FAUX;
	}
}
//6-Rond : Im o Vi = Im/(Im x Vi) (/ : XOR | TFV : x)
void Rond (const Image *d, Image *res, short v[9])
{
	Image TempIm;
	CreerImage(&TempIm, d->width, d->height);
	//Im x Vi
	TFV (d, &TempIm,v);
	//Im/(Im x Vi)
	XOR(d, &TempIm, res);
	
	LibererImage(&TempIm);
	
}
//7-Amincissement : Im oV = ((Im o V1)) o V2)o V3....o V8
void Amin(const Image *d, Image  *res, short V[8][9])
{
	int i;
	Image TempIm;
	CreerImage(&TempIm, d->width, d->height);
	
	Copier(d,&TempIm);
	for (i = 0; i< 8; i++)
	{
		Rond (&TempIm, res, V[i]);
		Copier(res,&TempIm);
	}
	
	LibererImage(&TempIm);
}
//8-Epaississement
void Epais(Image *d,Image *r, short V[8][9])
{
	Image TempIm;
	CreerImage(&TempIm, d->width, d->height);
	Image TempIm2;
	CreerImage(&TempIm2, d->width, d->height);
	
	Complementaire(d, &TempIm);
	Amin(&TempIm, &TempIm2, V);
	Complementaire(&TempIm2, r);

	LibererImage(&TempIm);
	LibererImage(&TempIm2);
}
//9-Squelette
void Squel(const Image *d, Image *r, short V[8][9])
// appelle de la fonction aminc jusqu'à idempotence (cf. fonction idem). Au moins une image temporaire ici aussi...
{
	Image TempIm;
	CreerImage(&TempIm, d->width, d->height);
	Copier(d,&TempIm);
	Amin(&TempIm, r, V);
	while(Idem(r,&TempIm)!=1)
	{
		Copier(r,&TempIm);
		Amin(&TempIm, r, V);
	}
}
//10-Ebarbulage
void Barbules(const Image *d, Image *r, int lg, short V[8][9])
{}
// Enlève les barbules de longueurs au plus lg

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
{int i;
	switch (value)
	{

		case 6: Erosion(&Im,&Im2,ordre);//EROSION
			BasculeImage(&Im2,I2);
			glutSetWindow(f2);
			glutPostRedisplay();
			break;
			case 7: Dilatation(&Im,&Im2,ordre);//DILATATION
			BasculeImage(&Im2,I2);
			glutSetWindow(f2);
			glutPostRedisplay();
			break;
			case 8: Fermeture(&Im,&Im2,ordre);//FERMETURE
			BasculeImage(&Im2,I2);
			glutSetWindow(f2);
			glutPostRedisplay();
			break;
			case 9: Ouverture(&Im,&Im2,ordre);//OUVERTURE
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
		case 13 : Copier (&Im, &Im2);//COPIE
			BasculeImage(&Im2,I2);
			glutSetWindow(f2);
			glutPostRedisplay();
			break;
		case 14 :
			i=Idem (&Im, &Im2);//IDEM
			if (i == 0)
			{
				Complementaire (&Im, &Im2);
				BasculeImage(&Im2,I2);
				glutSetWindow(f2);
				glutPostRedisplay();
			}
			break;
		case 15 : XOR (&Im, &Im2, &Im3);//XOR
			BasculeImage(&Im3,I2);
			glutSetWindow(f2);
			glutPostRedisplay();
			break;
			case 16 : /*ExisteVoisinage(&Im, &Im2,vois[0]);//EXISTE VOISINAGE
			BasculeImage(&Im2,I2);
			glutSetWindow(f2);
				glutPostRedisplay();*/
			break;
		case 17 : TFV(&Im, &Im2,vois[0]);//TFV
			BasculeImage(&Im2,I2);
			glutSetWindow(f2);
			glutPostRedisplay();
			break;
		case 18 : Rond(&Im, &Im2,vois[0]);//ROND
			BasculeImage(&Im2,I2);
			glutSetWindow(f2);
			glutPostRedisplay();
			break;
		case 19 : Amin (&Im, &Im2, vois);//AMINCISSEMENT
			BasculeImage(&Im2,I2);
			glutSetWindow(f2);
			glutPostRedisplay();
			break;
		case 20 : Squel (&Im, &Im2, vois);//SQUELETTE
			BasculeImage(&Im2,I2);
			glutSetWindow(f2);
			glutPostRedisplay();
			break;
		case 21 : Epais (&Im, &Im2, vois);//EPAISSISSEMENT
			BasculeImage(&Im2,I2);
			glutSetWindow(f2);
			glutPostRedisplay();
			break;
		case 22 : LibererImage(&Im);
			LibererImage(&Im2);
			LibererImage(&Im3);
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
	glutAddMenuEntry("Copier",13);
	glutAddMenuEntry("Idem",14);
	glutAddMenuEntry("XOR",15);
	glutAddMenuEntry("Existance Voisinage",16);
	glutAddMenuEntry("TFV",17);
	glutAddMenuEntry("Rond",18);
	glutAddMenuEntry("Amincissement",19);
	glutAddMenuEntry("Squelette",20);
	glutAddMenuEntry("Epaississement",21);
	glutAddMenuEntry("Quitter",22);
	
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
	CreerImage(&Im3,Im.width,Im.height);
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
