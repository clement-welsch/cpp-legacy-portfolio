#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "OutilsPGM.h"

#define VRAI 0 // Noir
#define FAUX 255 //Blanc
#define SOUSENS 122
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
void Complementaire(const Image *in, Image *out) //Inverser les couleurs - Négatif
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
void Erosion(const Image *in, Image *out, int o)
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
void Copier(const Image *in, Image *out) //Copier
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
	LibererImage(&TempIm);
}
//10-Ebarbulage
void Barbules(const Image *d, Image *r, int lg, short V[8][9])
{ 
}
// Enlève les barbules de longueurs au plus lg

//---------------------------------
//---------------TP3--------------
//-------------------------------
//------------------------------
//-----------------------------
//---------------------------

void DilatationGeod(Image *in, Image *out, int o)
{
	int x,y, i, j;
	Copier(in, out);
	for(x = 0; x<in->width; x++)
		for(y = 0; y<in->height; y++)
	{
		if(readPix(in, x, y) == SOUSENS)
		{
			for(i = -o; i<=o; i++)
				for(j = -o; j<=o; j++)
			{
				if (readPix(in, x+i, y+j) == VRAI)
					writePix(out, x+i, y+j, SOUSENS);
			}
		}
	}
}
void OR(Image *d, Image *r, int o)
{
	Image TempIm;//BasculeImage(&Im,I2);
	CreerImage(&TempIm, d->width, d->height);
	Copier(d,&TempIm);
	DilatationGeod(&TempIm, r, o);
	while(Idem(r,&TempIm)!=1)
	{
		Copier(r,&TempIm);
		DilatationGeod(&TempIm, r, o);
	}
	LibererImage(&TempIm);
}
void AmelioreImage(Image *d, Image *r, int o)
{
	Image TempIm;//BasculeImage(&Im,I2);
	CreerImage(&TempIm, d->width, d->height);
	Copier(d,&TempIm);
	
	Erosion(d, &TempIm, o);
	OR(&TempIm, r, o);
	LibererImage(&TempIm);
}
void EliminationTrous(const Image *in, Image *out,int o)
{
	int i,j;
	Image ImTemp;
	CreerImage(&ImTemp,in->width,in->height);
	Complementaire(in, out);
	for (i=0,j=0; i<in->width; i++)
		if(readPix(in, i, j)==FAUX)
			writePix(out, i, j, SOUSENS);

	for (i=0,j=0; j<in->height; j++)
		if(readPix(in, i, j)==FAUX)
			writePix(out, i, j, SOUSENS);

	for (i=0,j=in->height-1; i<in->width; i++)
		if(readPix(in, i, j)==FAUX)
			writePix(out, i, j, SOUSENS);

	for (i=in->width-1,j=0; j<in->height; j++)
		if(readPix(in, i, j)==FAUX)
			writePix(out, i, j, SOUSENS);


	OR(out,&ImTemp,o);

	for(i=0;i<in->width;i++)
	{
		for(j=0;j<in->height; j++)
		{
			if(readPix(&ImTemp,i,j)==SOUSENS)
				writePix(out,i,j,FAUX);
			else
				writePix(out,i,j,VRAI);

		}
	}
	LibererImage(&ImTemp);
} 
//---------------------------------
//---------------TP4--------------
//-------------------------------
//------------------------------
//-----------------------------
//---------------------------
void ErosionUltime(const Image *in, Image *out, int o)
{
	Erosion(in, out, o);
	int vide=0;
	int i=0;
	
	Image ImTemp;
	CreerImage(&ImTemp,in->width,in->height);
	Image ImTemp2;
	CreerImage(&ImTemp2,in->width,in->height);
	Image ImTemp3;
	CreerImage(&ImTemp3,in->width,in->height);
	Image ImTemp4;
	CreerImage(&ImTemp4,in->width,in->height);
	
	Copier(in, &ImTemp);
	initIm(out, FAUX);
	//initIm(&ImTemp3, FAUX);
	
	
	while (!vide)
	{
		Erosion(&ImTemp,&ImTemp2,o);
		//vide = Testvide(&ImTemp2)
		vide = 1;
		for(i = 0; i<ImTemp2.size; i++)
			if(ImTemp2.data[i]==VRAI)
				vide=0;
		//calque &ImTemp1 + &ImTemp2 = &ImTemp3
		initIm(&ImTemp3, FAUX);
		for(i = 0; i<ImTemp.size; i++)
		{
			if(ImTemp.data[i]==VRAI)
				ImTemp3.data[i] = VRAI;
			if(ImTemp2.data[i]==VRAI)
			{
				ImTemp3.data[i] = SOUSENS;
				//ImTemp2.data[i] = FAUX;
			}
		}
		OR(&ImTemp3, &ImTemp4, o);
		//parcour temp4
			//si temp4(i) = Vrai
				//out(i) = Vrai
		for(i = 0; i<ImTemp4.size; i++)
			if(ImTemp4.data[i]==VRAI)
				out->data[i] = VRAI;
		
		//copier (temp2, temp1)
		Copier(&ImTemp2, &ImTemp);
		printf("%d \n", vide);
	}
	LibererImage(&ImTemp);
	LibererImage(&ImTemp2);
	LibererImage(&ImTemp3);
	LibererImage(&ImTemp4);
}
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------

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
		case 23 : DilatationGeod (&Im, &Im2, ordre);//DILATATION GEODESIQUE
			BasculeImage(&Im2,I2);
			glutSetWindow(f2);
			glutPostRedisplay();
		break;
		case 24 : OR (&Im, &Im2, ordre);//OUVERTURE RECONSTRUCTION
			BasculeImage(&Im2,I2);
			glutSetWindow(f2);
			glutPostRedisplay();
		break;
		case 25 : AmelioreImage (&Im, &Im2, ordre);//AMELIORE IMAGE
			BasculeImage(&Im2,I2);
			glutSetWindow(f2);
			glutPostRedisplay();
		break;
		case 26 : EliminationTrous (&Im, &Im2, ordre);//ELIMINATION TROUS
			BasculeImage(&Im2,I2);
			glutSetWindow(f2);
			glutPostRedisplay();
		break;
		case 27 : ErosionUltime(&Im, &Im2, ordre);//ERODES ULTIME
		BasculeImage(&Im2,I2);
		glutSetWindow(f2);
		glutPostRedisplay();
		break;
		case 28 : LibererImage(&Im);
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
	glutAddMenuEntry("Dilatation Geodesique",23);
	glutAddMenuEntry("OR",24);
	glutAddMenuEntry("Ameliore Image",25);
	glutAddMenuEntry("Elimination Trous",26);
	glutAddMenuEntry("Erode Ultime",27);
	glutAddMenuEntry("Quitter",28);
	
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
