#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <string.h>
#include <time.h>
#include <math.h>
using namespace std;

typedef struct Point
{
	int x;
	int y;
	int classe;
} Point;
typedef struct Representant //Représentant
{
	int x;
	int y;
	float r;
	float g;
	float b;
	int classe;
} Rep;

vector<Point> listePoint;
vector<Rep> listeClasse;
int indice = 0, indiceClasse = 0,selection=-1, Classe;
int height = 600, width = 600;
//------RANDOM------
int RandomPos()
{	
  int i;
  //srand(time(NULL));
  i = rand()%width;
  return i;
}
int RandomRGB()
{	
  int i;
  //srand(time(NULL));
  i = rand()%255;
  return i;
}
//------------------
void DefinitionClasse()
{
	int x,y;
	//Radom X
	x = RandomPos();
	//Random Y
	y = RandomPos();
	//Créer Classe
	Rep temp;
	temp.x = x;
	temp.y = y;
	temp.r = RandomRGB();
	temp.g = RandomRGB();
	temp.b = RandomRGB();
	temp.r = temp.r / 255;
	temp.g = temp.g / 255;
	temp.b = temp.b / 255;
	temp.classe = indiceClasse;
	listeClasse.insert(listeClasse.begin()+indiceClasse,temp);
	indiceClasse++;
}
float Distance (int indicePoint, int indiceClasse)
{
  int x,y,x2,y2;
  float dist;
  
  x = listePoint.at(indicePoint).x;
  y = listePoint.at(indicePoint).y;
  
  x2 = listeClasse.at(indiceClasse).x;
  y2 = listeClasse.at(indiceClasse).y;
  
  //cout<<"x :"<<x<<"y : "<<y<<"x2 : "<<x2<<"y2 : "<<y2<<endl;
  
  dist = sqrt(pow(x-x2,2)+pow(y-y2,2));
  //cout <<"distance : "<<dist<<endl;
  return dist;
}
void DefinirClassePoint()
{
	int indice;
	float proch=-1, dist;
	for(int i = 0;i<listePoint.size();i++)
	{
		for(int j = 0;j<listeClasse.size();j++)
		{
		  dist = Distance (i, j);
		  if(proch == -1)
		    proch = dist;
		  if(dist < proch)
		  {
		    proch = dist;
		    indice = j;
		  }
		   //cout <<"distance : "<<dist<<"vs proch :"<<proch<<endl;
		   //cout <<"--"<<endl;
		}
		//cout <<"-----------------------------------------"<<endl;
		listePoint.at(i).classe = indice;
		proch = -1;
	}
}
void Barycentre ()
{
	int nbpoint = 0;
	int ixG = 0,iyG = 0;
	float xG = 0,yG = 0;
	for (int i = 0;i<listeClasse.size();i++)//Parcours les Classes
	{cout<<"x:"<<listeClasse.at(i).x<<" |y:"<<listeClasse.at(i).y;
		for (int j = 0; j < listePoint.size();j++)//parcours les Points de la Classe i
		{
			//Nombre de X + Nombre de Y
			if (listePoint.at(j).classe == i)
			{
				xG = xG + listePoint.at(j).x;
				yG = yG + listePoint.at(j).y;
				nbpoint ++;
				cout<<"|| x2:"<<xG<<" |y2:"<<yG;
			}
			
		}
		//Moyenne de Classe
		xG = xG / nbpoint;
		yG = yG / nbpoint;
		//Créer Barycentre
		listeClasse.at(i).x = (int) ixG;
		listeClasse.at(i).y = (int) iyG;
		nbpoint = 0;
		cout<<"|| x3:"<<xG<<" |y3:"<<yG<<endl;
		//Réinitialisation
		xG = 0;
		yG = 0;
	}
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
		case GLUT_LEFT_BUTTON://gestion du bouton gauche 
			if (etat==GLUT_DOWN)//Si le bouton gauche est cliqué
			{
				for(int i = 0; i<listePoint.size();i++)//Définit si on clique sur une position qui possède déjà un point à +/- 5px du clic
				{
					if(listePoint.at(i).x > x-5 && listePoint.at(i).x < x+5 
					  && listePoint.at(i).y > 600 -y -5 
					  && listePoint.at(i).y < 600 - y +5 )
					{
						selection = i;
						break;
					}
				}
			}
			if (etat==GLUT_UP)//Si le bouton gauche est relaché
			{
				if(selection == -1)//Création d'un nouveau point
				{
					Point temp;
					temp.x = x;
					temp.y = 600-y;
					listePoint.insert(listePoint.begin()+indice,temp);
					indice++;
				}
				else
				selection = -1;
			}
			break ;
		case GLUT_RIGHT_BUTTON:
			if (etat==GLUT_DOWN)// Supression d'un point
			{
				for(int i = 0; i<listePoint.size();i++)
				{
					if(listePoint.at(i).x > x-5 && listePoint.at(i).x < x+5 
					  && listePoint.at(i).y > 600 -y -5 
					  && listePoint.at(i).y < 600 - y +5 )
					{
						listePoint.erase(listePoint.begin()+i);
						indice--;
						break;
					}
				}
			}
			break;
	}
	glutPostRedisplay() ;
}
void mouvement(int x,int y) // Calcule le déplacement du point avec le curseur
{
	if(selection != -1)
	{
		listePoint.at(selection).x = x;
		listePoint.at(selection).y = 600-y;
		glutPostRedisplay();
	}
}
void clavier(unsigned char key, int x, int y)
{
	string s;
	 char* str;
	switch(key)
	{
		case 'a' :
		case 'A':
			cout<<"Donner le nombre de Classe!"<<endl;
			//--------ALT + TAB--------
			/*keybd_event(VK_MENU, 0, 0, 0);
			keybd_event(VK_TAB, 0, 0, 0);
			keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);*/
			//-------------------------
			cin>>Classe;
			for (int i = 0; i<Classe;i++)
				DefinitionClasse();
		break;
		case 'z' :
		case 'Z':
			Barycentre();
			glutPostRedisplay();
		break;
		case 'e' :
		case 'E':
		  //for(int i = 0; i<listePoint.size()*2;i++)
		while(listePoint.size() != NULL)
		    listePoint.erase(listePoint.begin());
		indice = 0;
		while(listeClasse.size() != NULL)
			listeClasse.erase(listeClasse.begin());
		indiceClasse = 0;
		break;
		case 27 :
		exit(0);
		break;
	}
	glutPostRedisplay();
}
void affichage(void)
{
	float r=1.0f,g=1.0f,b=1.0f;
	if (indiceClasse != 0)
	  DefinirClassePoint();
	glClear (GL_COLOR_BUFFER_BIT);
	glPointSize(10);
	glBegin(GL_POINTS);
	for(int i = 0;i<listePoint.size();i++)
	{
		if (indiceClasse != 0)
		{
		  r = listeClasse.at(listePoint.at(i).classe).r;
		  g = listeClasse.at(listePoint.at(i).classe).g;
		  b = listeClasse.at(listePoint.at(i).classe).b;
		}
		glColor3f(r,g,b);
		glVertex2i(listePoint.at(i).x,listePoint.at(i).y);
	}
	glEnd();
	glFlush();
	glBegin(GL_TRIANGLES);
	for(int i = 0;i<listeClasse.size();i++)
	{
		glColor3f(listeClasse.at(i).r,listeClasse.at(i).g,listeClasse.at(i).b);
		glVertex2i(listeClasse.at(i).x-5,listeClasse.at(i).y-5);
		glVertex2i(listeClasse.at(i).x,listeClasse.at(i).y+5);
		glVertex2i(listeClasse.at(i).x+5,listeClasse.at(i).y-5);
	}
	glEnd();
	glFlush();
}
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitWindowSize(width, height);
	glutInitWindowPosition(500, 500);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	glutCreateWindow("Tp6");
	glutDisplayFunc(affichage);

	glutReshapeFunc(redim);
	glutMouseFunc(souris) ;
	glutMotionFunc(mouvement);
	glutKeyboardFunc(clavier);

	glutMainLoop();
	return 0;
}
