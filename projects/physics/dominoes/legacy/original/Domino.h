#ifndef DOMINO_H
#define DOMINO_H

#include "texture.h"
#include <ode/ode.h>

class Domino 
{
  private:
        float posX,posY,posZ; //sa position (coordonnées du centre de sa base)
        float dimX,dimY,dimZ; //ses dimensions (longueur selon x, largeur selon z, hauteur selon y) ;
        float orientation;    //son orientation en radian par rapport à l’axe vertical y ;
        Texture * texture;	  //un pointeur sur un objet de classe Texture (réutiliser la classe écrite en TP).
  public:
        dBodyID body;
        dGeomID geom;
        
        Domino();                                                                                                              //Constructeur d'initialisation à 0
        Domino(float _posX,float _posY,float _posZ,float _dimX,float _dimY,float _dimZ,float _orientation,Texture * _texture); //Constructeur par Défaut
        ~Domino();                                                                                                             //Destructeur
        void afficher();                                                                                                       //Affiche le Domino
		
        void DefinitPhysique(dWorldID world,dSpaceID space); //Ajoute  au domino  des  propriétés  physiques  grâce   à ODE
};

#endif
