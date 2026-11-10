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


/* Include de la bibliotheque de maillages */
#include <he.h>


/* Defines d'informations */
#define TP_TITLE			"[TP06] Modelisation Geometrique - Chargeur de maillages"
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
int	mouseLeftDown;

/* Variable Perso */
double camX=0.0, camY=0.0, camZ=-3.0;
hePtrMesh mesh;
int mode = 0;
int iSommetCourant = 0;
int iNbSommets = 0;
int bAretesCourantes = 0;
int bFacesCourantes = 0;
int bGeoCourante = 0;
int iAreteCourante = 0;
int iNbAretes = 0;
int iFaceCourante = 0;
int iNbFaces = 0;

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
void VertexMode(hePtrMesh _mesh)
{
	int i;
	
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(1.0);
	for(i = 0; i < _mesh->nVerts; ++i)
	{
		glBegin(GL_POINTS);
		glNormal3dv(_mesh->verts[i]->normal);
		glVertex3dv(_mesh->verts[i]->pos);/*Load un Vecteur*/
		glEnd();
	}
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
void FacesMode(hePtrMesh _mesh)
{
	int i, v1;
	hePtrListItem	it;
	glColor3f(0.0, 0.8, 0.5);
	glPointSize(1.0);
	for(i = 0; i < _mesh->nFaces; ++i)
	{
		glBegin(GL_POLYGON);
		for(it = _mesh->faces[i]->edges.head; it != NULL; it = it->next)
		{
			v1 = ((hePtrEdge)it->data)->tail->id;
			
			glNormal3dv(_mesh->verts[v1]->normal);
			glVertex3dv(_mesh->verts[v1]->pos);
		}
		glEnd();
	}
}
/* ---------------------------   */
/* ------ SommetCourant ------  */
/* --------------------------- */
void SommetCourant(hePtrMesh _mesh)
{
	int i;
	
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(5.0);
	for(i = 0; i < _mesh->nVerts; ++i)
	{
		if (i == iSommetCourant)
		{
			glBegin(GL_POINTS);
				glNormal3dv(_mesh->verts[i]->normal);
				glVertex3dv(_mesh->verts[i]->pos);/*Load un Vecteur*/
			glEnd();
		}
	}
}
void SCAretes(hePtrMesh _mesh)
{
	int i, v1,v2;
	
	glColor3f(1.0, 0.0, 0.0);
	glLineWidth(3.0);
	for(i = 0; i < mesh->nEdges; ++i)
	{
		v1 = mesh->edges[i]->tail->id;
		v2 = mesh->edges[i]->head->id;
		
		if ((v1 == iSommetCourant)||(v2 == iSommetCourant))
		{
			glBegin(GL_LINES);
			glNormal3dv(_mesh->verts[v1]->normal);
			glVertex3dv(_mesh->verts[v1]->pos);
			glNormal3dv(_mesh->verts[v2]->normal);
			glVertex3dv(_mesh->verts[v2]->pos);
			glEnd();
		}
	}
	glLineWidth(1.0);
}
void SCFaces(hePtrMesh _mesh)
{
	int i, j, tab[100], bIdPresent=0;
	hePtrListItem	it;
	glColor3f(0.8, 0.2, 0.2);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	for(i = 0; i < 100; ++i)
	  tab[i]= -1;
	
	for(i = 0; i < _mesh->nFaces; ++i)
	{
		/* Si le sommet est présent dans la liste de sommet d'une face*/
		for(it = _mesh->faces[i]->edges.head; it != NULL; it = it->next)
		{
			if (((hePtrEdge)it->data)->tail->id == iSommetCourant)
				bIdPresent = 1;
		}
		if (bIdPresent == 1)
		{
			/* On récupère la liste des sommets comprient dans cette face*/
			for(it = _mesh->faces[i]->edges.head,j = 0; it != NULL; it = it->next, j++)
				tab [j] = ((hePtrEdge)it->data)->tail->id;
			/* Et on les affiche*/
			glBegin(GL_POLYGON);
			for(j = 0; j < 100; ++j)
			{
				if (tab [j] != -1)
				{
					glNormal3dv(_mesh->verts[tab [j]]->normal);
					glVertex3dv(_mesh->verts[tab [j]]->pos);
					tab [j] = -1;
				}
			}
			glEnd();
			bIdPresent = 0;
		}
	}
	if (mode == 0)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	else if (mode == 1)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if (mode == 2)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
/* ---------------------------   */
/* ------ AreteCourante ------  */
/* --------------------------- */
void AreteCourante (hePtrMesh _mesh)
{
	int i, v1,v2;
	
	glColor3f(1.0, 0.0, 1.0);
	glLineWidth(5.0);
	for(i = 0; i < mesh->nEdges; ++i)
	{
		
			
		if (i == iAreteCourante)
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
	glLineWidth(1.0);
}
void ACSommets (hePtrMesh _mesh)
{
	int i, v1,v2;
	
	glColor3f(0.8, 0.2, 0.8);
	glPointSize(5.0);
	for(i = 0; i < mesh->nEdges; ++i)
	{
		if (i == iAreteCourante)
		{
			v1 = mesh->edges[i]->tail->id;
			v2 = mesh->edges[i]->head->id;
			
			glBegin(GL_POINTS);
			glNormal3dv(_mesh->verts[v1]->normal);
			glVertex3dv(_mesh->verts[v1]->pos);
			glEnd();
			
			glBegin(GL_POINTS);
			glNormal3dv(_mesh->verts[v2]->normal);
			glVertex3dv(_mesh->verts[v2]->pos);
			glEnd();
		}
	}
}
void ACFaces (hePtrMesh _mesh)
{
	int i, j, tab[100], v1, v2;
	int bIdPresent=0;
	hePtrListItem	it;
	
	glColor3f(0.8, 0.2, 0.8);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	v1 = mesh->edges[iAreteCourante]->tail->id;
	v2 = mesh->edges[iAreteCourante]->head->id;
	
	for(i = 0; i < 100; ++i)
	  tab[i]= -1;
	
	for(i = 0; i < _mesh->nFaces; ++i)
	{
		/* Si l'arète (ses deux sommets) est présente dans la liste d'arètes d'une face*/
		for(it = _mesh->faces[i]->edges.head; it != NULL; it = it->next)
		{
			if (((((hePtrEdge)it->data)->tail->id == v1) && (((hePtrEdge)it->data)->head->id == v2))
			  || ((((hePtrEdge)it->data)->tail->id == v2) && (((hePtrEdge)it->data)->head->id == v1)))
				bIdPresent = 1;
		}
		if (bIdPresent == 1)
		{
			/* On récupère la liste des sommets comprient dans cette arètes*/
			for(it = _mesh->faces[i]->edges.head,j = 0; it != NULL; it = it->next, j++)
				tab [j] = ((hePtrEdge)it->data)->tail->id;
			/* Et on les affiche*/
			glBegin(GL_POLYGON);
			for(j = 0; j < 100; ++j)
			{
				if (tab [j] != -1)
				{
					glNormal3dv(_mesh->verts[tab [j]]->normal);
					glVertex3dv(_mesh->verts[tab [j]]->pos);
					tab [j] = -1;
				}
			}
			glEnd();
			bIdPresent = 0;
		}
	}
	if (mode == 0)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	else if (mode == 1)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if (mode == 2)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
/* -----------------------------   */
/* ------ FaceCourante ------  */
/* ----------------------------- */
void FaceCourante(hePtrMesh _mesh)
{
	int i, v1;
	hePtrListItem	it;
	glColor3f(1.0, 1.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	for(i = 0; i < _mesh->nFaces; ++i)
	{
		if ( i == iFaceCourante)
		{
			glBegin(GL_POLYGON);
			for(it = _mesh->faces[i]->edges.head; it != NULL; it = it->next)
			{
				v1 = ((hePtrEdge)it->data)->tail->id;
				
				glNormal3dv(_mesh->verts[v1]->normal);
				glVertex3dv(_mesh->verts[v1]->pos);
			}
			glEnd();
		}
	}
	if (mode == 0)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	else if (mode == 1)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if (mode == 2)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
void FCContours(hePtrMesh _mesh)
{
	int i, v1;
	hePtrListItem	it;
	glColor3f(0.8,  0.2, 0.2);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(3.0);
	for(i = 0; i < _mesh->nFaces; ++i)
	{
		if ( i == iFaceCourante)
		{
			glBegin(GL_POLYGON);
			for(it = _mesh->faces[i]->edges.head; it != NULL; it = it->next)
			{
				v1 = ((hePtrEdge)it->data)->tail->id;
				
				glNormal3dv(_mesh->verts[v1]->normal);
				glVertex3dv(_mesh->verts[v1]->pos);
			}
			glEnd();
		}
	}
	glLineWidth(1.0);
	if (mode == 0)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	else if (mode == 1)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if (mode == 2)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
void FCFaces(hePtrMesh _mesh)
{
	int i, j, k, tab[100], v1, v2;
	int bIdPresent=0;
	hePtrListItem	it;
	hePtrListItem	jt;
	
	glColor3f(0.8, 0.2, 0.8);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	for(i = 0; i < 100; ++i)
		tab[i]= -1;
	
	for(i = 0; i < _mesh->nFaces; ++i)
	{
		if ( i == iFaceCourante)
		{
			for(it = _mesh->faces[i]->edges.head; it != NULL; it = it->next)
			{
				v1 = ((hePtrEdge)it->data)->tail->id;
				v2 = ((hePtrEdge)it->data)->head->id;
				
				for(j = 0; j < _mesh->nFaces; ++j)
				{
					/* Si l'arète (ses deux sommets) est présente dans la liste d'arètes d'une face*/
					for(jt = _mesh->faces[j]->edges.head; jt != NULL; jt = jt->next)
					{
						if (((((hePtrEdge)jt->data)->tail->id == v1) && (((hePtrEdge)jt->data)->head->id == v2))
						  || ((((hePtrEdge)jt->data)->tail->id == v2) && (((hePtrEdge)jt->data)->head->id == v1)))
							bIdPresent = 1;
					}
					if (bIdPresent == 1)
					{
						/* On récupère la liste des sommets comprient dans cette arètes*/
						for(jt = _mesh->faces[j]->edges.head,k = 0; jt != NULL; jt = jt->next, k++)
							tab [k] = ((hePtrEdge)jt->data)->tail->id;
						/* Et on les affiche*/
						glBegin(GL_POLYGON);
						for(k = 0; k < 100; ++k)
						{
							if (tab [k] != -1)
							{
								glNormal3dv(_mesh->verts[tab [k]]->normal);
								glVertex3dv(_mesh->verts[tab [k]]->pos);
								tab [k] = -1;
							}
						}
						glEnd();
						bIdPresent = 0;
					}
				}
			}
			
			
		}
	}
	if (mode == 0)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	else if (mode == 1)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if (mode == 2)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
/* Fonction d'affichage des informations sur le maillage */
void getInfo(hePtrMesh _mesh)
{
	/* Writing mesh properties */
	printf("Done.\n\n");
	printf("Mesh properties:\n");
	printf("\t%d vertices\n", _mesh->nVerts);
	printf("\t%d faces\n", _mesh->nFaces);
	printf("\t%d edges\n", _mesh->nEdges / 2);
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
		if (mode ==0)
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		else if (mode == 1)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else if (mode == 2)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
		FacesMode(mesh);
		
		if (bGeoCourante == 0)
		{
			if (bAretesCourantes == 1)
				SCAretes(mesh);
			if (bFacesCourantes == 1)
				SCFaces(mesh);
			SommetCourant(mesh);
		}
		else if(bGeoCourante == 1)
		{
			if (bAretesCourantes == 1)
				ACSommets(mesh);
			if (bFacesCourantes == 1)
				ACFaces(mesh);
			AreteCourante(mesh);
		}
		else if (bGeoCourante == 2)
		{
			if (bAretesCourantes == 1)
				FCContours(mesh);
			if (bFacesCourantes == 1)
				FCFaces(mesh);
			FaceCourante(mesh);
		}
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
	/******* TODO *******/
	
	if(_k == 27 || _k == 'q' || _k == 'Q')
		exit(0);
	
	if(_k == 'h' || _k == 'H')
		usage();
	
	if(_k == '1')
		mode = 0;
	
	if(_k == '2')
		mode = 1;
	
	if(_k == '3')
		mode = 2;
	if(_k == '+')
	{
		if(bGeoCourante == 0)
		{
			if (iSommetCourant < iNbSommets - 1)
				iSommetCourant ++;
			else if (iSommetCourant == iNbSommets - 1)
				iSommetCourant = 0;
		}
		else if (bGeoCourante == 1)
		{
			if (iAreteCourante < iNbAretes - 2)
				iAreteCourante +=2;
			else if (iAreteCourante == iNbAretes - 2)
				iAreteCourante = 0;
		}
		else if (bGeoCourante == 2)
		{
			if (iFaceCourante < iNbFaces - 1)
				iFaceCourante ++;
			else if (iFaceCourante == iNbFaces - 1)
				iFaceCourante = 0;
		}
	}
	if(_k == '-')
	{
		if(bGeoCourante == 0)
		{
			if (iSommetCourant > 0)
				iSommetCourant --;
			else if (iSommetCourant == 0)
				iSommetCourant = iNbSommets - 1;
		}
		else if (bGeoCourante == 1)
		{
			if (iAreteCourante > 0)
				iAreteCourante -=2;
			else if (iAreteCourante == 0)
				iAreteCourante = iNbAretes - 2;
		}
		else if (bGeoCourante == 2)
		{
			if (iFaceCourante > 0)
				iFaceCourante --;
			else if (iFaceCourante == 0)
				iFaceCourante = iNbFaces - 1;
		}
	}
	printf("Sommet Courant : %i\n",iSommetCourant);
	printf("Arete Courant : %i\n",iAreteCourante);
	printf("Face Courant : %i\n",iFaceCourante);
	if(_k == 'a' || _k == 'A')
	{
		if (bAretesCourantes == 0)
		{
			bAretesCourantes = 1;
			bFacesCourantes = 0;
		}
		else
			bAretesCourantes = 0;
	}
	if(_k == 'p' || _k == 'P')
	{
		if (bFacesCourantes == 0)
		{
			bFacesCourantes = 1;
			bAretesCourantes = 0;
		}
		else
			bFacesCourantes = 0;
	}
	if(_k == 't' || _k == 'T')
	{
		/******* TODO *******/
	}
	if(_k == 'i' || _k == 'I')
	{
		/******* TODO *******/
	}	
	if(_k == 'b' || _k == 'B')
	{
		/******* TODO *******/
	}
	if(_k == 9)/*TAB*/
	{
		if(bGeoCourante == 0)
			bGeoCourante = 1;
		else if (bGeoCourante == 1)
			bGeoCourante = 2;
		else if (bGeoCourante == 2)
			bGeoCourante = 0;
	}
	if(_k == 25)
	{
		printf("25\n");
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
	printf("    ./tp06 [.obj file]\n");
	printf("\n\n");
	
	printf("%c[%dm", 27, 1);
	printf("[Manuel]\n");
	printf("%c[%dm", 27, 0);
	printf("    'Clic gauche' :\t bouger la camera (trackball)\n");
	printf("    'Clic droit'  :\t selectionner un element\n");
	printf("    '1'/'2'/'3'   :\t changer le mode de rendu\n");
	printf("    'TAB'         :\t changer le mode de selection\n");
	printf("    '+'/'-'       :\t naviguer entre les elements\n");
	printf("    'a'           :\t afficher/masquer les aretes liees a l'element selectionne\n");
	printf("    'p'           :\t afficher/masquer les polygones lies a l'element selectionne\n");
	printf("    't'           :\t afficher/masquer le plan tangent du polygone selectionne\n");
	printf("    'b'           :\t afficher/masquer les bords du maillage\n");
	printf("    'i'           :\t afficher les informations sur le maillage\n");
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
	
	/* Writing mesh properties */
	printf("Done.\n\n");
	printf("Mesh properties:\n");
	printf("\t%d vertices\n", mesh->nVerts);
	printf("\t%d faces\n", mesh->nFaces);
	printf("\t%d edges\n", mesh->nEdges / 2);
	iNbSommets = mesh->nVerts;
	iNbAretes = mesh->nEdges / 2;
	iNbFaces = mesh->nFaces;
	
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

	/* Mesh destruction */
	heMesh_Free(&mesh);
	
	return EXIT_SUCCESS;
	return 0;
}
