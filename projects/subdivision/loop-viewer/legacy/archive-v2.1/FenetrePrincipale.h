#ifndef HEADER_FENPRINCIPALE
#define HEADER_FENPRINCIPALE

#include <QtGui>
#include "MaFenetre.h"
#include "LoaderOBJ.h"


class FenetrePrincipale : public QMainWindow
{

	Q_OBJECT

	public:
		FenetrePrincipale();

/*	public:
		FenetrePrincipale(Mesh *m = 0);*/

	public slots:
	void ouvrirDialogue();


};


#endif

