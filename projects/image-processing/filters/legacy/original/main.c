#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "OutilsPGM.h"

#define VRAI 0
#define FAUX 255
#define NBNIVGRIS 255

Image Im,Im2;          	     //Images originales
GLubyte *I1=NULL,*I2=NULL;   //Images à afficher

int f1,f2;
int ordre=1;

typedef struct
{
	int taille;//Ordre
	float * coef;
}Filtre_Lineaire;

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
//Filtre Linéaire
int ValMiroir(Image * I, int x, int y)
{
	if(x<0)
		x= -x-1;
	else if (x>=I->width)
		x = 2 * I->width - x - 1;
	if(y<0)
		y= -y-1;
	else if (y>=I->height)
		y = 2 * I->height - y - 1;
	return (I->data[y * I->width + x]);
}
void FiltreLineaire(Image *I, Image *F, Filtre_Lineaire *fl)
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
//Filtre Médian
void FiltreMedian (Image *I, Image *F, int cote)
{
	//Ce filtre ne fonctionne que pour des valeurs de coté impaires
	int b,m, *v, e,l,c,x,y,k;
	
	v = malloc (sizeof(int) * cote * cote);
	b = cote /2;
	m = (cote * cote)/2;
	for(y=0;y<I->height;y++)
		for(x=0;x<I->width;x++)
		{
			m=0;
			for(l=-b;l<=b;l++)
			{
				for(c=-b;c<=b;c++)
				{
					e=ValMiroir(I, x+c, y+l);
					k=m;
					while((k>=0)&&(e<v[k]))
					{
						v[k+1]=v[k];
						k--;
					}
					v[k+1]=e;
					m++;
				}
				F->data[y*I->width+x]=v[m];
			}
		}	
}
//Filtre Nagao
void FiltreNagao(Image* in, Image* out, int cote)
{
	int i, j, k, l;
        int px[9][9] = 
        {
	  {-2,-1,0,-2,-1,0,-2,-1,0},
	  {0,1,2,0,1,2,0,1,2},
	  {-2,-1,0,-2,-1,0,-2,-1,0},
	  {0,1,2,0,1,2,0,1,2},
	  {-1,0,1,-1,0,1,-1,0,1},
	  {-2,-1,0,1,2,-1,0,1,0},
	  {-2,-2,-1,-2,-1,0,-2,-1,-2},
	  {0,-1,0,1,-2,-1,0,1,2},
	  {2,1,2,0,1,2,1,2,2}
        };
        int py[9][9] = 
        {
	  {-2,-2,-2,-1,-1,-1,0,0,0},
	  {-2,-2,-2,-1,-1,-1,0,0,0},
	  {0,0,0,1,1,1,2,2,2},
	  {0,0,0,1,1,1,2,2,2},
	  {-1,-1,-1,0,0,0,1,1,1},
	  {-2,-2,-2,-2,-2,-1,-1,-1,0},
	  {-2,-1,-1,0,0,0,1,1,2},
	  {0,1,1,1,2,2,2,2,2},
	  {-2,-1,-1,0,0,0,1,1,2}
        };

        for(i=0;i<in->width;i++)
	{
                for(j=0;j<in->height;j++)
		{
                        int varMin=0,indiceVarMin=0;
                        for(k = 0; k<9;k++)
			{
                                int moyenne = 0;
                                for(l = 0; l<9;l++)
				{
					moyenne += ValMiroir(in,i+px[k][l],j+py[k][l]);
                                }
                                moyenne /=9;
                                int variance = 0;
                                for(l = 0; l<9;l++)
				{
					variance += (moyenne - ValMiroir(in,i+px[k][l],j+py[k][l]))*(moyenne - 			ValMiroir(in,i+px[k][l],j+py[k][l]));
                                }
                                if(k==0) varMin = variance;
                                if(variance<varMin)
				{
                                        varMin = variance;
                                        indiceVarMin = k;
                                }
                        }
                        
                        int *v = malloc (sizeof(int) * cote * cote);
                        for(l = 0; l<9;l++)
			{
				v[l]=ValMiroir (in, i+px[indiceVarMin][l], j+py[indiceVarMin][l]);
                        }
                        /*int beg=0, end=9;
                        if (end > beg + 1)
			{
			  int piv = v[beg], l = beg + 1, r = end;
			  while (l < r)
			  {
			    if (v[l] <= piv)
			      l++;
			    else
				    swap(v[l], v[--r]);
			  }
			  swap(v[--l], v[beg]);
			  sort(v, beg, l);
			  sort(v, r, end);
		}*/
			  
                        //std::sort(v.begin(),v.end());
                        out->data[i+j*in->width] = v[4];
                }
                
        }
        
                
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
	glClear(GL_COLOR_BUFFER_BIT);
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

void ChoixMenuPrincipal(int value)
{
	switch (value)
	{
		case 6 : 
			BasculeImage(&Im,I2);
			glutSetWindow(f2);
			glutPostRedisplay();
		break;
		case 7 : 
		{
			Filtre_Lineaire fl;
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
			FiltreMedian(&Im, &Im2, 3);
			BasculeImage(&Im2,I2);
			glutSetWindow(f2);
			glutPostRedisplay();
		}
		break;
		case 9 : 
		{
			FiltreNagao(&Im, &Im2, 3);
			BasculeImage(&Im2,I2);
			glutSetWindow(f2);
			glutPostRedisplay();
		}
		break;
		case 10 : LibererImage(&Im);
			LibererImage(&Im2);
			free(I1);
			free(I2);
			exit(0); /* On quitte */
			break;
	} 
}


void CreerMenu(void)
{
	
	glutCreateMenu(ChoixMenuPrincipal);
	glutAddMenuEntry("Bascule Image",6);
	glutAddMenuEntry("Filtre Lineaire",7);
	glutAddMenuEntry("Filtre Median",8);
	glutAddMenuEntry("Filtre Nagao",9);
	glutAddMenuEntry("Quitter",10);
	
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
	glutReshapeFunc(redim);
	CreerMenu();
	initGL();
	
	/*----------------------------------------------------*/
	glutInitWindowSize(Im2.width,Im2.height);
	glutInitWindowPosition(800,100);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	f2=glutCreateWindow("Fenetre 2");
	glutDisplayFunc(affichage2);
	glutReshapeFunc(redim);
	initGL();
	
	glutMainLoop();
	return 0;
} 
