#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "OutilsPGM.h"
#include "basesTP.h"

#define IMG_NAME "bin1.pgm"

int iWidth = 0;
int iHeight = 0;

GLubyte* pImage = NULL;
GLubyte* pImageModif = NULL;
GLubyte* pImageModif2 = NULL;

Image stImage;
Image stImageModif;

Histo stHisto, stHistoModif,stHistoModif2;

int bNew;

void BasculeImage (Image *image, GLubyte *_pImage)
{
	int i,j;
	for (i=0 ; i < image->height; i++)
	{
		for (j=0 ; j < image->width ; j++)
		{
			_pImage[(image->height-1-i)*image->width+j]= (GLubyte)image->data[i*image->width+j] ;

		}
	}
}

void LirePGM (GLubyte **_pImage, Image* imgOut)
{
	if ( LireImage(IMG_NAME, imgOut) != 0 )
		{
		printf("Erreur LirePGM\n") ;
		fflush(stdout) ;
		return ;
		}
	*_pImage = (GLubyte*) malloc(sizeof(GLubyte) * imgOut->size) ;
	iHeight = imgOut->height ;
	iWidth = imgOut->width ;
}


void redim(int largeur, int hauteur)
{
	glViewport(0, 0, largeur, hauteur) ;
	glMatrixMode(GL_PROJECTION) ;
	glLoadIdentity() ;
	gluOrtho2D(0, largeur, 0, hauteur) ;
	glMatrixMode(GL_MODELVIEW) ;
	glLoadIdentity() ;
}

void souris (int bouton, int etat, int x, int y)
{
	switch (bouton)
	{
		case GLUT_LEFT_BUTTON:
			if (etat==GLUT_DOWN)
			{
			}
			break ;
		case GLUT_MIDDLE_BUTTON:
			if (etat==GLUT_DOWN)
			{
			}
			break;
	}
	glutPostRedisplay() ;

}

void img2glubyte(Image * imgIn, GLubyte* _pImage)
{
	int i,j;
	for (i=0 ; i < imgIn->height; i++)
	{
		for (j=0 ; j < imgIn->width ; j++)
		{
			_pImage[i * imgIn->width + j] = imgIn->data[i * imgIn->width + j];
		}
	}
}


void affichage(void)
{

	glClear (GL_COLOR_BUFFER_BIT);
	glDrawPixels(iWidth, iHeight, GL_LUMINANCE, GL_UNSIGNED_BYTE, pImage) ;
	glFlush();
}

void affichage2(void)
{

	glMatrixMode(GL_PROJECTION) ;
	glLoadIdentity() ;
	gluOrtho2D(0, 256, 0, iHeight) ;
	glMatrixMode(GL_MODELVIEW) ;
	glLoadIdentity() ;
	
	int i;
	float ratio;
	glClear (GL_COLOR_BUFFER_BIT);
	ratio =  (float)iHeight / (float)stHisto.max ;
	
	glColor3f(1.0,0.0,0.0);

	for (i = 0; i < NB_NV_GRIS; i++)
	{
		glBegin (GL_LINE_LOOP);
		glVertex2i(i,0);
		glVertex2i(i, stHisto.t[i] * ratio);
		glEnd();
	}
	glFlush();
}

void affichage3(void)
{
	glClear (GL_COLOR_BUFFER_BIT);
	glDrawPixels(iWidth, iHeight, GL_LUMINANCE, GL_UNSIGNED_BYTE, pImageModif) ;
	glFlush();
}



void affichage4(void)
{
	glClear (GL_COLOR_BUFFER_BIT);
	glDrawPixels(iWidth, iHeight, GL_LUMINANCE, GL_UNSIGNED_BYTE, pImageModif2) ;
	glFlush();
}

void affichage5(void)
{

	glMatrixMode(GL_PROJECTION) ;
	glLoadIdentity() ;
	gluOrtho2D(0, 256, 0, iHeight) ;
	glMatrixMode(GL_MODELVIEW) ;
	glLoadIdentity() ;
	
	int i;
	float ratio;
	glClear (GL_COLOR_BUFFER_BIT);
	ratio =  (float)iHeight / (float)stHistoModif.max ;

	for (i = 0; i < NB_NV_GRIS; i++)
	{
		glBegin (GL_LINE_LOOP);
		glVertex2i(i,0);
		glVertex2i(i, stHistoModif.t[i] * ratio);
		glEnd();
		}
	glFlush();
}

void affichage6(void)
{

	glMatrixMode(GL_PROJECTION) ;
	glLoadIdentity() ;
	gluOrtho2D(0, 256, 0, iHeight) ;
	glMatrixMode(GL_MODELVIEW) ;
	glLoadIdentity() ;
	
	int i;
	float ratio;
	glClear (GL_COLOR_BUFFER_BIT);
	ratio =  (float)iHeight / (float)stHistoModif2.max ;

	for (i = 0; i < NB_NV_GRIS; i++)
	{
		glBegin (GL_LINE_LOOP);
		glVertex2i(i,0);
		glVertex2i(i, stHistoModif2.t[i] * ratio);
		glEnd();
	}
	glFlush();
}

