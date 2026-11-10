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
#define TP_TITLE			"[TP02] Modelisation Geometrique - Courbes de Bezier"
#define TP_GROUP			""
#define TP_PROMO			"2010-2011"
void						usage();


/* Taille de la fenetre */
int winX = 800;
int winY = 600;


/* Nombre maximum de points de controle */
#define MAX_CTRL_POINTS	200

/* Structure d'une courbe de Bezier */
typedef struct _bezierCurve
{
	
	/* Degre de la courbe */
	int		degree;
	
	/* Tableau de points de controle et nombre de points inseres */
	mlVec3		controlPoints[MAX_CTRL_POINTS];
	int		nControlPoints;
	
	/* Tableau de points d'echantillonnage et nombre de points inseres */
	mlVec3 *	curvePoints;
	int		nCurvePoints;
	
	/* Nombre maximum de points d'echantillonnage de la courbe */
	double		resolution;
	
	/* Tableau contenant la construction de la courbe par De Casteljau */
	mlVec3 **	casteljauPoints;
} bezierCurve;
/* Structure du triangle de Pascal */
typedef struct _pascalTriangle
{
	
	/* Tableau de coefficients et profondeur */
	unsigned int	**	coefficients;
	unsigned int	depth;
} pascalTriangle;


/* Courbe affichee a l'ecran et triangle de Pascal */
bezierCurve	* bCurve;
pascalTriangle	 * pTriangle;

