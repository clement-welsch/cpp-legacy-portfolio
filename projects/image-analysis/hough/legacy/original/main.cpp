#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <unistd.h>
#include <vector>

int I1, I2, I3;
bool Click = false;
float x_souris=0,y_souris=0;
int Accu[100][100];
int iDimCellule = 10;

void initAccu()
{
	for(int i = 0;i<20;i++)
		for (int j=0;j<58;j++)
			Accu[i][j]=0;
}

struct point 
{
        int x;
        int y;
	float r;
	float g;
	float b;
        point(int _x,int _y,float _r, float _g,float _b){ x=_x;y=_y;r=_r;g=_g;b=_b;}
};
typedef struct
{
	float theta;
	float rho;
} ptHough;

std::vector<point*> liste;
int RandomRGB()
{	
  int i;
  //srand(time(NULL));
  i = rand()%255;
  return i;
}
void onetomany()
{
	float Ro;
	glBegin(GL_POINTS);
	for(unsigned int i = 0;i<liste.size();i++)
		for (float Theta = -90*(M_PI/180);Theta<90*(M_PI/180);Theta = Theta + 0.1*(M_PI/180))
		{
			glColor3f(liste.at(i)->r,liste.at(i)->g,liste.at(i)->b);
			Ro = (liste.at(i)->x) * cos(Theta) + (liste.at(i)->y) * sin(Theta);
			glVertex2f(Theta*(180/M_PI)+100,Ro + 300);
			//std::cout<<"Theta : "<<Theta<<" | Ro : "<<Ro<<std::endl;
		}
	glEnd();
}
double sgn(double i)
{
	if (i>=0)
		return 1;
	else
		return 0;
}
void manytoone()
{
	/*(θ,ρ) pour θ ∈ ]-90°, 90°], ρ=PTS[i].abs*cos θ + PTS[i].ord*sin θ*/
	int i,j;
	double a,b,c,x1,x2,y1,y2;
	ptHough PH;
	
	glPointSize(2.0);
	glColor3f(1, 1, 1);
	glBegin(GL_POINTS);
	
	for(i = 0; i<liste.size(); i++)
		for(j = 0; j<i; j++)
		{
			x1 = liste.at(i)->x;
			y1 = liste.at(i)->y;
			x2 = liste.at(j)->x;
			y2 = liste.at(j)->y;
	
			a = x1*y2-x2*y1;
			b = pow(x2,2)-2*x2*x1+x1*x1+y2*y2-2*y2*y1+y1*y1;
			c = pow(y1-y2,2);
			
			PH.theta = -atan((x1-x2)/(y1-y2)) ;
			PH.rho=sgn(y1-y2)-a/((y1-y2)*sqrt(b/c));
			//std::cout<<"Theta :"<<PH.theta<<" Rho :"<<PH.rho<<std::endl;
			glColor3f(liste.at(i)->r,liste.at(i)->g,liste.at(i)->b);
			glVertex2f(PH.theta*(180/M_PI)+100,PH.rho+300); //(PH.rho/10)+100);

			/*On place ce point de hough dans une case du damier dont les cases font 10*10*/
			Accu[(int)((PH.theta*180/M_PI+100)/iDimCellule)][(int)((PH.rho+290)/iDimCellule)]++;
		}
	glEnd();
}
void TraceLigne(float theta, float rho)
{
	double angle_rad = (theta)*M_PI/180;
	
	double x1, x2, y1, y2;
	//X1
	x1 = -800;
	//Y1
	y1= (-cos(angle_rad) * x1 + rho)/(sin (angle_rad));
	//X2
	x2 = 1200;
	//Y2
	y2= (-cos(angle_rad) * x2 + rho)/(sin (angle_rad)); 

	glColor3f(0.5, 0.5, 0.5);
	glPointSize(2.0);
	glBegin(GL_LINES); 
	if(theta != 0)
	    { 
		glVertex2f(x1, y1); 
		glVertex2f(x2, y2); 
	} 
	else
	    { 
		glVertex2f(rho+200,0); 
		glVertex2f(rho+200,400); 
	}
	glEnd(); 
}
void RechercheAuto()
{
	int MaxI = 0, MaxJ = 0, Max = 0;
	
	for(int i = 0; i < 20; i++)
		for(int j = 0; j < 58; j++)
		{
			if(Accu[i][j] > Max)
			{
				Max = Accu[i][j];
				MaxI = i;
				MaxJ = j;
			}
		}

	/*pH.theta = (float)(MaxI*10-100);
	pH.rho = (float)(580-290-MaxJ*10);
	pH.rho = -pH.rho;*/
	//std::cout<<"MaxI : "<<MaxI<<"| MaxJ : "<<MaxJ<<std::endl;
	if (MaxI>0 && MaxJ>0)
	{
		MaxI = (float)(MaxI*iDimCellule-100);
		MaxJ = (float)(580-290-MaxJ*iDimCellule);
		MaxJ = - MaxJ;
	
		TraceLigne(MaxI, MaxJ);
	}
}
void affichage1(void)
{
        glClearColor(1.0,1.0,1.0,1.0);
        glClear (GL_COLOR_BUFFER_BIT);
	glColor3f(0.0,0.0,0.0);
	//Axe X
	glBegin(GL_LINES);
	glVertex2i(1,1);
	glVertex2i(1,400);
	glEnd();
	//Axe Y
	glBegin(GL_LINES);
	glVertex2i(1,1);
	glVertex2i(400,1);
	glEnd();

        glPointSize(10);
        glBegin(GL_POINTS);
        for(unsigned int i = 0;i<liste.size();i++)
	{
		glColor3f(liste.at(i)->r,liste.at(i)->g,liste.at(i)->b);
		glVertex2i(liste.at(i)->x,liste.at(i)->y);
	}
        glEnd();
	RechercheAuto();
	if(Click == true)
	  TraceLigne(x_souris, y_souris);
        glFlush();
}
void affichage2(void)
{
	glClearColor(1.0,1.0,1.0,1.0);
	glClear (GL_COLOR_BUFFER_BIT);
	glColor3f(0.0,0.0,0.0);
	//Axe X
	glBegin(GL_LINES);
	glVertex2i(0,300);
	glVertex2i(300,300);
	glEnd();
	//Axe Y
	glBegin(GL_LINES);
	glVertex2i(100,0);
	glVertex2i(100,600);
	glEnd();
	//Affiche la sinusoide
	onetomany();
	
	glFlush();
}
void affichage3(void)
{
	glClearColor(1.0,1.0,1.0,1.0);
	glClear (GL_COLOR_BUFFER_BIT);
	glColor3f(0.0,0.0,0.0);
        //Axe X
	glBegin(GL_LINES);
	glVertex2i(0,300);
	glVertex2i(300,300);
	glEnd();
	//Axe Y
	glBegin(GL_LINES);
	glVertex2i(100,0);
	glVertex2i(100,600);
	glEnd();
	
	manytoone();
	
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
		case 11 : 
                exit(0); /* On quitte */
                break;
        } 
}
void initGL(void)
{
        glPixelStorei(GL_UNPACK_ALIGNMENT,1);
}
void souris (int bouton, int etat, int x, int y)
{
        switch (bouton)
        {
                case GLUT_LEFT_BUTTON://gestion du bouton gauche
			if (etat==GLUT_DOWN && glutGetWindow() == I1)//Si le bouton gauche est cliqué
                        {
				float r,g,b;
				r = RandomRGB();
				g = RandomRGB();
				b = RandomRGB();
				r = r / 255;
				g = g / 255;
				b = b / 255;

                                liste.push_back(new point(x,400-y,r,g,b));//Création d'un nouveau point
                        }
			if (etat==GLUT_DOWN && (glutGetWindow() == I3 || glutGetWindow() == I2))//Tracer la Ligne
			{//std::cout<<"teste"<<std::endl;
				x_souris = x-100;
				y_souris = -y+300;
				std::cout<<"x = "<<x_souris<<std::endl;
				std::cout<<"y = "<<y_souris<<std::endl;
				Click = true;
				glutSetWindow(I1);
				glutPostRedisplay() ;
			}
			if (etat==GLUT_UP)//Effacer la Ligne
				Click = false;
                        break ;
                default:
                        break;
        }
	glutSetWindow(I1);
        glutPostRedisplay() ;
	glutSetWindow(I2);
	glutPostRedisplay() ;
	glutSetWindow(I3);
	glutPostRedisplay() ;
}
void clavier ( unsigned char key, int x, int y)
{
        switch (key)
        {
		case 'a' :
			std::cout<<"---------------------------"<<std::endl;
			for(unsigned int i = 0;i<liste.size();i++)
				std::cout<<"P("<<i<<") : ("<<liste.at(i)->x<<","<<liste.at(i)->y<<")"<<std::endl;
			std::cout<<"---------------------------"<<std::endl;
		break;
		case 'e' :
			while(liste.size() != 0)
				liste.erase(liste.begin());
			glutPostRedisplay();
		break;
		case '+' :
			iDimCellule ++;
			glutPostRedisplay();
		break;
		case '-' :
			iDimCellule --;
			glutPostRedisplay();
		break;
		case 27 : exit(0);break;
        	default:
                        break;
        }
}
int main(int argc,char **argv)
{
        glutInit(&argc,argv);
	initAccu();

        /*--------------------------Fenetre 1----------------------------*/
        glutInitWindowSize(400,400);
        glutInitWindowPosition(100,100);
        glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	I1 = glutCreateWindow("Tp1 Transfo de Hough");
        glutDisplayFunc(affichage1);
        glutReshapeFunc(redim);
        glutMouseFunc(souris) ;
        glutKeyboardFunc(clavier);
        initGL();
	/*--------------------------Fenetre 2----------------------------*/
	glutInitWindowSize(200,600);
	glutInitWindowPosition(525,150);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	I2 = glutCreateWindow("Espace de Hough 1");
	glutDisplayFunc(affichage2);
	glutReshapeFunc(redim);
	glutMouseFunc(souris) ;
	glutKeyboardFunc(clavier);
	initGL();
	/*--------------------------Fenetre 3----------------------------*/
	glutInitWindowSize(200,600);
	glutInitWindowPosition(750,150);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	I3 = glutCreateWindow("Espace de Hough 2");
	glutDisplayFunc(affichage3);
	glutReshapeFunc(redim);
	glutMouseFunc(souris) ;
	glutKeyboardFunc(clavier);
	initGL();

        glutMainLoop();
        return 0;
}