void expDynamique(Image *im1)
{
	short transfo[NB_NV_GRIS];
	double aMin, aMax, alpha, beta;
	int i, j;

	initHisto(&stHisto);
	calculHisto(im1, &stHisto);

	aMin = stHisto.t[0];
	// on parcours l'histo du d�but vers la fin, la derniere valeur non nulle est la plus haute
	for (i = 0; i < NB_NV_GRIS; i++)
	{
		if (stHisto.t[i] != 0)
		{
			aMin = i;
			break;
		}
	}

	// on parcours l'histo de la fin vers le debut,  la derniere valeur non nulle est la plus basse
	for (i = NB_NV_GRIS-1; i >= 0;  i--)
	{
		if (stHisto.t[i] != 0)
		{
			aMax = i;
			break;
		}
	}

	for (i = 0; i < im1->width; i++){
		for (j = 0; j < im1->height ; j++){
		    im1->data[i+j*im1->width] = (short)255*((im1->data[i+j*im1->width] -aMin)/(aMax - aMin));
		}
	}
}

void egaHisto(Image *im){
	
	int i,j;

	initHisto(&stHistoModif2);
	calculHistoCumul(im,&stHistoModif2);
	for (i = 0; i < im->width; i++){
		for (j = 0; j < im->height ; j++){
			im->data[i+j*im->width] = (short)(255*(stHistoModif2.t[im->data[i+j*im->width]]/(im->size)));
		}
	}	
}

void initHisto(Histo * _histo)
{
	int i;
	for (i = 0; i< NB_NV_GRIS; i++)
	{
		_histo->t[i] = 0;
	}
}


void calculHisto (Image * im1, Histo * _histo)
{
	int i,j;
	int max, classeMax;
	for (i = 0; i < im1->width; i++)
		for (j = 0; j < im1->height; j++)
		{
			_histo->t[ im1->data[j * im1->width + i] ] ++;
		}

	max = _histo->t[0];
	classeMax = 0;
	for (i = 1; i< NB_NV_GRIS; i++)
	{
		if (_histo->t[i] > max)
		{
		 	max = _histo->t[i];
			classeMax = i;
		}
	}
	_histo->max = max;
	_histo->classeMax = classeMax;
	_histo->nb_pixels = im1->size;
}
void calculHistoCumul(Image * im1, Histo * _histo){
	int i,j;
	int cumul;
	for (i = 0; i < im1->width; i++)
		for (j = 0; j < im1->height; j++)
		{
			_histo->t[ im1->data[j * im1->width + i] ] ++;
		}

	cumul = _histo->t[0];
	for (i = 1; i< NB_NV_GRIS; i++)
	{
	 	_histo->t[i] += _histo->t[i-1];
	}
	_histo->max = _histo->t[NB_NV_GRIS-1];
	_histo->classeMax = NB_NV_GRIS-1;
	_histo->nb_pixels = im1->size;
}

short valeurMiroir(Image * imgIn, int x, int y)
{
	if (x < 0)
		x = abs(x);
	else if (x > imgIn->width)
		x = imgIn->width - 1 - x;
	if (y < 0)
		y = abs(y);
	else if (y > imgIn->height)
		y = imgIn->height -1 - y;
	return imgIn->data[y*imgIn->width + x];
}

void traitement ()
{

	expDynamique(&stImage);
	
	pImageModif = (GLubyte*) malloc(sizeof(GLubyte) * stImage.size) ;
	BasculeImage (&stImage, pImageModif);
	
	initHisto(&stHistoModif);
	calculHisto(&stImage, &stHistoModif);
	
	egaHisto(&stImage);
	
	pImageModif2 = (GLubyte*) malloc(sizeof(GLubyte) * stImage.size) ;
	BasculeImage (&stImage, pImageModif2);
	
	initHisto(&stHistoModif2);
	calculHisto(&stImage, &stHistoModif2);

}

void clavier ( unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'e':
			bNew = !bNew;
            		glutPostRedisplay();
			break;
        default:
        break;
	}
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	LirePGM(&pImage, &stImage) ;
	if ( pImage == NULL ) {
		printf("Image non charg�e \n") ;
		return 0;
	}
	BasculeImage(&stImage, pImage) ;

	traitement();

	bNew = 0;


	glutInitWindowSize(iWidth, iHeight);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	glutCreateWindow("IN : Tp3 Image base");
	glutDisplayFunc(affichage);
	
	glutInitWindowSize(iWidth, iHeight);
	glutInitWindowPosition(0, iHeight+20);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	glutCreateWindow("IN : Tp3 Image modif");
	glutDisplayFunc(affichage3);
	
	glutInitWindowSize(iWidth, iHeight);
	glutInitWindowPosition(0, iHeight*2+40);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	glutCreateWindow("IN : Tp3 Image modif2");
	glutDisplayFunc(affichage4);
	
	glutInitWindowSize(256,   iHeight );
	glutInitWindowPosition(iWidth+10, 0);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	glutCreateWindow("IN : Tp3 Histo base");
	glutDisplayFunc(affichage2);
	
	glutInitWindowSize(256, iHeight);
	glutInitWindowPosition(iWidth+10, iHeight+20);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	glutCreateWindow("IN : Tp3 Histo modif");
	glutDisplayFunc(affichage5);
	
	glutInitWindowSize(256, iHeight);
	glutInitWindowPosition(iWidth+10, iHeight*2+40);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	glutCreateWindow("IN : Tp3 Histo modif2");
	glutDisplayFunc(affichage6);


	glutReshapeFunc(redim);
	glutMouseFunc(souris) ;
	glutKeyboardFunc(clavier);


	glutMainLoop();
	return 0;
}
