#ifndef DEF_MAFENETRE
#define DEF_MAFENETRE

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QtGui/QLabel>
#include <QtGui>
#include <QGLWidget>
#include <QtOpenGL>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPoint>
#include <iostream>
#include <fstream>

#include "LoaderOBJ.h"


class MaFenetre : public QGLWidget
{
	Q_OBJECT

	public:
	MaFenetre(Mesh* mesh = 0);

	private:
        Mesh * mesh;
        Mesh * mesh2;
        Mesh * mesh3;
        Mesh * mesh4;
        Mesh * mesh5;
        Mesh * mesh6;

        //CAM
        QPoint lastPos;

        double translationX;
        double translationY;
        double translationZ;

        double rotationX;
        double rotationY;
        double rotationZ;

        double eloign;

        //Loop
        int Looptroop;


	public:
	MaFenetre();

	protected:
        Vert SommetBord (Vert v);
        void Loop (Mesh *m, Mesh *m2);
        void Repere();
        void AffichageVerts(Mesh * m);
        void AffichageNouveaux(Mesh * m);
        void AffichageHE(Mesh * m, int i);
        void AffichageFaces(Mesh * m);
        void Affichage(Mesh * _m);
        void initializeGL();
        void resizeGL(int w, int h);
        void paintGL();
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void VertVoisin (Mesh* _m);//Recherche les arètes voisines
        void HEPaire (Mesh* _m);//Recherche les arètes paires
        void VertNormal (Mesh * _m);//Calcul des normales
        int MeshType (Mesh * _m);//Définit le Type du Maillage, 0 : Quelconque, 1 : Triangulaire, 2 : Carré
};

#endif
