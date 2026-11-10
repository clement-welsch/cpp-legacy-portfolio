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
#define TP_TITLE			"[TP05] Modelisation Geometrique - Surfaces a poles"
#define TP_GROUP			""
#define TP_PROMO			"2010-2011"
void						usage();


/* Taille de la fenetre */
int winX = 800;
int winY = 600;


/* Nombre maximum de points de controle */
#define MAX_CTRL_POINTS	200


/* Structure d'une surface de Bezier */
typedef struct _xSurface {
	
	/* Degre de la surface */
	int			uDegree;
	int			vDegree;
	
	/* Tableau de points de controle et nombre de points inseres */
	mlVec3		controlPoints[MAX_CTRL_POINTS][MAX_CTRL_POINTS];
	int			nuControlPoints;
	int			nvControlPoints;
	
	/* Resolution d'echantillonnage de la surface */
	int			resolution;
} xSurface;

/* Structure du triangle de Pascal */
typedef struct _pascalTriangle {
	
	/* Tableau de coefficients et profondeur */
	int	**		coefficients;
	int			depth;
} pascalTriangle;

/* Surface affichee a l'ecran */
xSurface		 * pSurface;
pascalTriangle	 * pTriangle;

/* Structure d'une camera */
typedef struct _glCamera 
{

	/* Position, point de vue et vecteur pointant vers le haut */
	mlVec3		position;
	mlVec3		center;
	mlVec3		up;
	
} glCamera;


/* Variables globales (camera, position du triangle, ...) */
glCamera *	camera;
mlVec3		trianglePos;
int			mouseLeftDown;
int			idCubeTexture;


/* Options d'affichage */
int	optionRenderingMode;
int	optionRenderingLight;
int	optionDisplayVertex;
int	optionUniqueColor;
int optionShadeModel;
int optionProjection;
int optionScene;

#define TAILLE_MAX 1000

/* Options d'affichage */
int		mouseLeftDown;
int		optionDisplayPoints;
int		optionDisplaySurface;
int		optionComputeBezier;

/* Variable Perso */
double camX=3.0, camY=0.0, camZ=-10.0;

