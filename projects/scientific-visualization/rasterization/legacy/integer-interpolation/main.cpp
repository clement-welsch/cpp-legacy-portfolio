#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
	   
#define HEIGHT 480	   
#define WIDTH 640
	   
class Point
{
	public:

	float x;
	float y;
	Point()
	{
	    x = 0;
	    y = 0;
	}
	Point(double _x, double _y)
	{
	    x= _x;
	    y= _y;
	}
	~Point() {}
};
vector<Point>  mPoints;
int OHeight = 0;

void redim (int width, int height)
{
	OHeight = height;
	glViewport(0,0,width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void displayAxys2D()	   
{
    glLineWidth(2);	   
    // axe des X
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
		glVertex2i(0, HEIGHT/2);
		glVertex2i(WIDTH, HEIGHT/2);
    glEnd();
    // axe des Y
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
		glVertex2i(WIDTH/2, 0);
		glVertex2i(WIDTH/2, HEIGHT);
    glEnd();
}
void tracer_pointMilieu(int x1, int y1, int x2, int y2)	   
{
    int x3 = x1 + (x2 - x1) / 2;
    int y3 = y1 + (y2 - y1) / 2;
    glPointSize(1.0);
    glColor3f(0.5,0.5,0.5);
    glBegin(GL_POINTS);
		glVertex2i(x3,y3);
    glEnd();
    if ( ( abs(x2 - x3) <= 1) && ( abs(y2 - y3) <= 1) )
        return;
    tracer_pointMilieu( x1,  y1,  x3,  y3);
    tracer_pointMilieu( x3,  y3,  x2,  y2);
}
static void display(void)	   
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    displayAxys2D();
    glPushMatrix();
    for (unsigned int i = 0; i < mPoints.size() ; i++)
    {
        if (i == mPoints.size()-1 || i == 0)
        {
            glPointSize(5);
            glBegin(GL_POINTS);
            glColor3f(1,0,0);
        }
        else
        {
            glPointSize(3);
            glBegin(GL_POINTS);
            glColor3f(0,0,0);
        }
        glVertex2i(mPoints[i].x,mPoints[i].y);
        glEnd();
    }  
    for (unsigned int i = 0; i < mPoints.size()-1 && mPoints.size()> 1 ; i++)
    {
        if ( mPoints.size() > 1 && mPoints.size() != i)
        {
		tracer_pointMilieu(mPoints[i].x, mPoints[i].y, mPoints[i+1].x, mPoints[i+1].y);
        }
    }
    glPopMatrix();
    glutSwapBuffers();
}   
void init()	   
{
    glClearColor(1.0f,1.0f, 1.0f,1);
    glEnable(GL_FRONT_AND_BACK);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();
    gluPerspective(75, 1.33, 0.1, 2000.0);
}
void mouseClick(int button, int state, int x, int y)
{
	Point point;
    if (state == GLUT_DOWN)	   
    {
	   Point point;
	   if (state == GLUT_DOWN)
	   {  
			glutPostRedisplay();	   
			point.x = x;
			point.y = OHeight - y;
			mPoints.push_back( point );	   
	   }   
    }
	   
}
void idle()
{
    glutPostRedisplay();
}
void key(unsigned char key, int x, int y)	   
{
    switch (key)
    {
    case 27 :
    case 'q':
    case 'Q':	
      exit(0);	   
    break;
    }
    glutPostRedisplay();
}
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(WIDTH,HEIGHT);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Interpolation d'entiers");
    glutReshapeFunc(redim);
    glutMouseFunc(mouseClick);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    init();
    glutMainLoop();
    return EXIT_SUCCESS;
}