/* Options d'affichage */
int	optionDisplayPoints;
int	optionDisplayCurve;
int	optionDisplayCoords;
double	optionCasteljauStep;
int	optionDisplayCasteljau;

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
/* Fonction de calcul du triangle de Pascal */
void computePascalTriangle(pascalTriangle * _triangle)
{
	unsigned int i,j,a,b;
	_triangle->coefficients = (unsigned int **)malloc(sizeof(unsigned int*)*MAX_CTRL_POINTS);
	for(i=0;i<MAX_CTRL_POINTS;i++)
		_triangle->coefficients[i] = (unsigned int *)malloc(sizeof(unsigned int)*MAX_CTRL_POINTS);

	_triangle->coefficients[0][0]=1;
	for(i=0;i<_triangle->depth;i++)
	{
		for(j=0;j<_triangle->depth;j++)
		{
			if (i==0)
			{
				if (j==0)
					a=1;
				else
					a=0;
			}
			else
			{
				if (j==0)
					a=0;
				else
					a=_triangle->coefficients[i-1][j-1];
			}
			if (j==0)
			{
				b=0;
			}
			else
			{
				b=_triangle->coefficients[i][j-1];
			}

			_triangle->coefficients[i][j]=a+b;

		}
	}
} 
/* Fonction de calcul d'un point de la courbe de Bezier en fonction d'une valeur _t selon la methode De Casteljau */
void computeBezierDeCasteljau(bezierCurve * _curve, double _t)
{
	/******* TODO *******/
}
/* Fonction de calcul des coefficients d'une courbe de Bezier en fonction d'un indice _i, du degre _n et d'une valeur _t */
double computeBezierCoeff(int _i, int _n, double _t)
{
	int C,dim;
	double B;
	dim = _n+1;
	C = pTriangle->coefficients[_i][_n];
	B= C * pow(_t,_i)*pow((1-_t),(_n-_i));
	return B;
}
/* Fonction de calcul d'un point de la courbe de Bezier _q a partir du point d'indice i0 en fonction d'une valeur _t */
void computeBezierPoint(bezierCurve * _curve, int _i0, double _t, mlVec3 _q)
{
	int n;
	double B;
	double x,y;
	/*printf("teste CBPoint |");*/
	x = y = 0;
	for(n=0;n<= _curve->degree;n++)
	{
		B=computeBezierCoeff(n,_curve->degree,_t);
		x += B * _curve->controlPoints[_i0 + n][0];
		y += B * _curve->controlPoints[_i0 + n][1];
	}
	
	mlVec3_Set(_q, x, y, 0);
}
/* Fonction de calcul de la courbe de Bezier */
void computeBezierCurve(bezierCurve * _curve)
{
	double t;
	double pas = (double) 1.0/(double)_curve->resolution;
	mlVec3 q;
	_curve->nCurvePoints = 0; 
	for(t=0.0;t<1.0;t=t+pas)
	{
		computeBezierPoint(_curve, 0, t, q);
		
		_curve->curvePoints[_curve->nCurvePoints][0] = q[0];
		_curve->curvePoints[_curve->nCurvePoints][1] = q[1];
		_curve->curvePoints[_curve->nCurvePoints][2] = q[2];
		
		_curve->nCurvePoints++;
	}
}
/* Fonction d'affichage d'une courbe de Bezier */
void drawBezierCurve(bezierCurve * _curve)
{
	int n;
	computeBezierCurve(_curve);
	glBegin(GL_LINE_STRIP);
	glColor3f(0,0,1);
	for(n=0;n<_curve->nCurvePoints;n++)
		glVertex2d(_curve->curvePoints[n][0], _curve->curvePoints[n][1]);
	glEnd();
}
/* Callback OpenGL d'affichage */
void displayGL()
{
	int	   i, j;
	char   buffer[32];
	
	glClear(GL_COLOR_BUFFER_BIT);
	
	if(optionDisplayCurve == 1)
	{
		glColor3ub(50, 50, 50);
		if(bCurve->nControlPoints>=4)
			drawBezierCurve(bCurve);
	}
	
	if(optionDisplayCasteljau == 1 && bCurve->casteljauPoints != NULL)
	{
		glLineStipple(2, 0xAAAA);
		glEnable(GL_LINE_STIPPLE);
		
		glColor3ub(50, 180, 50);
		for(j = 0; j < bCurve->degree; ++j)
		{
			glBegin(GL_LINE_STRIP);
		
			for(i = 0; i < bCurve->degree + 1 - j; ++i)
				glVertex3dv(bCurve->casteljauPoints[j][i]);
			
			glEnd();
		}
		glDisable(GL_LINE_STIPPLE);
	}
	
	if(optionDisplayPoints == 1)
	{
		if(idPointHover != -1)
		{
			glColor3ub(220, 220, 220);
			glPointSize(pointWindowSelection * 2 + 6);
	
			glBegin(GL_POINTS);
				glVertex3dv(bCurve->controlPoints[idPointHover]);
			glEnd();
		}
	
		if(idPointInsertion != -1)
		{
			glColor3ub(220, 220, 100);
			glPointSize(pointWindowSelection * 2 + 6);
	
			glBegin(GL_POINTS);
				glVertex3dv(bCurve->controlPoints[idPointInsertion]);
			glEnd();
		}
		
		glColor3ub(175, 125, 125);
		glPointSize(pointWindowSelection * 2);
	
		glBegin(GL_POINTS);
		for(i = 0; i < bCurve->nControlPoints; ++i)
			glVertex3dv(bCurve->controlPoints[i]);
		glEnd();
	
		if(idPointSelection != -1)
		{
			glColor3ub(225, 125, 125);
			glBegin(GL_POINTS);
				glVertex3dv(bCurve->controlPoints[idPointSelection]);
			glEnd();
		}
	}
	
	if(optionDisplayCasteljau == 1 && bCurve->casteljauPoints != NULL)
	{
		glColor3ub(250, 180, 50);
		glBegin(GL_POINTS);
			glVertex3dv(bCurve->casteljauPoints[bCurve->degree][0]);
		glEnd();
	}
	
	if(optionDisplayCoords == 1)
	{
		glColor3ub(100, 100, 100);
		for(i = 0; i < bCurve->nControlPoints; ++i)
		{
			sprintf(buffer, "[%02d] (%3d, %3d)", i, (int)bCurve->controlPoints[i][0], (int)bCurve->controlPoints[i][1]);
			glPrintText((int)bCurve->controlPoints[i][0], (int)bCurve->controlPoints[i][1] + pointWindowSelection + 6, buffer);
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
			idPointSelection = idPointHover = findPoints(bCurve->controlPoints, bCurve->nControlPoints, _x, _y, pointWindowSelection);
		
			if(_state == GLUT_DOWN)
			{
				if(idPointSelection == -1 && bCurve->nControlPoints < MAX_CTRL_POINTS - 2)
				{
					if(idPointInsertion != -1)
					{
						for(i = bCurve->nControlPoints; i >= idPointInsertion; --i)
							mlVec3_Copy(bCurve->controlPoints[i], bCurve->controlPoints[i - 1]);
			
						mlVec3_Set(bCurve->controlPoints[idPointInsertion++], _x, _y, 0.0);
						idPointSelection = idPointHover = idPointInsertion - 1;
						bCurve->nControlPoints++;
					}
					else
					{
						mlVec3_Set(bCurve->controlPoints[bCurve->nControlPoints++], _x, _y, 0.0);
						idPointSelection = idPointHover = bCurve->nControlPoints - 1;
					}
				
					computeBezierCurve(bCurve);
				}
			}
			else
			{
				idPointSelection = -1;
			}
		}
	
		if(_button == GLUT_RIGHT_BUTTON)
		{
			idPointDeletion = findPoints(bCurve->controlPoints, bCurve->nControlPoints, _x, _y, pointWindowSelection);
		
			if(idPointDeletion != -1 && _state == GLUT_UP)
			{	
				for(i = idPointDeletion; i < bCurve->nControlPoints - 1; ++i)
					mlVec3_Copy(bCurve->controlPoints[i], bCurve->controlPoints[i + 1]);
			
				--bCurve->nControlPoints;
			
				if(idPointDeletion == idPointInsertion)
					idPointInsertion = -1;
			
				idPointDeletion = -1;
				computeBezierCurve(bCurve);
			}
		}
	
		if(_button == GLUT_MIDDLE_BUTTON)
		{
			if(_state == GLUT_UP)
				idPointInsertion = findPoints(bCurve->controlPoints, bCurve->nControlPoints, _x, _y, pointWindowSelection);
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
		bCurve->nControlPoints	=  0;
		bCurve->nCurvePoints	=  0;
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
	
	if(_k == 'c' || _k == 'C')
		optionDisplayCoords = !optionDisplayCoords;
	
	if(_k == '+')
	{
		/* augmentation du pas d'echantillonnage */
	
		if (bCurve->resolution<190)
			bCurve->resolution += 10;
	}
	
	if(_k == '-')
	{
		/* diminution du pas d'echantillonnage */
		
		if (bCurve->resolution>20)
			bCurve->resolution -= 10;
	}
	
	if(_k == '*')
	{
		/* augmentation du degre */
		
		if (bCurve->degree < bCurve->nControlPoints-1)
			bCurve->degree++;
	}
	
	if(_k == '/')
	{
		/* diminution du degre */
		
		if (bCurve->degree > 1)
			bCurve->degree--;
	}
	
	if(_k == 's')
	{
		/* augmentation de t dans la construction de De Casteljau */
		
		/******* TODO *******/
	}

	if(_k == 'S')
	{
		/* diminution de t dans la construction de De Casteljau */
		
		/******* TODO *******/
	}
	
	if(_k == 'd' || _k == 'D')
	{
		optionDisplayCasteljau = !optionDisplayCasteljau;
		computeBezierDeCasteljau(bCurve, optionCasteljauStep);
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
		mlVec3_Set(bCurve->controlPoints[idPointSelection], _x, _y, 0.0);
		computeBezierCurve(bCurve);
		computeBezierDeCasteljau(bCurve, optionCasteljauStep);
	}
	
	glutPostRedisplay();
}
/* Callback OpenGL de gestion de survol */
void passiveMotionGL(int _x, int _y)
{
	_y = winY - _y;
	
	idPointHover = findPoints(bCurve->controlPoints, bCurve->nControlPoints, _x, _y, pointWindowSelection);

	glutPostRedisplay();
}
/* Fonction d'initialisation des variables */
void init()
{
	bCurve			= (bezierCurve *)malloc(sizeof(bezierCurve));
	bCurve->degree			= 3;
	bCurve->nControlPoints	= 0;
	bCurve->curvePoints	= (mlVec3 *)malloc(sizeof(mlVec3) * 200);
	bCurve->nCurvePoints	= 0;
	bCurve->casteljauPoints	= NULL;
	bCurve->resolution		= 200;
	
	pTriangle		= (pascalTriangle *)malloc(sizeof(pascalTriangle));
	pTriangle->coefficients	= NULL;
	pTriangle->depth	= MAX_CTRL_POINTS;
	computePascalTriangle(pTriangle);

	optionDisplayPoints	= 1;
	optionDisplayCurve	= 1;
	optionDisplayCoords	= 0;
	optionCasteljauStep	= 0.80;
	optionDisplayCasteljau	= 0;
	
	pointWindowSelection	=  4;
	idButtonPressed		= -1;
	idPointSelection	= -1;
	idPointDeletion		= -1;
	idPointHover		= -1;
	idPointInsertion	= -1;
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
	printf("    ./tp02\n");
	printf("\n\n");
	
	printf("%c[%dm", 27, 1);
	printf("[Manuel]\n");
	printf("%c[%dm", 27, 0);
	printf("    Clic gauche   :\t ajouter/deplacer un point de controle\n");
	printf("    Clic droit    :\t supprimer un point de controle\n");
	printf("    Clic milieu   :\t definir l'indice d'insertion des points de controle\n\n");
	printf("    'e'           :\t supprimer tous les points de controle\n");
	printf("    '+'/'-'       :\t augmenter/diminuer le nombre de points d'echantillonnage\n");
	printf("    '*'/'/'       :\t augmenter/diminuer le degre de la courbe\n");
	printf("    ' '	          :\t afficher/masquer la courbe\n");
	printf("    'p'           :\t afficher/masquer les points de controle\n");
	printf("    'c'           :\t afficher/masquer les coordonnees des points de controle\n");
	printf("    's'/'S'       :\t augmenter/diminuer la valeur de calcul de De Casteljau\n");
	printf("    'd'           :\t afficher/masquer la construction selon De Casteljau\n");
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
