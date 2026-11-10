#ifndef CHUTEDOMINO_H
#define CHUTEDOMINO_H

#include <vector>
#include "texture.h"
#include "Domino.h"

class ChuteDomino 
{


  public:
        std::vector<Domino> listDomino;   // Liste d'objets Domino
        std::vector<Texture> listTexture; //Liste d'objets Texture (réutiliser la classe Texture écrite en TP).
 
        ChuteDomino();                            //Constructeur
        ~ChuteDomino();                           //Destructeur
        void charger_textures(char * nomfichier); //Pour chaque fichier TGA,  on crée un objet de classe Texture et  on l'ajoute à la liste de textures
        void charger_dominos(char * nomfichier);  //Pour   chaque  domino   lu  dans   le   fichier,   on créer  un objet  de   classe  Domino.  
												  //On transmet   au   constructeur   de   l’objet  Domino  un   pointeur   sur   la   texture correspondant au numéro. On Ajoute cet objet à la liste de dominos.
        void afficher(int i);                     //Affiche l’ensemble des dominos.
		
        void DefinitPhysique(dWorldID world,dSpaceID space); //fait appel à la méthode DefinitPhysique() de chacun des dominos de la liste.
};

#endif

