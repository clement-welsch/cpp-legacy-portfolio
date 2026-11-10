#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <ode/ode.h>
#include <glut.h>
#include "ChuteDomino.h"
#include "texture.h"

#define PI      3.14
#define WIDTH   800                                                                             // Largeur de la fenêtre OpenGL
#define HEIGHT  600                                                                             // Hauteur de la fenêtre OpenGl
#define KEY_ESC 27                                                                              // Code ASCII de la touche Echap

float   xpos = 0, ypos = -5, zpos = -50;                                        // Position de la caméra
int     angle_x = 45, angle_y = 45;                                             // Orientation de la caméra
int     mouse_x = 0, mouse_y = 0;                                               // Position de la souris

// Paramètres de la source de lumière 0 (directionnelle)
GLfloat Light0Position[]= { 4.0f, 4.0f, 2.0f, 0.0f };
GLfloat Light0Ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat Light0Diffuse[] = { 0.9f, 0.9f, 0.9f, 1.0f };
GLfloat Light0Specular[]= { 0.3f, 0.3f, 0.3f, 1.0f };

float   dt=0.02;                                                // Contiendra le temps écoulé en secondes entre deux affichages

static dWorldID world;                                          // Le monde
static dSpaceID space;                                          // L'espace des objets, pour accélérer les détections de collisions
static dJointGroupID contactgroup;                              // Liste des jointures de contact temporaires

static dGeomID geom_sol;                                        // Geom pour le sol

ChuteDomino domTest;


