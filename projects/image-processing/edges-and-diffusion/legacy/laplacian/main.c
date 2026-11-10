#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "OutilsPGM.h"

#define VRAI 0
#define FAUX 255
#define NB_NV_GRIS 10000

typedef struct
{
	int taille;//Ordre
	float * coef;
}Filtre;

Filtre fl1;

typedef struct 
{
	int ClasseMax;
	int Max;
	int NbPixels;
	int t[NB_NV_GRIS];
}Hist;

Hist H;// histrogramme

Image Im, Im2, Im3, Im4, NG, Ext, Hyst;          	     //Images originales
GLubyte *I1=NULL,*I2=NULL,*I3=NULL, *I4=NULL;   //Images à afficher

int f1,f2,f3, f4;
int ordre=1;
int s=150;

int largeur,hauteur, parent,hauteurHist = 0;
int largeur_Fen_hist = 400; int hauteur_Fen_hist = 400;
float delta = 100;
int pseuil = 50;

int Sh,Sb;

Filtre fx, fy;

 int chapeauMexicain[49] = { 0, 0, -1, -1, -1, 0, 0, 
 			     0, -1, -3, -3 ,-3, -1, 0,
			     -1, -3, 0, 7, 0, -3, -1,
			     -1, -3, 7, 24, 7 , -3, -1,
			     -1, -3, 0, 7, 0, -3, -1,
			     0, -1, -3, -3 ,-3, -1, 0, 
			     0, 0, -1, -1, -1, 0, 0};
