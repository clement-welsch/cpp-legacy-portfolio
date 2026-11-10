#include <QApplication>
#include "MaFenetre.h"
#include <iostream>
#include <fstream>
#include "LoaderOBJ.h"
#include "FenetrePrincipale.h"

using namespace std;
 

//Mesh* mesh = new Mesh();


 
int main(int argc, char *argv[])
	{
	QApplication app(argc, argv);

	/*if (argc > 1)
		{
		cout << "Chargement du maillage :\n";

		if (LoadOBJ(mesh, "cube_tri.obj") == false)
			{
			std::cout<<"Fail."<<std::endl;

			exit(0);
			}
		}
	else
		{
		cout << "Pas de maillage pas de programme !\n";
		exit(0);
		}*/


	FenetrePrincipale fenetre;

	fenetre.show();

	return app.exec();
	}