///////////////////////////////////////////////////////////////////////////////
// Intitialisation de certains paramètres d'OpenGL.
//-----------------------------------------------------------------------------
// Paramètres :
//    _
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
GLvoid initGL()
{
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);                                           // Couleur servant à effacer la fenêtre
        glShadeModel(GL_SMOOTH);                                                        // Modèle d'ombrage : lissage de Gouraud
        glEnable(GL_CULL_FACE);                                                         // Ne traite pas les faces cachées
        glEnable(GL_DEPTH_TEST);                                                        // Active le Z-Buffer
        glDepthFunc(GL_LEQUAL);                                                         // Mode de fonctionnement du Z-Buffer
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);      // Active la correction de perspective (pour ombrage, texture, ...)
        glLightfv(GL_LIGHT0, GL_AMBIENT, Light0Ambient);        // Couleur ambiante de la source de lumière 0 (directionnelle)
        glLightfv(GL_LIGHT0, GL_DIFFUSE, Light0Diffuse);        // Couleur diffuse de la source de lumière 0
        glLightfv(GL_LIGHT0, GL_SPECULAR,Light0Specular);       // Couleur spéculaire de la source de lumière 0
        glLightfv(GL_LIGHT0, GL_POSITION,Light0Position);       // Direction de la source de lumière 0
        glEnable(GL_LIGHT0);                                                            // Activation de la source de lumière 0
        glEnable(GL_LIGHTING);                                                          // Activation de l'éclairage général
}
///////////////////////////////////////////////////////////////////////////////
// Affiche le sol de la scène.
//-----------------------------------------------------------------------------
// Paramètres :
//    _
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
void affiche_sol()
{
        glNormal3f( 0.0f, 1.0f, 0.0f);                                          // Normale du quadrilatère

        glBegin(GL_QUADS);                                                      // Affichage d'un quadrilatère
                glVertex3f(-500, -5, -500);
                glVertex3f(-500, -5,  500);
                glVertex3f( 500, -5,  500);
                glVertex3f( 500, -5, -500);
        glEnd();
}
///////////////////////////////////////////////////////////////////////////////
// Transformation de matrice (ODE -> OpenGL) pour créer la matrice de
// transformation afin de positionner un objet dans l'espace.
//-----------------------------------------------------------------------------
// Paramètres :
//    pos (in) : matrice de translation de l'objet.
//    R   (in) : matrice de rotation de l'objet.
//
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
void set_transf(const dReal *pos,const dReal *R)
{
        float local_matrix[16];

        local_matrix[0] = R[0];
        local_matrix[1] = R[4];
        local_matrix[2] = R[8];
        local_matrix[3] = 0;
        local_matrix[4] = R[1];
        local_matrix[5] = R[5];
        local_matrix[6] = R[9];
        local_matrix[7] = 0;
        local_matrix[8] = R[2];
        local_matrix[9] = R[6];
        local_matrix[10] = R[10];
        local_matrix[11] = 0;
        local_matrix[12] = pos[0];
        local_matrix[13] = pos[1];
        local_matrix[14] = pos[2];
        local_matrix[15] = 1;

        glMultMatrixf(local_matrix);
}
///////////////////////////////////////////////////////////////////////////////
// Fonction de callback de ODE, appellée pour tester les collisions entre
// chaque paire d'objets du monde.
//-----------------------------------------------------------------------------
// Paramètres :
//    data  (in) : données optionnelles
//    o1    (in) : le premier objet de la paire à tester.
//    o2    (in) : le second objet de la paire à tester.
//
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
static void ode_callback(void *data, dGeomID o1, dGeomID o2)
{
        int i;

        // On quitte sans faire quoi que ce soit si les body de ces deux objets
        // sont connectés par un joint
        dBodyID b1 = dGeomGetBody(o1);
        dBodyID b2 = dGeomGetBody(o2);
        if (b1 && b2 && dAreConnected (b1,b2))
            return;

        // Maximum de trois points de contacts (plus ce nombre est important,
        // plus les calculs sont précis mais lents)
        dContact contact[3];

        // Pour chaque point de contact trouvé, on défini les propriétés du contact
        // (rebondissement, friction)
        for (i=0; i<3; i++)
        {
            contact[i].surface.mode = dContactApprox1;              // Mode de contact (voir section 7.3.7. Contact)
            contact[i].surface.mu = 0.2;                                    // Coefficient de friction
        }

        // Pour chaque point de contact trouvé, création d'une jointure temporaire
        // entre les deux objets pour calculer une réaction à la collision.
        if (int numc = dCollide (o1,o2,3,&contact[0].geom,sizeof(dContact)))
        {
                for (i=0; i<numc; i++)
                {
                    dJointID c = dJointCreateContact (world,contactgroup,contact+i);
                    dJointAttach (c,b1,b2);
                }
        }
}
///////////////////////////////////////////////////////////////////////////////
// Affiche les différents objets de la scène.
//-----------------------------------------------------------------------------
// Paramètres :
//    _
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
void affiche_scene()
{
        // On détermine les collisions entre toutes les paires d'objets du monde
        dSpaceCollide(space,0,&ode_callback);

        // On avance la simulation du pas de temps dt qui s'est écoulé entre deux affichages
        dWorldQuickStep (world, dt);

        // On vide la liste des jointures de contact temporaires
        dJointGroupEmpty (contactgroup);

        // Affiche le sol
        affiche_sol();
       
        // On affiche les objets de la scène (ici des dominos) en utilisant comme
        // positions et orientations les valeurs calculées par la simulation
        for( int i=0; i<domTest.listDomino.size(); i++ )
        {
                glPushMatrix();
                        set_transf(dGeomGetPosition(domTest.listDomino.at(i).geom),dGeomGetRotation(domTest.listDomino.at(i).geom));
                        domTest.afficher(i);
                glPopMatrix();
        }
       
        // Affiche la scène à l'écran (affichage en double buffer)
        glutSwapBuffers();                                                      
}
///////////////////////////////////////////////////////////////////////////////
// Call-back : affichage dans la fenêtre.
//-----------------------------------------------------------------------------
// Paramètres :
//    _
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_display()
{
        // On calcule le temps écoulé en secondes entre deux affichages, c'est à dire
        // entre deux appels à la fonction callback_display() :
        dt = 0.001;

        // On efface les pixels de l'image (color buffer) et le Z-Buffer (depth buffer).
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       
        // On initialise la matrice de vue avec la matrice identité.
        glLoadIdentity();

        // On applique une translation et une rotation à la scène pour simuler
        // un déplacement de caméra.
        glTranslatef(xpos,ypos,zpos);
        glRotatef(angle_x, 1,0,0);
        glRotatef(angle_y, 0,1,0);

        // On re-spécifie la position des sources de lumière avec pour qu'elles
        // soient aussi affectées par la translation et la rotation qu'on vient
        // d'appliquer à la scène.
        glLightfv(GL_LIGHT0, GL_POSITION, Light0Position);

        // On affiche la scène.
        affiche_scene();

        // On force OpenGL à afficher avant de passer à la suite.
        glFlush();
}
///////////////////////////////////////////////////////////////////////////////
// Call-back : redimensionnement de la fenêtre.
//-----------------------------------------------------------------------------
// Paramètres :
//    width, height (in) : nouvelles largeur et hauteur de la fenêtre.
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_reshape(GLsizei width, GLsizei height)
{
        glViewport(0, 0, width, height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,1.0f,5000.0f);

        glMatrixMode(GL_MODELVIEW);
}
///////////////////////////////////////////////////////////////////////////////
// Call-back : gestion des événements clavier.
//-----------------------------------------------------------------------------
// Paramètres :
//    key (in) : code ascii de la touche utilisée.
//    x,y (in) : coordonnées de la souris.
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_keyboard(unsigned char key, int x, int y)
{
        switch (key)
        {
                case KEY_ESC:                                           // 'ECHAP' :
                        exit(1);                                                // on quitte le programme
                        break;
        }
}
///////////////////////////////////////////////////////////////////////////////
// Call-back : gestion des touches speciales du clavier.
//-----------------------------------------------------------------------------
// Paramètres :
//    key (in) : code ascii de la touche utilisée.
//    x,y (in) : coordonnées de la souris.
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_special(int key, int x, int y)
{
        float speed = 1.0f;                                             // Vitesse de déplacement de la caméra

        switch (key)
        {
                case GLUT_KEY_UP:                                       // Flèche vers le haut :
                        zpos += speed;                                  // on déplace la caméra selon z-
                        glutPostRedisplay();                    // et on demande le réaffichage.
                        break;

                case GLUT_KEY_DOWN:                                     // Flèche vers le bas :
                        zpos -= speed;                                  // on déplace la caméra selon z+
                        glutPostRedisplay();                    // et on demande le réaffichage.
                        break;

                case GLUT_KEY_LEFT:                                     // Flèche vers la gauche :
                        xpos += speed;                                  // on déplace la caméra selon x-
                        glutPostRedisplay();                    // et on demande le réaffichage.
                        break;

                case GLUT_KEY_RIGHT:                            // Flèche vers la droite :
                        xpos -= speed;                                  // on déplace la caméra selon x+
                        glutPostRedisplay();                    // et on demande le réaffichage.
                        break;

                case GLUT_KEY_PAGE_UP:                          // Page Up :
                        ypos += speed;
                        glutPostRedisplay();                    // et on demande le réaffichage.
                        break;

                case GLUT_KEY_PAGE_DOWN:                        // Page Down :
                        ypos -= speed;
                        glutPostRedisplay();                    // et on demande le réaffichage.
                        break;
        }
}
///////////////////////////////////////////////////////////////////////////////
// Call-back : gestion de clics souris.
//-----------------------------------------------------------------------------
// Paramètres :
//    button (in) : code du bouton utilisé.
//    state  (in) : état du bouton.
//    x,y    (in) : coordonnées de la souris.
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_mouse(int button, int state, int x, int y)
{
        if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
        {
                mouse_x = x;
                mouse_y = y;
        }
}
///////////////////////////////////////////////////////////////////////////////
// Call-back : gestion des déplacements de la souris.
//-----------------------------------------------------------------------------
// Paramètres :
//    x,y    (in) : coordonnées de la souris.
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_motion(int x, int y)
{
        angle_x += y - mouse_y;                                         // Modifie la direction de vue de la caméra
        angle_y += x - mouse_x;                                         // en fonction de la position de la souris
        mouse_x = x;
        mouse_y = y;
        glutPostRedisplay();                                            // Demande le réaffichage
}
///////////////////////////////////////////////////////////////////////////////
// Initialise la scène.
//-----------------------------------------------------------------------------
// Paramètres :
//    _
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
void initialise_scene()
{
        dMass mass;

        // Initialisation de ODE
        dInitODE();

        // Création du monde
        world = dWorldCreate();

        // Création de l'espace pour accélérer les calculs de collision
        space = dHashSpaceCreate(0);

        // Création de la liste de jointures de contact temporaires
        contactgroup = dJointGroupCreate (0);

        // On fixe le vecteur de gravité du monde
        dWorldSetGravity (world, 0, -9.81, 0);

        // Création d'un plan correspondant au sol. Ce plan fait partie de l'environnement statique,
        // il ne peut pas être bougé par la simulation physique.
        // (voir dans la documentation de ODE : 10.7.3. Plane Class)
        geom_sol = dCreatePlane (space,0,1,0,-5);
       
        domTest.charger_dominos((char*)"dominos.txt");
        domTest.DefinitPhysique(world,space);
        dBodyAddForce (domTest.listDomino.at(0).body, 0, 0, 50000); // Applique  une   force   au   premier   domino   de   la   liste   afin   de   le   faire   chuter
																	//Et d’entraîner tous les autres dominos dans sa chute.
       
}
///////////////////////////////////////////////////////////////////////////////
// Effectue les désallocations pour terminer proprement le programme.
//-----------------------------------------------------------------------------
// Paramètres :
//    _
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
void termine_scene()
{
        dSpaceDestroy (space);
        dWorldDestroy ( world );
        dCloseODE();
}
///////////////////////////////////////////////////////////////////////////////
// Call-back : déclenche l'affichage même en l'absence d'évènements souris ou
//             clavier
//-----------------------------------------------------------------------------
// Paramètres :
//    _
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_idle()
{
        glutPostRedisplay();
}
///////////////////////////////////////////////////////////////////////////////
// Fonction principale du programme.
//-----------------------------------------------------------------------------
// Paramètres :
//    argc (in) : nombre d'arguments de la ligne de commande.
//    argv (in) : tableau de chaînes de caractères contenant les arguments.
// Retour :
//    Un entier contenant le code de retour du programme.
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
        // Initialisation de paramètres de Glut
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
        glutInitWindowSize(WIDTH, HEIGHT);
        glutInitWindowPosition(0, 0);
        glutCreateWindow("Animation physique");

        // Intitialisation de paramètres d'OpenGL
        initGL();

        // initialise la scène
        initialise_scene();

        // Déclaration des fonctions de call-back.
        glutDisplayFunc(&callback_display);
        glutReshapeFunc(&callback_reshape);
        glutKeyboardFunc(&callback_keyboard);
        glutSpecialFunc(&callback_special);
        glutMouseFunc(&callback_mouse);
        glutMotionFunc(&callback_motion);
        glutIdleFunc(&callback_idle);

        // Boucle principale de Glut.
        glutMainLoop();

        // Effectue les désallocations pour terminer proprement le programme.
        termine_scene();

        // Code de retour
        return 1;
}