void Repere()
{
	glBegin(GL_LINES);
		/*X*/
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, 0.0);		
		glVertex3f(10.0, 0.0, 0.0);
		/*Y*/
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 10.0, 0.0);
		/*Z*/
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 10.0);
	glEnd();
}
/* Fonction de calcul du triangle de Pascal */
void computePascalTriangle(pascalTriangle * _triangle)
{
	int i,j,a,b;
	_triangle->coefficients = ( int **)malloc(sizeof(int*)*MAX_CTRL_POINTS);
	for(i=0;i<MAX_CTRL_POINTS;i++)
		_triangle->coefficients[i] = (int *)malloc(sizeof(int)*MAX_CTRL_POINTS);

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
/* Fonction de calcul d'une factorielle */
long double fact(long double _n)
{
	return (_n < 2 ? 1 : _n * fact(_n - 1));
}
/* --------------------------------- Bézier ------------------------------ */
/* Fonction de calcul des coefficients d'une surface de Bezier en fonction d'un indice _i, du degre _n et d'une valeur _t */
double computeBezierCoeff(int _i, int _n, double _t)
{
	int C,dim;
	double B;
	dim = _n+1;
	C = pTriangle->coefficients[_i][_n];
	B= C * pow(_t,_i)*pow((1-_t),(_n-_i));
	return B;
}
/* Fonction de calcul d'un point de la surface de Bezier */
void computeBezierPoint(xSurface * _surface, int _i0, int _j0, double _u, double _v, mlVec3 _q)
{
	int nu, nv;
	double Bi, Bj;
	double x,y,z;
	/*printf("teste CBPoint |");*/
	x = y = z = 0;
	for(nu=0;nu<= _surface->uDegree;nu++)
	{
		for(nv=0;nv<= _surface->vDegree;nv++)
		{
			Bi=computeBezierCoeff(nu,_surface->uDegree,_u);
			Bj=computeBezierCoeff(nv,_surface->vDegree,_v);
			x += Bi * Bj * _surface->controlPoints[_i0 + nu][_i0 + nv][0];
			y += Bi * Bj * _surface->controlPoints[_i0 + nu][_i0 + nv][1];
			z += Bi * Bj * _surface->controlPoints[_i0 + nu][_i0 + nv][2];
		}
	}
	/*printf(" B (%f, %f, %f)\n", x, y, z);*/
	mlVec3_Set(_q, x, y, z);
}
/* Fonction d'affichage d'une surface de Bezier */
void drawBezierSurface(xSurface * _surface)
{
	int  p=0;
	double u,v;
	double pas = 1.0/_surface->resolution;
	mlVec3 q;
	mlVec3	P[4];
	
	for(u=0.0;u<1.0;u=u+pas)
	  for(v=0.0;v<1.0;v=v+pas)
	  {
		  
		  computeBezierPoint(_surface, 0, 0, u, v, q);
		  
		  if(p==3)
		  {
			glBegin(GL_TRIANGLES);
			glColor3f(0,0,1);
			glVertex3d(P[0][0], P[0][1],P[0][2]);
			glVertex3d(P[1][0], P[1][1],P[1][2]);
			glVertex3d(q[0], q[1], q[2]);
			glEnd();
			
			glBegin(GL_TRIANGLES);
			glColor3f(0,1,0);
			glVertex3d(P[0][0], P[0][1],P[0][2]);
			glVertex3d(P[2][0], P[2][1],P[2][2]);
			glVertex3d(q[0], q[1], q[2]);
			glEnd();
			p = 2;
			
			P[0][0] = P[2][0];
			P[0][1] = P[2][1];
			P[0][2] = P[2][2];
		    
			P[1][0] = q[0];
			P[1][1] = q[1];
			P[1][2] = q[2];
		  }
		  else
		  {
		    P[p][0] = q[0];
		    P[p][1] = q[1];
		    P[p][2] = q[2];
		    p++;
		  }
		  
	  }
}
/* --------------------------------- B-SPLINES --------------------------------- */
/* Fonction de calcul des coefficients d'une surface BSpline en fonction d'un indice _i, du degre _n et d'une valeur _t */
double computeBSplineCoeff(int _i, int _n, double _t)
{
	int k;
	double R = 0;
	for (k=0;k<=_n-_i;k++)
		R += pow(-1 , k) * pow(( _t + _n - _i - k),_n) / ( fact(k) * fact(_n-k+1));
	
	R = (_n +1) * R;
	return R;
}
/* Fonction de calcul d'un point de la surface BSpline */
void computeBSplinePoint(xSurface * _surface, int _i0, int _j0, double _u, double _v, mlVec3 _q)
{
	int nu, nv;
	double Ri, Rj;
	double x,y,z;
	/*printf("teste CBPoint |");*/
	x = y = z = 0;
	for(nu=0;nu<= _surface->uDegree;nu++)
	{
		for(nv=0;nv<= _surface->vDegree;nv++)
		{
			Ri=computeBSplineCoeff(nu,_surface->uDegree,_u);
			Rj=computeBSplineCoeff(nv,_surface->vDegree,_v);
			x += Ri * Rj * _surface->controlPoints[_i0 + nu][_i0 + nv][0];
			y += Ri * Rj * _surface->controlPoints[_i0 + nu][_i0 + nv][1];
			z += Ri * Rj * _surface->controlPoints[_i0 + nu][_i0 + nv][2];
		}
	}
	
	mlVec3_Set(_q, x, y, 0);
}
/* Fonction d'affichage d'une surface BSpline */
void drawBSplineSurface(xSurface * _surface)
{
		int  p=0;
	double u,v;
	double pas = 1.0/_surface->resolution;
	mlVec3 q;
	mlVec3	P[4];
	glBegin(GL_QUADS);
	glColor3f(0,0,1);
	for(u=0.0;u<1.0;u=u+pas)
	  for(v=0.0;v<1.0;v=v+pas)
	  {
		  
		  computeBSplinePoint(_surface, 0, 0, u, v, q);
		  printf("q (%f,%f,%f)\n", q[0],q[1],q[2]);
		  
		  if(p==3)
		  {
			glVertex3d(P[0][0], P[0][1],P[0][2]);
			glVertex3d(P[1][0], P[1][1],P[1][2]);
			glVertex3d(P[2][0], P[2][1],P[2][2]);
			glVertex3d(q[0], q[1], q[2]);
			
			p = 2;
			
			P[0][0] = P[2][0];
			P[0][1] = P[2][1];
			P[0][2] = P[2][2];
		    
			P[1][0] = q[0];
			P[1][1] = q[1];
			P[1][2] = q[2];
		  }
		  else
		  {
		    P[p][0] = q[0];
		    P[p][1] = q[1];
		    P[p][2] = q[2];
		    p++;
		  }
		  
	  }
	glEnd();
}
/* Fonction de chargement d'un fichier de surface */
void loadSurfaceFile(const char * _path, xSurface * _surface)
{
	/******* TODO *******/
}
/* Callback OpenGL d'affichage */
void displayGL()
{
	/******* TODO *******/
	
	/*glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glutSwapBuffers();*/
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(camX, camY, -camZ, camX, camY, camZ+10.0, 0, 1, 0);

	glMultMatrixd(mlTbGetRotation());

	Repere();
	if (optionComputeBezier)
	  drawBezierSurface(pSurface);
	else
	  drawBSplineSurface(pSurface);
	
	glutSwapBuffers();
}
/* Callback OpenGL de redimensionnement */
void reshapeGL(int _w, int _h)
{
	/******* TODO *******/
	winX = _w;
	winY = _h;
	
	glViewport(0, 0, winX, winY);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-25.0, +25.0, -25.0, +25.0, -25.0, +25.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	mlTbInit(winX, winY); /* init trackball */
	
	glutPostRedisplay();
	
	/*
	winX = _w;
	winY = _h;
	
	glViewport(0, 0, winX, winY);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-25.0, +25.0, -25.0, +25.0, -25.0, +25.0);
	glMatrixMode(GL_MODELVIEW);
	
	glutPostRedisplay();*/
}
/* Callback OpenGL de gestion de souris */
void mouseGL(int _button, int _state, int _x, int _y)
{	
	switch (_button)
     	{
		 case GLUT_LEFT_BUTTON: 
		 	if(_state==GLUT_DOWN)
		 	{
				mlTbClick( _x, _y);
			}
			if(_state==GLUT_UP)
		 	{
				mlTbRelease( _x,  _y);
			}	
		 break;
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
	
	if(_k == ' ')
		optionDisplaySurface = !optionDisplaySurface;
	
	if(_k == 'p' || _k == 'P')
		optionDisplayPoints = !optionDisplayPoints;
	
	if(_k == '+')
		pSurface->resolution += 25;
	
	if(_k == '-')
		pSurface->resolution -= (pSurface->resolution > 25) ? 25 : 0;
	
	if(_k == '*')
		pSurface->uDegree++;
	
	if(_k == '/')
		pSurface->uDegree -= (pSurface->uDegree > 1) ? 1 : 0;
	
	if(_k == ',')
		pSurface->vDegree++;
	
	if(_k == ';')
		pSurface->vDegree -= (pSurface->vDegree > 1) ? 1 : 0;
	
	if(_k == 'b' || _k == 'B')
		optionComputeBezier = !optionComputeBezier;
	
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
	mlTbMotion(_x, _y);	
	
	glutPostRedisplay();
}
/* Callback OpenGL de gestion de survol */
void passiveMotionGL(int _x, int _y)
{
	glutPostRedisplay();
}
/* Fonction d'initialisation des variables */
void init()
{
	camera = (glCamera *)malloc(sizeof(glCamera));
	mlVec3_Set(camera->position, +0.00, +0.25, +1.00);
	mlVec3_Set(camera->center,   +0.00, +0.00, +0.00);
	mlVec3_Set(camera->up,       +0.00, +1.00, +0.00);
	
	mlVec3_Set(trianglePos, -0.50, -0.50, +0.00);
	
	mouseLeftDown = 0;
	
	optionRenderingMode = GL_QUADS;
	optionRenderingLight = 0;
	optionDisplayVertex = 0;
	optionUniqueColor = 1;
	optionShadeModel = GL_FLAT;
	optionProjection = 1;
	optionScene = 1;
/*-------------------------------------*/
	pSurface					= (xSurface *)malloc(sizeof(xSurface));
	pSurface->uDegree			= 3;
	pSurface->vDegree			= 3;
	pSurface->nuControlPoints	= 0;
	pSurface->nvControlPoints	= 0;
	pSurface->resolution		= 5;
	
	pTriangle					= (pascalTriangle *)malloc(sizeof(pascalTriangle));
	pTriangle->coefficients		= NULL;
	pTriangle->depth			= MAX_CTRL_POINTS;
	computePascalTriangle(pTriangle);
	
	optionDisplayPoints			= 1;
	optionDisplaySurface		= 1;
	optionComputeBezier			= 1;
}
/* Fonction d'initialisation OpenGL */
void initGL()
{
	GLfloat position [] = { 0.0f, 0.0f, 10.0f, 0.0 };
	/*Z Buffer pour la suppression des parties cachées*/
	glEnable(GL_DEPTH_TEST);
	/*La variable d'état de couleur GL_AMBIENT_AND_DIFFUSE*/
	/*est définie par des appels à glColor*/
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100.0);
	
	/*Crée une source de lumière directionnelle*/
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	glClearColor(0.7, 0.8, 0.8, 1.0); /*RGB + Alpha*/
	/*Définit un modèle d'ombrage*/
	glShadeModel(GL_SMOOTH);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	/*glEnable(GL_CULL_FACE);*/
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
	printf("    ./tp05 [fichier .pts2]\n");
	printf("\n\n");
	
	printf("%c[%dm", 27, 1);
	printf("[Manuel]\n");
	printf("%c[%dm", 27, 0);
	printf("    '+'/'-'       :\t augmenter/diminuer le nombre de points d'echantillonnage\n");
	printf("    '*'/'/'       :\t augmenter/diminuer le degre de la surface en u\n");
	printf("    ','/';'       :\t augmenter/diminuer le degre de la surface en v\n");
	printf("    ' '	          :\t afficher/masquer la surface\n");
	printf("    'p'           :\t afficher/masquer les points de controle\n");
	printf("    'b'           :\t transformer en surface de Bezier/BSpline\n");
	printf("    'h'           :\t afficher cette aide\n");
	printf("    'q'/Esc       :\t quitter\n");
	printf("---------------------------------------------------------------------------\n");
}
void InitSurface()
{
	pSurface->nuControlPoints = 4;
	pSurface->nvControlPoints = 5;

	/*P1*/
	pSurface->controlPoints[0][0][0]=2.54;
	pSurface->controlPoints[0][0][1]=5.3;
	pSurface->controlPoints[0][0][2]=0.54;
	
	/*P2*/
	pSurface->controlPoints[0][1][0]=4.5;
	pSurface->controlPoints[0][1][1]=5.0;
	pSurface->controlPoints[0][1][2]=-2.21;
	
	/*P3*/
	pSurface->controlPoints[0][2][0]=8.3;
	pSurface->controlPoints[0][2][1]=3.12;
	pSurface->controlPoints[0][2][2]=4.18;

	/*P4*/
	pSurface->controlPoints[0][3][0]=7.5;
	pSurface->controlPoints[0][3][1]=5.6;
	pSurface->controlPoints[0][3][2]=-1.45;
	
	/*P5*/
	pSurface->controlPoints[1][0][0]=1.52;
	pSurface->controlPoints[1][0][1]=-4.2;
	pSurface->controlPoints[1][0][2]=5.2;
	
	/*P6*/
	pSurface->controlPoints[1][1][0]=4.5;
	pSurface->controlPoints[1][1][1]=4.2;
	pSurface->controlPoints[1][1][2]=-2.4;
	
	/*P7*/
	pSurface->controlPoints[1][2][0]=5.5;
	pSurface->controlPoints[1][2][1]=6.12;
	pSurface->controlPoints[1][2][2]=5.61;
	
	/*P8*/
	pSurface->controlPoints[1][3][0]=9.1;
	pSurface->controlPoints[1][3][1]=8.5;
	pSurface->controlPoints[1][3][2]=-6.26;
}
int LirePts2(char *nom)
{

	FILE *f;
	mlVec3 pC;
	int  i, j, U=0;
	float x, y, z;
	char s[TAILLE_MAX];

	/* Ouverture du fichier */
        f = fopen (nom, "r");
        if (f == NULL)
	{
		printf ("ERREUR fopen(%s)\n", nom);
		return -1;
	}


	fgets ( s, strlen("s "), f );
	printf("%s \n", s);
	fscanf(f, "%d ", &U);
	pSurface->nuControlPoints = U;
	printf("nb: %d \n", pSurface->nuControlPoints);

	fgets ( s, strlen(" "), f );
	printf("%s \n", s);
	fscanf(f, "%d ", &pSurface->nvControlPoints);
	printf("nb: %d \n", pSurface->nvControlPoints);

	for( i=0; i<pSurface->nuControlPoints; i++)
	{
		fscanf(f, "%c %f %f %f ", s, &x, &y, &z);
		printf("%c %f %f %f \n", s[0], x, y, z);
		pC[0] = x;
		pC[1] = y;
		pC[2] = z;
		mlVec3_Copy(pSurface->controlPoints[i][0], pC);
		for( j=1; j<MAX_CTRL_POINTS; j++)
		{
			fscanf(f, "%c %f %f %f ", s, &x, &y, &z);
			printf("%c %f %f %f \n", s[0], x, y, z);
			pC[0] = x;
			pC[1] = y;
			pC[2] = z;
			mlVec3_Copy(pSurface->controlPoints[i][j], pC);
		}
	}

	fclose (f);

	return 0;
}
/* Fonction principale */
int main(int _argc, char ** _argv)
{
	int posX, posY;
	
	glutInit(&_argc, _argv);
	if(_argc > 2)
	{
		usage();
		return 1;
	}
	
	usage();
	
	posX = (glutGet(GLUT_SCREEN_WIDTH ) - winX) / 2;
	posY = (glutGet(GLUT_SCREEN_HEIGHT) - winY) / 2;
	
	glutInitWindowSize(winX, winY);
	glutInitWindowPosition(posX, posY);
	
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
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
	
	InitSurface();
	
	if(_argc == 2)
	{
		if(strstr(_argv[1], ".pts2") != NULL)
			loadSurfaceFile(_argv[1], pSurface);
		else
		{
			printf("Loading of \"%s\": unknown format.\n", _argv[1]);
			return 1;
		}
	}
	
	glutMainLoop();

	return 0;
}
