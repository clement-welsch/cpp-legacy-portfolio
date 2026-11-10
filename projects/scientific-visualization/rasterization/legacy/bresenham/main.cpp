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
bool bTracerNaif = true;
bool bTracerBresenham = false;
int OHeight = 0;
int tx = 0,ty = 0,tz = 0;

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
void tracer_bresenham(int x1, int y1, int x2, int y2)
{
    bool bInverseX =false;
    bool bInverseY =false;
    int x, y;
    int tmpX, tmpY;

    if ((x2 -x1) < 0)        bInverseX = true;
    if ((y2 -y1) < 0)        bInverseY = true;

    if (bInverseX)
            x2 += 2*abs(x2-x1);

    if (bInverseY)
            y2 += 2*abs(y2-y1);
    x = x1;
    y = y1;
    int d = 2*(y2 - y1) - (x2-x1);
    int inc1 = 2*(y2-y1);
    int inc2 = 2*(y2-y1) - 2.*(x2-x1);
    glColor3f(0.2f, 0.8f, 0.2f);
    glPointSize(1.0);
    glBegin(GL_POINTS);
    // cas normal < 45
    if (abs(x2 - x1) > abs(y2 - y1))
    {
        x = x1;
        y = y1;
        d = 2*(y2 - y1) - (x2-x1);
        inc1 = 2*(y2-y1);
        inc2 = 2*(y2-y1) - 2.*(x2-x1);
        tmpX = x;
        tmpY = y;
        if (bInverseX)
		tmpX -= 2*abs(tmpX-x1);
        if (bInverseY)
		tmpY -= 2*abs(tmpY-y1);
        glVertex2i(x,y);
        while(x < x2)
        {
            x++;
            if (d >=0)
            {
                y++;
                d+=inc2;
            }
            else
                d+=inc1;
            tmpX = x;
            tmpY = y;
            if (bInverseX)
                tmpX -= 2*abs(tmpX-x1);
            if (bInverseY)
                tmpY -= 2*abs(tmpY-y1);
            glVertex2i(tmpX,tmpY);
	}
    }
    // cas > 45
    else if (abs(x2 - x1) <= abs(y2 - y1))
    {
        d = 2*(x2 - x1) - (y2-y1);
        inc1 = 2*(x2-x1);
        inc2 = 2*(x2-x1) - 2.*(y2-y1);
        tmpX = x;
        tmpY = y;
        if (bInverseX)
            tmpX -= 2*abs(tmpX-x1);
        if (bInverseY)
            tmpY -= 2*abs(tmpY-y1);
        glVertex2i(tmpX,tmpY);
        while(y < y2)
        {
            y++;
            if (d >=0)
            {
                x++;
                d+=inc2;
            }
            else
		d+=inc1;
            tmpX = x;
            tmpY = y;
            if (bInverseX)
                tmpX -= 2*abs(tmpX-x1);
            if (bInverseY)
                tmpY -= 2*abs(tmpY-y1);
            glVertex2i(tmpX,tmpY);
        }
    }
    glEnd();
}
void tracer_naif(int x1, int y1, int x2, int y2)
{
    double a,b;
    double x1tmp = x1;
    double x2tmp = x2;
    double y1tmp = y1;
    double y2tmp = y2;
    glPointSize(1.0);
    if (x1 > x2)
    {
        x1tmp = x2;
        x2tmp = x1;
    }
    if (y1 > y2)
    {
        y1tmp = y2;
        y2tmp = y1;
    }
    glColor3f(0.8f, 0.2f, 0.2f);
    if (x2 == x1)
    {
        glBegin(GL_POINTS);
        for (double i = y1tmp; i < y2tmp; i++)
            glVertex2i( x2 , i );
        glEnd();
        return;
    }
    a = (double)(y2 - y1) / (double)(x2 - x1);
    b = y2 - a * x2;
    if ( abs(x1tmp - x2tmp) > abs(y1tmp - y2tmp))
    {
        glBegin(GL_POINTS);
        for (double i = x1tmp; i < x2tmp; i++)
            glVertex2i( i , a*i+b );
        glEnd();
    }
    else
    {
        glBegin(GL_POINTS);
        for (double i = y1tmp; i < y2tmp; i++)
            glVertex2i( (i-b)/a , i );
        glEnd();
    }
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
            glPointSize(10);
            glBegin(GL_POINTS);
            glColor3f(1.0, 0.0, 0.0);
        }
        else
        {
            glPointSize(10);
            glBegin(GL_POINTS);
            glColor3f(0.2, 0.5, 0.8);
        }
        glVertex2i(mPoints[i].x,mPoints[i].y);
        glEnd();
    }
    for (unsigned int i = 0; i < mPoints.size()-1 && mPoints.size()> 1 ; i++)
    {
        if ( mPoints.size() > 1 && mPoints.size() != i)  
        {
            if (bTracerNaif)
                tracer_naif(mPoints[i].x, mPoints[i].y, mPoints[i+1].x, mPoints[i+1].y);
            else if(bTracerBresenham)
                tracer_bresenham(mPoints[i].x, mPoints[i].y, mPoints[i+1].x, mPoints[i+1].y);
        }
    }
    glPopMatrix();
    glutSwapBuffers();
}
void displayHelp()
{
    cout << " ------------------------------------------- " << endl;
    cout << "  b   : Trace Bresenham                      " << endl;
    cout << "  n   : Trace naif                           " << endl;
    cout << "  e   : Supprimer points                     " << endl;
    cout << "  h   : Display help                         " << endl;
    cout << "  Esc : Exit                                 " << endl;
    cout << " ------------------------------------------- " << endl;
}
void init()
{
    glClearColor(1.0, 1.0, 1.0, 1);
    glEnable(GL_FRONT_AND_BACK);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();
    gluPerspective(75, 1.0, 0.1, 2000.0);
    displayHelp();
}
void mouseClick(int button, int state, int x, int y)
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
		case 'h':
			displayHelp();
		break;
		case 'n':	
			if (bTracerBresenham)
			{
				bTracerBresenham = false;
				bTracerNaif = true;
			}
		break;	
		case 'b':
			if (bTracerNaif)
			{
				bTracerNaif = false;
				bTracerBresenham = true;
			}
		break;
		case 'e':
			mPoints.clear();
		break;
	}
	glutPostRedisplay();
}
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(WIDTH,HEIGHT);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Tracer de segments naif/bresenham");
    glutReshapeFunc(redim);
    glutMouseFunc(mouseClick);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    init();
    glutMainLoop();
    return EXIT_SUCCESS;
}
