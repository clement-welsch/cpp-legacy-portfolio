#include <string>
#include <vector>

#ifndef LOADEROBJ_H
#define LOADEROBJ_H
class Vert;
class Face;
class Halfedge;

class Vert
{
    public :
        int id;
        double pos [3];
        double normal [3];
        std::vector < Halfedge * > voisin;
        Vert * nouveau; // catmull
};
class Face
{
    public :
        int id;
        Halfedge * he;
        Vert * nouveau; // catmull
};
class Halfedge
{
    public :
        int id;
        Halfedge * suivante;
        Halfedge * precedente;
        Halfedge * paire;
        Face * f;
        Vert * tete;
        Vert * queue;
        Vert * nouveau ;
};
class Mesh
{
    public :
	int 	id;

        int     meshType;
	
	int	nbVerts;
	int	nbEdges;
	int	nbFaces;
	
    std::vector<Vert*> vVerts;
        std::vector<Halfedge*> vHEdges;
    std::vector<Face*> vFaces;
};

bool LoadOBJ (Mesh* m, std::string filename);//Charge l'OBJ
void VertVoisin (Mesh* _m);//Recherche les arètes voisines
void HEPaire (Mesh* _m);//Recherche les arètes paires
void VertNormal (Mesh * _m);//Calcul des normales
int MeshType (Mesh * _m);//Définit le Type du Maillage, 0 : Quelconque, 1 : Triangulaire, 2 : Carré
void Affichage(Mesh * _m);
#endif
