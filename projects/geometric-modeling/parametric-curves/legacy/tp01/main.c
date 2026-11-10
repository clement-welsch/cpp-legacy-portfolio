/* Includes OpenGL */
#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	#include <GLUT/glut.h>
#else
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <GL/glut.h>
#endif


/* Includes standards */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>



/* Include de la bibliotheque d'algebre */
#include <ml.h>


/* Defines d'informations */
#define TP_TITLE			"[TP01] Modelisation Geometrique - Courbes hermitiennes cubiques"
#define TP_GROUP			""
#define TP_PROMO			"2010-2011"
void usage();


/* Taille de la fenetre */
int winX = 800;
int winY = 600;


/* Nombre maximum de points de controle */
#define MAX_CTRL_POINTS	200


/* Structure d'une courbe hermitienne */
typedef struct _hermiteCurve 
{
	
	/* Tableau de points de controle et nombre de points inseres */
	mlVec3		controlPoints[MAX_CTRL_POINTS];
	int			nControlPoints;
	
	/* Tableau de points d'echantillonnage et nombre de points inseres */
	mlVec3 *	curvePoints;
	int			nCurvePoints;
	
	/* Resolution d'echantillonnage de la courbe */
	int			resolution;
} hermiteCurve;


/* Courbe affichee a l'ecran */
hermiteCurve * hCurve;


/* Options d'affichage */
int		optionDisplayPoints;
int		optionDisplayCurve;
int		optionDisplayVectors;
int		optionDisplayCoords;


/* Variables pour l'edition de la courbe */
int		pointWindowSelection;
int		idButtonPressed;
int		idPointSelection;
int		idPointDeletion;
int		idPointHover;
int		idPointInsertion;


