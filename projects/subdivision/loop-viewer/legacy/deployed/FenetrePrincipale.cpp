#include "FenetrePrincipale.h"
#include <iostream>
#include <fstream>
#include "LoaderOBJ.h"

using namespace std;



Mesh* mesh = new Mesh();
/*
FenetrePrincipale::FenetrePrincipale()
{
	QMenu *menuFichier = menuBar()->addMenu("&Fichier");

	QAction *actionQuitter = new QAction("&Quitter", this);
	menuFichier->addAction(actionQuitter);

}
*/

FenetrePrincipale::FenetrePrincipale(/*Mesh *m*/)
{
	setFixedSize(500, 500);
	QMenu *menuFichier = menuBar()->addMenu("&Fichier");

	QAction *actionOuvrir = new QAction("&Ouvrir", this);
	menuFichier->addAction(actionOuvrir);

	QAction *actionQuitter = new QAction("&Quitter", this);
	menuFichier->addAction(actionQuitter);


	MaFenetre *zoneCentrale = new MaFenetre(mesh/*m*/);
	setCentralWidget(zoneCentrale);

	connect(actionOuvrir, SIGNAL(triggered()), this, SLOT(ouvrirDialogue()));

	connect(actionQuitter, SIGNAL(triggered()), qApp, SLOT(quit()));

	

}


void FenetrePrincipale::ouvrirDialogue()
{	
	QString fichier = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", QString(), "Maillage (*.obj)");

	std::string _fichier = fichier.toUtf8().constData();

	
	if (LoadOBJ(mesh, _fichier) == true)
		{
		cout << "test\n";
		}
}
