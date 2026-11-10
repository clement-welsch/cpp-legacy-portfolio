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
#include <time.h>
#include <math.h>


/* Include de la bibliotheque de maillages */
#include <he.h>


/* Defines d'informations */
#define TP_TITLE			"[TP07] Modelisation Geometrique - Composantes connexes"
#define TP_GROUP			""
#define TP_PROMO			"2010-2011"
void	usage();


/* Taille de la fenetre */
int winX = 800;
int winY = 600;

/* Taille de la zone de picking */
#define			PICK_SIZE 4

/* Variables globales (maillage, picking, ...) */
hePtrMesh		mesh;
int				selectionType;
int				vertexSelected;
int				edgeSelected;
int				faceSelected;
int				mouseLeftDown;


/* Options d'affichage */
int				optionRenderingMode;
int				optionShowEdgesV;
int				optionShowFacesV;
int				optionShowFacesE;
int				optionShowFaceN;
int				optionShowFaceB;
int				optionShowFaceP;
int				optionShowBorders;

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
int		mouseLeftDown;
int		idCubeTexture;


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

/* Variable Perso */
double camX=0.0, camY=0.0, camZ=-3.0;
hePtrMesh mesh;
int mode = 1;
int bCCActive = 0;

/* Variables globales (maillage, composantes connexes, ...) */
hePtrMesh		mesh;
int			mouseLeftDown;

/* Variable Perso */
int tMarque [10][2000];
int iNbCC = -1, iNbFaces=0;
float tColorCC [10][3];
char* fichier = "Objet";