/* Fonction pour afficher du texte en OpenGL */
void glPrintText(int _x, int _y, const char * _text)
{
	int i;
	
	glRasterPos2i(_x, _y);
	
	for(i = 0; i < (int)strlen(_text); ++i)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, _text[i]);
}
/* Fonction pour rechercher un point aux coordonnees (x +/- p, y +/- p) */
int findPoints(mlVec3 * _points, int _nPoints, int _x, int _y, int _p)
{
	int i;
	
	for(i = 0; i < _nPoints; ++i)
	{
		if(fabs(_points[i][0] - _x) < _p && fabs(_points[i][1] - _y) < _p)
			return i;
	}
	
	return -1;
}
/* Fonction de calcul des coefficients _cx, _cy, _cz d'Hermite */
void computeHermiteCoeff(mlVec3 _p1, mlVec3 _r1, mlVec3 _p4, mlVec3 _r4, mlVec4 _cx, mlVec4 _cy, mlVec4 _cz)
{
	/* C = G x M */	
	mlMat4 M = {2,-3,0,1,-2,3,0,0,1,-2,1,0,1,-1,0,0};
	
	mlVec4 Ghx, Ghy, Ghz;
	
	mlVec4_Set(Ghx, _p1[0], _p4[0], _r1[0], _r4[0]);
	mlVec4_Set(Ghy, _p1[1], _p4[1], _r1[1], _r4[1]);
	mlVec4_Set(Ghz, _p1[2], _p4[2], _r1[2], _r4[2]);
	
	mlVec4_MultMat(_cx, Ghx, M);
	mlVec4_MultMat(_cy, Ghy, M);
	mlVec4_MultMat(_cz, Ghz, M);
} 
/* Fonction de calcul d'un point de la courbe d'Hermite _q en fonction des coefficients _cx, _cy, _cz et d'une valeur _t */
void computeHermitePoint (mlVec4 cx , mlVec4 cy , mlVec4 cz , double t , mlVec3 q)
{
	/*Q(t) = C · T*/
	/*------------*/
	double t2 = t * t;
	double t3 = t2 * t;
	
	mlVec4 T;
	mlMat4 C;
	/*C*/
	
	mlMat4_Set(C,cx[0],cx[1],cx[2],cx[3],cy[0],cy[1],cy[2],cy[3],cz[0],cz[1],cz[2],cz[3],1,1,1,1);

	/*T*/
	mlVec4_Set(T, t3, t2, t, 1);

	/*Q*/
	/*mlMat4_MultMat(q, C, T);
	mlMat4_Print(q);*/
	
	q[0] = mlVec4_Dot(cx, T);
	q[1] = mlVec4_Dot(cy, T);
	q[2] = mlVec4_Dot(cz, T);
}
/* Fonction de calcul de la courbe d'Hermite */
void computeHermiteCurve(hermiteCurve * _curve)
{
	int ctrl;
	double t;
	double pas = 1.0/_curve->resolution;
	double x1, y1, x2, y2;
	mlVec3 q;
	mlVec3 P1, R1, P4, R4;
	mlVec4 cx;
	mlVec4 cy;
	mlVec4 cz;
	
	_curve->nCurvePoints = 0;
	for (ctrl = 0; ctrl+3 < _curve->nControlPoints;ctrl ++)
	  for(t=0.0;t<1.0;t=t+pas)
	  {
		  /* Calcule de P1, P4, R1 et R4*/
		  /*P1*/
		  x1 = _curve->controlPoints[ctrl][0];
		  y1 = _curve->controlPoints[ctrl][1];
		  mlVec3_Set(P1, x1, y1, 0);
		  /*R1*/
		  
		  x2 = _curve->controlPoints[ctrl+1][0] - x1;
		  y2 = _curve->controlPoints[ctrl+1][1] - y1;
		  mlVec3_Set(R1, x2, y2, 0);
		  /*P4*/
		  x1 = _curve->controlPoints[ctrl+2][0];
		  y1 = _curve->controlPoints[ctrl+2][1];
		  mlVec3_Set(P4, x1, y1, 0);
		  /*R4*/
		  x2 = _curve->controlPoints[ctrl+3][0] - x1;
		  y2 = _curve->controlPoints[ctrl+3][1] - y1;
		  mlVec3_Set(R4, x2, y2, 0);
		  
		  computeHermiteCoeff(P1,R1,P4,R4,cx,cy,cz);
		  computeHermitePoint(cx , cy , cz , t , q);
		  
		  _curve->curvePoints[_curve->nCurvePoints][0] = q[0];
		  _curve->curvePoints[_curve->nCurvePoints][1] = q[1];
		  _curve->curvePoints[_curve->nCurvePoints][2] = q[2];
		  
		  _curve->nCurvePoints++;
	  }
}
/* Fonction d'affichage d'une courbe d'Hermite */
void drawHermiteCurve(hermiteCurve * _curve)
{
	int n;
	computeHermiteCurve(_curve);
	
	glBegin(GL_LINE_STRIP);
		glColor3f(0,0,1);
		for(n=0;n<_curve->nCurvePoints;n++)
		  glVertex2d(_curve->curvePoints[n][0], _curve->curvePoints[n][1]);
	glEnd();
}
/* Callback OpenGL d'affichage */
void displayGL()
{
	int	   i;
	mlVec3 v;
	char   buffer[32];
	
	glClear(GL_COLOR_BUFFER_BIT);
	
	if(optionDisplayCurve == 1 && hCurve->nControlPoints==4)
	{
		glColor3ub(50, 50, 50);
		drawHermiteCurve(hCurve);
	}
	
	if(optionDisplayVectors == 1)
	{
		glColor3ub(100, 175, 100);
		
		glLineWidth(2.0);
		glBegin(GL_LINES);
		for(i = 0; i < hCurve->nControlPoints - 1; i += 2)
		{
			mlVec3_SubVec(v, hCurve->controlPoints[i + 1], hCurve->controlPoints[i]);
			mlVec3_Normalize(v, v);
			mlVec3_Scale(v, v, 100.0);
			mlVec3_AddVec(v, v, hCurve->controlPoints[i]);
			
			glVertex3dv(hCurve->controlPoints[i]);
			glVertex3dv(v);
		}
		glEnd();
		glLineWidth(1.0);
	}
	if(optionDisplayPoints == 1)
	{
		if(idPointHover != -1)
		{
			glColor3ub(220, 220, 220);
			glPointSize(pointWindowSelection * 2 + 6);
	
			glBegin(GL_POINTS);
				glVertex3dv(hCurve->controlPoints[idPointHover]);
			glEnd();
		}
	
		if(idPointInsertion != -1)
		{
			glColor3ub(220, 220, 100);
			glPointSize(pointWindowSelection * 2 + 6);
	
			glBegin(GL_POINTS);
				glVertex3dv(hCurve->controlPoints[idPointInsertion]);
			glEnd();
		}
		
		glColor3ub(175, 125, 125);
		glPointSize(pointWindowSelection * 2);
	
		glBegin(GL_POINTS);
		for(i = 0; i < hCurve->nControlPoints; ++i)
			glVertex3dv(hCurve->controlPoints[i]);
		glEnd();
	
		if(idPointSelection != -1)
		{
			glColor3ub(225, 125, 125);
			glBegin(GL_POINTS);
				glVertex3dv(hCurve->controlPoints[idPointSelection]);
			glEnd();
		}
	}
	
	if(optionDisplayCoords == 1)
	{
		glColor3ub(100, 100, 100);
		for(i = 0; i < hCurve->nControlPoints; ++i)
		{
			sprintf(buffer, "[%02d] (%3d, %3d)", i, (int)hCurve->controlPoints[i][0], 
				(int)hCurve->controlPoints[i][1]);
			glPrintText((int)hCurve->controlPoints[i][0], (int)hCurve->controlPoints[i][1] + 
					pointWindowSelection + 6, buffer);
		}
		glEnd();
	}	
	glutSwapBuffers();
}
/* Callback OpenGL de redimensionnement */
void reshapeGL(int _w, int _h)
{
	winX = _w;
	winY = _h;
	
	glViewport(0, 0, winX, winY);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluOrtho2D(0.0, winX, 0.0, winY);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glutPostRedisplay();
}
/* Callback OpenGL de gestion de souris */
void mouseGL(int _button, int _state, int _x, int _y)
{
	int i;
	
	idButtonPressed  = _state == GLUT_DOWN ? _button : -1;
	idPointSelection = -1;
	idPointHover	 = -1;
	
	_y = winY - _y;
	
	if(optionDisplayPoints == 1)
	{
		if(_button == GLUT_LEFT_BUTTON)
		{
			idPointSelection = idPointHover = findPoints(hCurve->controlPoints, hCurve->nControlPoints, _x, 
					_y, pointWindowSelection);
		
			if(_state == GLUT_DOWN)
			{
				if(idPointSelection == -1 && hCurve->nControlPoints < MAX_CTRL_POINTS - 2)
				{
					if(idPointInsertion != -1)
					{
						for(i = hCurve->nControlPoints; i >= idPointInsertion; --i)
							mlVec3_Copy(hCurve->controlPoints[i], 
									hCurve->controlPoints[i - 1]);
			
						mlVec3_Set(hCurve->controlPoints[idPointInsertion++], _x, _y, 0.0);
						idPointSelection = idPointHover = idPointInsertion - 1;
						hCurve->nControlPoints++;
					}
					else
					{
						mlVec3_Set(hCurve->controlPoints[hCurve->nControlPoints++], _x, _y, 0.0);
						idPointSelection = idPointHover = hCurve->nControlPoints - 1;
					}
				
					/*computeHermiteCurve(hCurve);*/
				}
			}
			else
			{
				idPointSelection = -1;
			}
		}
	
		if(_button == GLUT_RIGHT_BUTTON)
		{
			idPointDeletion = findPoints(hCurve->controlPoints, hCurve->nControlPoints, _x, _y, 
					pointWindowSelection);
		
			if(idPointDeletion != -1 && _state == GLUT_UP)
			{	
				for(i = idPointDeletion; i < hCurve->nControlPoints - 1; ++i)
					mlVec3_Copy(hCurve->controlPoints[i], hCurve->controlPoints[i + 1]);
			
				--hCurve->nControlPoints;
			
				if(idPointDeletion == idPointInsertion)
					idPointInsertion = -1;
			
				idPointDeletion = -1;
				computeHermiteCurve(hCurve);
			}
		}
	
		if(_button == GLUT_MIDDLE_BUTTON)
		{
			if(_state == GLUT_UP)
				idPointInsertion = findPoints(hCurve->controlPoints, hCurve->nControlPoints, _x, _y, 
					pointWindowSelection);
		}
	}
	glutPostRedisplay();
}
/* Callback OpenGL de gestion de clavier */
void keyboardGL(unsigned char _k, int _x, int _y)
{	
	if(_k == 27 || _k == 'q' || _k == 'Q')
		exit(0);
	
	if(_k == 'h' || _k == 'H')
		usage();
	
	if(_k == 'e' || _k == 'e')
	{
		hCurve->nControlPoints	=  0;
		hCurve->nCurvePoints	=  0;
		idButtonPressed			= -1;
		idPointSelection		= -1;
		idPointDeletion			= -1;
		idPointHover			= -1;
		idPointInsertion		= -1;
	}
	if(_k == ' ')
		optionDisplayCurve = !optionDisplayCurve;
	
	if(_k == 'p' || _k == 'P')
		optionDisplayPoints = !optionDisplayPoints;
	
	if(_k == 'v' || _k == 'V')
		optionDisplayVectors = !optionDisplayVectors;
	
	if(_k == 'c' || _k == 'C')
		optionDisplayCoords = !optionDisplayCoords;
	
	if(_k == '+')
	{
		/* augmentation du pas d'echantillonnage */
		if (hCurve->resolution<190)
			hCurve->resolution+=10;
	}
	
	if(_k == '-')
	{
		/* diminution du pas d'echantillonnage */
		if (hCurve->resolution>20)
			hCurve->resolution-=10;
	}

	glutPostRedisplay();
}
/* Callback OpenGL de gestion des touches speciales de claviers */
void keyboardSpecialGL(int _k, int _x, int _y)
{
	glutPostRedisplay();
}
/* Callback OpenGL de gestion de drag */
void motionGL(int _x, int _y)
{
	_y = winY - _y;
	
	if(idButtonPressed == GLUT_LEFT_BUTTON && idPointSelection != -1)
	{
		mlVec3_Set(hCurve->controlPoints[idPointSelection], _x, _y, 0.0);
		computeHermiteCurve(hCurve);
	}
	
	glutPostRedisplay();
}
/* Callback OpenGL de gestion de survol */
void passiveMotionGL(int _x, int _y)
{
	_y = winY - _y;
	
	idPointHover = findPoints(hCurve->controlPoints, hCurve->nControlPoints, _x, _y, pointWindowSelection);

	glutPostRedisplay();
}
/* Fonction d'initialisation des variables */
void init()
{
	hCurve					= (hermiteCurve *)malloc(sizeof(hermiteCurve));
	hCurve->nControlPoints	= 0;
	hCurve->curvePoints		= (mlVec3 *)malloc(sizeof(mlVec3) * 200);
	hCurve->nCurvePoints	= 0;
	hCurve->resolution		= 200;
	
	optionDisplayPoints		= 1;
	optionDisplayCurve		= 1;
	optionDisplayVectors	= 0;
	optionDisplayCoords		= 0;
	
	pointWindowSelection	=  4;
	idButtonPressed			= -1;
	idPointSelection		= -1;
	idPointDeletion			= -1;
	idPointHover			= -1;
	idPointInsertion		= -1;
}
/* Fonction d'initialisation OpenGL */
void initGL()
{
	glClearColor(0.8, 0.8, 0.7, 1.0);
}
/* Fonction d'affichage du manuel */
void usage()
{
	printf("---------------------------------------------------------------------------\n");
	printf("%c[%dm", 27, 1);
	printf("%s\n", TP_TITLE);
	printf("%c[%dm", 27, 0);
	printf("%s (%s)\n", TP_GROUP, TP_PROMO);
	printf("\n\n");
	
	printf("%c[%dm", 27, 1);
	printf("[Utilisation]\n");
	printf("%c[%dm", 27, 0);
	printf("    ./tp01\n");
	printf("\n\n");
	
	printf("%c[%dm", 27, 1);
	printf("[Manuel]\n");
	printf("%c[%dm", 27, 0);
	printf("    Clic gauche   :\t ajouter/deplacer un point de controle\n");
	printf("    Clic droit    :\t supprimer un point de controle\n");
	printf("    Clic milieu   :\t definir l'indice d'insertion des points de controle\n\n");
	printf("    'e'           :\t supprimer tous les points de controle\n");
	printf("    '+'/'-'       :\t augmenter/diminuer le nombre de points d'echantillonnage\n");
	printf("    ' '	          :\t afficher/masquer la courbe\n");
	printf("    'p'           :\t afficher/masquer les points de controle\n");
	printf("    'v'           :\t afficher/masquer les vecteurs tangents\n");
	printf("    'c'           :\t afficher/masquer les coordonnees des points de controle\n");
	printf("    'h'           :\t afficher cette aide\n");
	printf("    'q'/Esc       :\t quitter\n");
	printf("---------------------------------------------------------------------------\n");
}
/* Fonction principale */
int main(int _argc, char ** _argv)
{
	int posX, posY;
	
	glutInit(&_argc, _argv);
	usage();
	
	posX = (glutGet(GLUT_SCREEN_WIDTH ) - winX) / 2;
	posY = (glutGet(GLUT_SCREEN_HEIGHT) - winY) / 2;
	
	glutInitWindowSize(winX, winY);
	glutInitWindowPosition(posX, posY);
	
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow(TP_TITLE);
	
	glutDisplayFunc(displayGL);
	glutReshapeFunc(reshapeGL);
	
	glutMouseFunc(mouseGL);
	glutKeyboardFunc(keyboardGL);
	glutSpecialFunc(keyboardSpecialGL);
	
	glutMotionFunc(motionGL);
	glutPassiveMotionFunc(passiveMotionGL);
	
	init();
	initGL();
	glutMainLoop();

	return 0;
}