void InitFxFy()
{
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
}
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
void NormeGradient(Image *ImX, Image* ImY, Image* NG)
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
			Im2->data[i] = 0;
		else
			Im2->data[i] = 255;
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
//------------
void InitialiserHistogramme(Hist * h)
{
	int i;
	for (i = 0; i< 2048; i++)
	{
		h->t[i] = 0;
	}
}
void CalculerHistogramme(Image *in, Hist *h)
{
	int i,j;
	int max, classeMax;
	for (i = 0; i < in->width; i++)
		for (j = 0; j < in->height; j++)
			h->t[in->data[j*in->width+i]]++;
			
	max = h->t[0];
	classeMax = 0;
	for (i = 1; i< 255; i++)
		if (h->t[i] > max)
		{
		 	max = h->t[i];
			classeMax = i;
		}
	h->Max = max;
	h->ClasseMax = classeMax;
	h->NbPixels = in->size;
}
void AfficherHistImageInitial(void)
{
	int i;
	double largeur_rectangle = 2; 
	int temp =0;
	
	CalculerHistogramme(&Im, &H);
	//creer la sous fenetre dans la quelle on affiche le histo
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);// histo sera afficher loin du bord 0.0 de 10 10
	for (i=0; i < NB_NV_GRIS; i++) 
	{
		glVertex2d(i+10+temp, 10);//p1
		glVertex2d(i+10+temp, (H.t[i] * hauteur_Fen_hist / H.Max)+ 10 );//p2 
		glVertex2d(i+10+temp+largeur_rectangle, (H.t[i] * hauteur_Fen_hist / H.Max)+ 10);//p3.x = p1.x + largeur_rectangle, p3.y= p2.y
		glVertex2d(i+10+temp+largeur_rectangle, 10);//p3.x = p1.x + 10 + largeur_rectangle, p3.y= 0
		//incremente largeur_rectangle
		temp = temp + largeur_rectangle;
	}
	glEnd();
	
	glFlush();
}
//-----------SeuillageAdaptatif-------------------------
int SAdap(Image *in, float p)
{
	Hist h;
	CalculerHistogramme(in,&h);
			
	float somme=0.,pourcentage=0.;
	int i=0;
	
	while ( (i<NB_NV_GRIS) && (pourcentage<p))
	{
		somme += h.t[i];
		pourcentage = somme / ( (float) h.NbPixels)*100;
		i++;
	}
	
	return i;
}
void FiltreSobel_NG2(Image *Im, Image *NG, Filtre *fx, Filtre *fy)
{
	int seuilAdap;
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
	
	ExpansionDynamique (NG, &ImNG, 0,255);
	
	seuilAdap=SAdap(&ImNG,pseuil);
	
	seuillage(&ImNG, NG, pseuil);
	
	LibererImagePGM(&ImX);
	LibererImagePGM(&ImY);
}
//-----------Laplacien---------------------------------
void PassageZero(Image *in, Image * out, float _delta)
{
  /*
	si les points voisin sont < |delta| et un changement de signe
		le passage par zéro se trouve entre les 2 pixels
		et le changement est négligeable
	si il est > |delta|
		c'est un pixel de contour
  */
	int i,x,x1,x2,y,y1,y2;
	int m,m1, m2;
	for(y=0;y<in->height;y++)
	{
		for(x=0;x<in->width;x++)
		{
			for(i=0;i<4;i++)//Voisinage X
			{
				//Nord-Sud
				if(i==0)
				{
					x1 = x;
					y1 = y+1;
					
					x2 = x;
					y2 = y-1;
					
					m = readPix(in,x,y);
					m1 = readPix(in,x1,y1);
					m2 = readPix(in,x2,y2);
					
					if((fabs(m)>delta && 
						m1*m2<0) ||
						(fabs(m) <= delta && 
						fabs(m1)>delta && 
						fabs(m2)>delta &&
						m1 * m2 > 0))
					{
						out->data[x+y*out->width] = 255;
					}
				}
				//Est-Ouest
				if(i==1)
				{
					x1 = x-1;
					y1 = y;
					
					x2 = x+1;
					y2 = y;
					
					m = readPix(in,x,y);
					m1 = readPix(in,x1,y1);
					m2 = readPix(in,x2,y2);
					
					if((fabs(m)>delta && 
						m1*m2<0) ||
						(fabs(m) <= delta && 
						fabs(m1)>delta && 
						fabs(m2)>delta &&
						m1 * m2 > 0))
					{
						out->data[x+y*out->width] = 255;
					}
				}
				//Nord/Est-Sud/Ouest
				if(i==2)
				{
					x1 = x+1;
					y1 = y+1;
					
					x2 = x-1;
					y2 = y-1;
					
					m = readPix(in,x,y);
					m1 = readPix(in,x1,y1);
					m2 = readPix(in,x2,y2);
					
					if((fabs(m)>delta && 
						m1*m2<0) ||
						(fabs(m) <= delta && 
						fabs(m1)>delta && 
						fabs(m2)>delta &&
						m1 * m2 > 0))
					{
						out->data[x+y*out->width] = 255;
					}
				}
				//Nord/Ouest-Sud/Est
				if(i==3)
				{
					x1 = x-1;
					y1 = y+1;
					
					x2 = x+1;
					y2 = y-1;
					
					m = readPix(in,x,y);
					m1 = readPix(in,x1,y1);
					m2 = readPix(in,x2,y2);
					
					if((fabs(m)>delta && 
						m1*m2<0) ||
						(fabs(m) <= delta && 
						fabs(m1)>delta && 
						fabs(m2)>delta &&
						m1 * m2 > 0))
					{
						out->data[x+y*out->width] = 255;
					}
				}
			}
		}
	}
}
void Laplacien(Image *in, Image * out, int _delta)
{
	Image Laplacien;
	CreerImagePGM(&Laplacien, in->width, in->height);
	Image affichLaplacien;
	CreerImagePGM(&affichLaplacien, in->width, in->height);
	Image affichZero;
	CreerImagePGM(&affichZero, in->width, in->height);
	
	FiltreLineaire(in,&Laplacien,&fl1);
	//PassageZero(&Laplacien,out,delta);
        PassageZero(&Laplacien,&affichZero,delta);
        ExpansionDynamique(&affichZero, out, 0, 255);
	ExpansionDynamique(&Laplacien, &affichLaplacien, 0, 255);
        BasculeImage(&affichLaplacien,I4);
	glutSetWindow(f4);
	glutPostRedisplay();
}
//-----------Hystérésis---------------------------------
//1-
//Coordonnées estimées du vesteur Gradient en tout point
//des 4 directions : N->S, E->O, NE->SO, NO->SE
//Détermination des extrema locaux (3x3) de NG dans la direction du Gradient => l'image des Points Candidats
void Extremum(Image *res, Filtre *fx, Filtre *fy)
{
	int y, x;
	int decalx, decaly;
	double dx, dy, k, ux, uy, dx1, dx2, dy1, dy2, k1, k2;
	double vd = sin(M_PI/8);
	
	Image ImX;
	CreerImagePGM(&ImX, res->width, res->height);
	Copier(res,&ImX);
	Image ImY;
	CreerImagePGM(&ImY, res->width, res->height);
	Copier(res,&ImY);
	Image ImNG;
	CreerImagePGM(&ImNG, res->width, res->height);
	
	FiltreLineaire(res, &ImX, fx);
	FiltreLineaire(res, &ImY, fy);
	
	for (y = 0; y < Im.height; y++)
		for (x = 0; x < Im.width ; x++)
		{
			decalx = 0;
			decaly = 0;
			dx = readPix(&ImX,x,y);
			dy = readPix(&ImY,x,y);
			k = readPix(&NG,x,y);
			//Normalisation du vecteur gradient
			ux = dx / k ;
			uy = dy / k;

			if(ux< -vd)
				decalx = -1;
			else if(ux > vd) 
				decalx = 1;
			if(uy< -vd)
				decaly = -1;
			else if(uy > vd) 
				decaly = 1;
				
			dx1 = readPix(&ImX,x+decalx,y+decaly);
			dx2 = readPix(&ImX,x-decalx,y-decaly);
			dy1 = readPix(&ImX,x+decalx,y+decaly);
			dy2 = readPix(&ImX,x-decalx,y-decaly);
			k1 = ux*dx1 + uy*dy1;
			k2 = ux*dx2 + uy*dy2;
			
			if( abs(k) >= abs(k1) && abs(k) > abs(k2))
				writePix(res, x, y, k);
		}    
}
//2-
//Définir au clavier Sb et Sh tel que Sb<Sh (Seuil Bas et Seuil Haut)
//Construire l'image binaire des contours retenus avec méthode de propagation par Hystérésis à partir des points candidats et des seuils Sb et Sh
void Propag(int x,int y,int sb)
{
	int k, l;
	if(x<0 || y<0 || readPix(&Ext,x,y) < sb || readPix(&Hyst,x,y)!= 0 ) 
	  return;
	else
		writePix(&Hyst, x, y, 255);
	for(k = y-1; k <= y+1; k++)
		for(l = x-1; l <= x+1; l++)
			Propag(k,l,sb);
}
void Hysteresis(int sh,int sb)
{
	int y, x;
	for (y = 0; y < Im.height; y++)
		for (x = 0; x < Im.width ; x++)
			writePix(&Hyst, x, y, 0);
	
	for (y = 0; y < Im.height; y++)
		for (x = 0; x < Im.width ; x++)
			if(readPix(&Ext,x,y) > sh)
				Propag(x,y,sb);
}
//-----------------------------------------------------
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
			s=s-5;
			
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
		case 'd':
		case 'D':
		{
			s=s+5;
			
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
		case 'a':
		case 'A':
		{
			if(pseuil > 0)
			  pseuil=pseuil-5;
			
			InitFxFy();
			FiltreSobel_NG2(&Im, &Im2, &fx, &fy);
			BasculeImage(&Im2, I2);
			glutSetWindow(f2);
			glutPostRedisplay();
		}
		break;
		case 'e':
		case 'E':
		{
			if(pseuil < 100)
			  pseuil=pseuil+5;
			
			InitFxFy();
			FiltreSobel_NG2(&Im, &Im2, &fx, &fy);
			BasculeImage(&Im2, I2);
			glutSetWindow(f2);
			glutPostRedisplay();
		}
		break;
		case 'w':
		case 'W':
		{
			if(delta > 0)
			  delta=delta-10;
			
			Laplacien(&Im, &Im3, delta);
			BasculeImage(&Im3, I3);
			glutSetWindow(f3);
			glutPostRedisplay();
		}
		break;
		case 'c':
		case 'C':
		{
			if(delta < 2000)
			  delta=delta+10;
			
			Laplacien(&Im, &Im3, delta);
			BasculeImage(&Im3, I3);
			glutSetWindow(f3);
			glutPostRedisplay();
		}
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
		{
			BasculeImage(&Im,I2);
			glutSetWindow(f2);
			glutPostRedisplay();
			BasculeImage(&Im,I3);
			glutSetWindow(f3);
			glutPostRedisplay();
			BasculeImage(&Im,I4);
			glutSetWindow(f4);
			glutPostRedisplay();
		}
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
			InitFxFy();
			FiltreSobel(&Im, &Im2, &Im3, &fx, &fy);
			BasculeImage(&Im2,I2);
			glutSetWindow(f2);
			glutPostRedisplay();
			BasculeImage(&Im3,I3);
			glutSetWindow(f3);
			glutPostRedisplay();
		}
		break;
		case 9 :
		{
			InitFxFy();
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
		case 10 :
		{
			InitFxFy();
			FiltreSobel_NG2(&Im, &Im2, &fx, &fy);//-OK
			BasculeImage(&Im2, I2);
			glutSetWindow(f2);
			glutPostRedisplay();
		}
		break;
		case 11 ://LAPLACIEN
		{
			Laplacien(&Im, &Im3, delta);
			BasculeImage(&Im3, I3);
			glutSetWindow(f3);
			glutPostRedisplay();
		}
		break;
		case 12 ://HYSTERESIS
		{
			printf("Donnez le Seuil Bas!\n");
			scanf("%d",&Sb);
			printf("Donnez le Seuil Haut!\n");
			scanf("%d",&Sh);
			//On echange Sb et Sh si Sb > Sh
			if(Sb>Sh)
			{
			  Sb = Sb+Sh;
			  Sh = Sb-Sh;
			  Sb = Sb-Sh;
			}
			InitFxFy();
			printf("test1!\n");
			FiltreSobel_NG2(&Im, &NG, &fx, &fy);
			printf("test2!\n");
			Extremum(&Ext, &fx, &fy);
			printf("test3!\n");
			//Hysteresis(Sb,Sh);
			printf("test4!\n");
			
			ExpansionDynamique(&Ext, &Im3, 0, 255 );
			printf("test5!\n");
			//f2=glutCreateWindow("Fenetre 2 : Norme du Gradient");
			BasculeImage(&NG, I2);
			glutSetWindow(f2);
			//f3=glutCreateWindow("Fenetre 3 : Extrema");
			BasculeImage(&Im3, I3);
			glutSetWindow(f3);
			//f4=glutCreateWindow("Fenetre 4 : Hysteresis");
			//BasculeImage(&Hyst, I4);
			//glutSetWindow(f4);
			glutPostRedisplay();
		}
		break;
		case 13 : 
		{
			LibererImagePGM(&Im);
			LibererImagePGM(&Im2);
			LibererImagePGM(&Im3);
			LibererImagePGM(&Im4);
			free(I1);
			free(I2);
			free(I3);
			free(I4);
			exit(0); /* On quitte */
		}
		break;
	}

}
void CreerMenu(void)
{
	
	glutCreateMenu(ChoixMenuPrincipal);
	glutAddMenuEntry("Bascule Image",6);
	glutAddMenuEntry("Filtre Lineaire",7);
	glutAddMenuEntry("Filtre Sobel",8);
	glutAddMenuEntry("NG",9);
	glutAddMenuEntry("Seuillage Adaptatif",10);
	glutAddMenuEntry("Laplacien",11);
	glutAddMenuEntry("Hysteresis",12);
	glutAddMenuEntry("Quitter",13);
	
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
int main(int argc,char **argv)
{
	int i;
	
	glutInit(&argc,argv);
	
	fl1.coef = (float*) malloc (49*sizeof(float));
	fl1.taille = 7;
	for(i=0; i<49; ++i)
		fl1.coef[i] = chapeauMexicain[i];
	
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
	
	CreerImagePGM(&NG, Im.width, Im.height);
	CreerImagePGM(&Ext, Im.width, Im.height);
	CreerImagePGM(&Hyst, Im.width, Im.height);
    
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
	f2=glutCreateWindow("Fenetre 2 : Seuillage Adaptatif");
	glutDisplayFunc(affichage2);//---------------------------------------------------------------
	glutReshapeFunc(redim);
	initGL();
	/*----------------------------------------------------*/
	glutInitWindowSize(Im3.width,Im3.height);
	glutInitWindowPosition(000,800);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	f3=glutCreateWindow("Fenetre 3 : Laplacien");
	glutDisplayFunc(affichage3);
	glutReshapeFunc(redim);
	initGL();
	/*----------------------------------------------------*/
	glutInitWindowSize(Im4.width,Im4.height);
	glutInitWindowPosition(800,800);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	f4=glutCreateWindow("Fenetre 4 : Hysteresis");
	glutDisplayFunc(affichage4);
	glutReshapeFunc(redim);
	initGL();
	
	glutMainLoop();
	return 0;
} 