void InitMarque()
{
	int i, j;
	for ( i = 0;i<10;i++)
	  for (j=0;j<2000;j++)
	    tMarque [i][j]=-1;
}
int RandomRGB()
{
	int i;
	i = rand()%255;
	return i;
}
void InitColorCC()
{
	int i;
	float r,g,b;
	for ( i = 0;i<=iNbCC;i++)
	{
		r = RandomRGB();
		g = RandomRGB();
		b = RandomRGB();
		r = r / 255;
		g = g / 255;
		b = b / 255;
		
		tColorCC [i][0] = r;
		tColorCC [i][1] = g;
		tColorCC [i][2] = b;
	}
}
void Repere()
{
	glDisable(GL_LIGHTING);
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
	glEnable(GL_LIGHTING);
}
void EdgesMode(hePtrMesh _mesh)
{
	int i, v1,v2;
	
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(1.0);
	for(i = 0; i < mesh->nEdges; ++i)
	{
		v1 = mesh->edges[i]->tail->id;
		v2 = mesh->edges[i]->head->id;
		
		glBegin(GL_LINES);
		glNormal3dv(_mesh->verts[v1]->normal);
		glVertex3dv(_mesh->verts[v1]->pos);
		glNormal3dv(_mesh->verts[v2]->normal);
		glVertex3dv(_mesh->verts[v2]->pos);
		glEnd();
	}
}
/* Fonction de reorientation des normales */
void reorientMeshNormals(hePtrMesh _mesh)
{
	/******* TODO *******/
}
int marquage(int id)
{
	int i,j,bMarque = 0;
	for ( i = 0;i<10;i++)
	{
		for (j=0;j<2000;j++)
		{
			/*if (tMarque [i][j] !=-1)
			  printf("tMarque [%i][%i] = %i\n", i,j,tMarque [i][j]);*/
			if (tMarque [i][j] == id)
			  bMarque = 1;
		}
		/*printf("---------\n");*/
	}
	/*printf("test 2.1\n");*/
	return bMarque;
}
/* Fonction de comptage de composantes connexes */
void getComponents(hePtrMesh _mesh)
{
	int i, ii, j, v1, v2;
	hePtrListItem	it;
	hePtrListItem	jt;
	
	for(i = 0; i < _mesh->nFaces; ++i)
	{
		/*printf("test 1\n");*/
		/*Si la face n'est pas déjà marquée*/
		/*On Crée une nouvelle Composante Connexe*/
		if (marquage(i) ==0)
		{
			iNbCC ++;
			iNbFaces = 0;
			/*printf("test 2: tMarque [%i][%i] = %i\n",iNbCC,iNbFaces,i);*/
			tMarque [iNbCC][iNbFaces] = i;
			iNbFaces++;
			/*printf("test 3\n");*/
			/*On cherche les faces voisines grâce aux arètes commune*/
			for (ii=0;ii<iNbFaces;ii++)
			{
				/*printf("test 4 : tMarque [%i][%i] = %i\n",iNbCC,ii,tMarque [iNbCC][ii]);*/
				for(it = _mesh->faces[tMarque [iNbCC][ii]]->edges.head; it != NULL; it = it->next)
				{
					/*printf("test 4.2\n");*/
					/*On stocke une arête de la face I*/
					v1 = ((hePtrEdge)it->data)->tail->id;
					v2 = ((hePtrEdge)it->data)->head->id;
					/*printf("test 5\n");*/
					
					for(j = 0; j < _mesh->nFaces; ++j)
					{
						/*printf("test 6\n");*/
						/* Si cette arète est présente dans la liste de face de la face J*/
						for(jt = _mesh->faces[j]->edges.head; jt != NULL; jt = jt->next)
						{
							if (((((hePtrEdge)jt->data)->tail->id == v1) && (((hePtrEdge)jt->data)->head->id == v2))
							  || ((((hePtrEdge)jt->data)->tail->id == v2) && (((hePtrEdge)jt->data)->head->id == v1)))
							{	/*On véridie si elle n'est pas déjà marquée*/
								if (marquage(j) == 0)
								{
								        /*printf("test 7 : tMarque[%i][%i] = %i\n",iNbCC,iNbFaces,tMarque[iNbCC][iNbFaces]);*/
									tMarque[iNbCC][iNbFaces] = j;
									iNbFaces++;
									/*printf("test 8\n");*/
								}
							}
						}
					}
				}
			}
		}
	}
	printf("Nombre de composante(s) connexe(s) : %d\n", iNbCC+1);
}
void affichageCC (hePtrMesh _mesh)
{
	int i,j,v1;
	
	hePtrListItem	it;
	
	for ( i = 0;i<10;i++)
	{
		for (j=0;j<2000;j++)
		{
			if (tMarque [i][j] !=-1)
			{
				glColor3f(tColorCC[i][0], tColorCC[i][1], tColorCC[i][2]);
				glBegin(GL_POLYGON);
				for(it = _mesh->faces[tMarque [i][j]]->edges.head; it != NULL; it = it->next)
				{
					v1 = ((hePtrEdge)it->data)->tail->id;
					
					glNormal3dv(_mesh->verts[v1]->normal);
					glVertex3dv(_mesh->verts[v1]->pos);
				}
				glEnd();
			}
		}
	}
}
void saveOBJ(hePtrMesh _mesh, char* nom, int nCC)
{
	int i, j=0, v1;
	hePtrListItem it;
	char nouveauNom[100];
	FILE** f;

	printf("test 1\n");
	f = (FILE**)malloc(sizeof(FILE*)*nCC+1);
	for(i=0; i< nCC+1; i++)
	{

		sprintf(nouveauNom,"OBJ/%s_part_%d.obj",nom,i);
		f[i]= (FILE*)malloc(sizeof(FILE));
		f[i] = fopen(nouveauNom, "w");
	}
	printf("test 2\n");
	/* Ecrire les Sommets de l'objets */
	for(i = 0; i < nCC+1; ++i)
	{
		while (tMarque[nCC][j] !=-1)
		{
			for(it = _mesh->faces[tMarque [nCC][j]]->edges.head; it != NULL; it = it->next)
			{
				v1 = ((hePtrEdge)it->data)->tail->id;
				printf("test 4 tMarque [%i][%i]= %i\n", nCC, j, tMarque [nCC][j]);
				printf("test 4.2 f %f %f %f\n",  _mesh->verts[v1]->pos[0],_mesh->verts[v1]->pos[1],_mesh->verts[v1]->pos[2]);
				fprintf(f[j], "v %f %f %f\n", _mesh->verts[v1]->pos[0],_mesh->verts[v1]->pos[1],_mesh->verts[v1]->pos[2]);
			}
			j++;
		}
	}
	for(i=0; i< nCC+1; i++)
	{
		printf("test 5\n");
		if (f[i] !=NULL)
		{
			printf("test 5.2\n");
			fclose(f[i]);
		}
		
	}
	printf("test 6\n");
}
void saveFile (char* nom)
{
	FILE* f;

	f = fopen(nom, "w");
	fprintf(f, "v 0 0 0\n");

	fclose(f);
}
/* Callback OpenGL d'affichage */
void displayGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(camX, camY, -camZ, camX, camY, camZ+1.0, 0, 1, 0);
	glMultMatrixd(mlTbGetRotation());
	
	if(mesh!= NULL)
	{
		if (bCCActive==1) 
		  affichageCC (mesh);
		else
		  EdgesMode(mesh);
	}
	Repere();
	
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
	gluPerspective(60.0, (double)winX/(double)winY, 0.1, 64.0);
	/*glOrtho(-25.0, +25.0, -25.0, +25.0, -25.0, +25.0);*/
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	mlTbInit(winX, winY); /* init trackball */
	
	glutPostRedisplay();
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
	/*int i=0;*/
	
	if(_k == 27 || _k == 'q' || _k == 'Q')
		exit(0);
	
	if(_k == 'h' || _k == 'H')
		usage();
	
	if(_k == 'c' || _k == 'C')
	{
		bCCActive = 1;
		getComponents(mesh);
		InitColorCC();
	}
	
	if(_k == 'r' || _k == 'R')
		reorientMeshNormals(mesh);
	if(_k == 's' || _k == 'S')
	{
		if (bCCActive==1)
		{
			/*for ( i = 0;i<iNbCC+1;i++)*/
				/*saveOBJ(mesh, fichier, 0);*/
				saveFile (fichier);
		}
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
	/*----------------------------------------------- */
	selectionType			= 0;
	vertexSelected			= 0;
	edgeSelected			= 0;
	faceSelected			= 0;
	mouseLeftDown			= 0;
	
	optionRenderingMode		= GL_FILL;
	optionShowEdgesV		= 0;
	optionShowFacesV		= 0;
	optionShowFacesE		= 0;
	optionShowFaceN			= 0;
	optionShowFaceB			= 0;
	optionShowFaceP			= 0;
	optionShowBorders		= 0;
}
/* Fonction d'initialisation OpenGL */
void initGL()
{
	float mat_specular   [4] = { 0.18, 0.18, 0.18, 0.18 };
	float mat_shininess  [ ] = { 128 };
	float global_ambient [ ] = { 0.02, 0.02, 0.05, 0.05 };
	float light0_ambient [ ] = { 0, 0, 0, 0 };
	float light0_diffuse [ ] = { 0.85, 0.85, 0.85, 0.85 };
	float light0_specular[ ] = { 0.85, 0.85, 0.15, 0.85 };
	GLfloat position [] = { 0.0f, 0.0f, 10.0f, 0.0 };
	
	/*Z Buffer pour la suppression des parties cachées*/
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.7, 0.8, 0.8, 1.0); /*RGB + Alpha*/
	
	/*La variable d'état de couleur GL_AMBIENT_AND_DIFFUSE*/
	/*est définie par des appels à glColor*/
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
	
	/*Crée une source de lumière directionnelle*/
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	/*Définit un modèle d'ombrage*/
	glShadeModel(GL_SMOOTH);
	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
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
	printf("    ./tp07 [.obj file]\n");
	printf("\n\n");
	
	printf("%c[%dm", 27, 1);
	printf("[Manuel]\n");
	printf("%c[%dm", 27, 0);
	printf("    'Clic gauche' :\t bouger la camera (trackball)\n");
	printf("    'c'           :\t afficher les composantes connexes\n");
	printf("    's'           :\t Sauvgarder les composantes connexes\n");
	printf("    'r'           :\t reorienter le maillage\n");
	printf("    'h'           :\t afficher cette aide\n");
	printf("    'q'/Esc       :\t quitter\n");
	printf("---------------------------------------------------------------------------\n");
}
/* Fonction principale */
int main(int _argc, char ** _argv)
{
	int posX, posY;
	
	glutInit(&_argc, _argv);
	/* Switching stdout to un-buffered */
	setvbuf(stdout, NULL, _IONBF, 0);
	
	/* Argument testing */
	if(_argc < 2)
	{
		fprintf(stderr, "usage: %s file\n", _argv[0]);
		return EXIT_FAILURE;
	}
	
	/* Mesh creation */
	mesh = heMesh_New();
	
	/* Mesh loading */
	printf("Loading... ");
	if(heMesh_Load(mesh, _argv[1]) < 0)
	{
		printf("Fail.\n");
		heMesh_Free(&mesh);
		
		return EXIT_FAILURE;
	}
	
	heMesh_Normalize(mesh);
	heMesh_Compute_Normals(mesh);
	
	if(_argc < 2)
	{
		usage();
		return 1;
	}
	/*getInfo(mesh);*/
	
	usage();
	/* Init Tableau de marquage*/
	InitMarque();
	
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
	glutMainLoop();

	return 0;
}
