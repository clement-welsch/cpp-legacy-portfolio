#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>

#include "LoaderOBJ.h"

unsigned int iNbVerts = 0;
unsigned int iNbEdges = 0;
unsigned int iNbFaces = 0;

bool LoadOBJ (Mesh* m, std::string filename)
{
m->meshType = -1;
bool bTeste = false;
//Ouverture du fichier
std::ifstream fichier(filename.c_str(), std::ifstream::in);
std::cout<<std::endl;
if ( !fichier )
std::cout << "------fichier inexistant------"<<std::endl;
else
{
    std::cout << "------fichier existant------"<<std::endl;

    std::string ligne;
    std::string res="";
    char c;
    //Parcours du fichier OBJ
    while(getline(fichier, ligne))  // tant que l'on peut mettre du contenu dans "ligne"
    {
        if (ligne.size()==0)
        {}
        else if (ligne.at(0) == 'v')//Vertice
        {
            int j = 0;
            Vert * v = new Vert();

            v->id = iNbVerts;//ID Vert
            for(unsigned int i = 2;i < ligne.size();i++)//Concaténation de caractères
            {
                c = ligne.at(i);

                if ((c == ' ')||(i ==ligne.size()-1))//Enregistrement de X puis Y puis Z
                {
                    v->pos[j] = atof (res.c_str()); //X Y Z
                    res="";
                    j++;
                }
                else
                res = res  + c;
            }
            m->vVerts.push_back(v);
            res="";
            iNbVerts++;
        }
        else if(ligne.at(0) == 'f')//Face & Edge
        {
            std::vector<int> p;
            unsigned int ires = 0;
            Face* f = new Face();
            f->id = iNbFaces;//ID Face
            f->he = NULL;
            f->nouveau = NULL;
            m->vFaces.push_back(f);
            iNbFaces++;
            int tour = 0;
            //Récupération des Sommets d'une face
            for(unsigned int i = 2; i < ligne.size(); i++)
            {
                c = ligne.at(i);
                if (c == '/')
                {
                    while(c != ' ')
                    {
                        i++;
                        c = ligne.at(i);
                    }
                }
                if ((c == ' ')||(i ==ligne.size()-1))
                {
                    if (res.size() == 0)
                        res = c;
                    if ((i ==ligne.size()-1)&& (tour>1))
                        res = res  + c;
                    tour = 0;
                    ires = atoi (res.c_str());
                    ires -= 1;
                    p.push_back(ires);
                    res="";
                }
                else
                    res = res  + c;
                tour++;
            }
            std::vector<Halfedge*> vHE;
            unsigned int j;
            unsigned int iTempiNbEdges = iNbEdges;
            //Création du vector des HE de la Face
            for(unsigned int i = 0; i < p.size(); i++)
            {
                if(i==p.size()-1)
                    j = 0;
                else
                    j=i+1;
                Halfedge * he = new Halfedge();
                he->id = iNbEdges;
                he->queue = m->vVerts.at(p.at(i));
                he->tete = m->vVerts.at(p.at(j));
                he->nouveau = NULL;
                he->paire = NULL;
                he->f = m->vFaces.at(iNbFaces-1);
                vHE.push_back(he);
                iNbEdges++;
            }
            //HE précédente et suivante
            for(unsigned int i = 0, j = vHE.size() - 1; i < vHE.size(); j = i, i++)
            {
                vHE.at(i)->precedente = vHE.at(j);
                vHE.at(j)->suivante   = vHE.at(i);
            }
            m->vHEdges.reserve(m->vHEdges.size() + vHE.size()); // allocation de l'espace
            m->vHEdges.insert(m->vHEdges.end(), vHE.begin(), vHE.end()); // copie de vHE dans m->vHEdges
            m->vFaces.at(iNbFaces-1)->he = m->vHEdges.at(iTempiNbEdges);
            for(unsigned int i = 0; i < vHE.size(); i++)
            {
                m->vHEdges.at(iTempiNbEdges)->precedente = m->vHEdges.at(vHE.at(i)->precedente->id);
                m->vHEdges.at(iTempiNbEdges)->suivante = m->vHEdges.at(vHE.at(i)->suivante->id);
                iTempiNbEdges++;
            }
            p.clear();
            vHE.clear();
        }
    }
    fichier.close();
    //Recherche des Voisins
    VertVoisin(m);
    std::cout << "-_-Voisin-_-"<<std::endl;
    //Recherche des Paires
    HEPaire (m);
    std::cout << "-_-Paire-_-"<<std::endl;
    //Calcule des normales
    VertNormal (m);
    std::cout << "-_-Normal-_-"<<std::endl;
    m->meshType = MeshType(m);
    std::cout << "-_-Type-_-"<<std::endl;
    m->nbVerts = iNbVerts;
    m->nbEdges = iNbEdges;
    m->nbFaces = iNbFaces;
    std::cout << "iNbVerts ("<<m->nbVerts<<")"<<std::endl;
    std::cout << "iNbEdges ("<<m->nbEdges<<")"<<std::endl;
    std::cout << "iNbFaces ("<<m->nbFaces<<")"<<std::endl;
    //Affichage (m);
    bTeste = true;
    }
    std::cout << "------FIN------"<<std::endl;
    //Ajouter les Edeges pour chaque Vertice
    return bTeste;
}
void VertVoisin (Mesh* _m)
{
    unsigned int iVsize = _m->vVerts.size();//Taille du vector Vert
    unsigned int iHEsize = _m->vHEdges.size();//Taille du vector HE
    unsigned int iVertId;//Id du sommet/Vert
    unsigned int iHETete;//Id de la tête de l'arète
    unsigned int iHEQueue;//Id de la queue de l'arète

    for (unsigned int i = 0; i < iVsize; i++)
    {
        iVertId = _m->vVerts.at(i)->id;
        for (unsigned int j = 0; j < iHEsize; j++)
        {
            iHETete =  _m->vHEdges.at(j)->tete->id;
            iHEQueue = _m->vHEdges.at(j)->queue->id;
            if((iVertId == iHETete)||(iVertId == iHEQueue))
                _m->vVerts.at(i)->voisin.push_back (_m->vHEdges.at(j));
        }
    }
}
void HEPaire(Mesh * _m)
{
    unsigned int iHEsize = _m->vHEdges.size(); //Taille du vector HE
    unsigned int iFsize = _m->vFaces.size(); //Taille du vector Face
    unsigned int iHETete;//Id de la tête de la première arète
    unsigned int iHEQueue;//Id de la queue de la première arète
    unsigned int iHEID;//Id de la première arète
    unsigned int iHE2Tete;//Id de la tête de la seconde arète
    unsigned int iHE2Queue;//Id de la queue de la seconde arète
    unsigned int iHE2ID;//Id de la seconde arète
    unsigned int iFHE;//Id de l'aèrete lié à la face
    unsigned int iVoisinSize; //Taille du Vector voisin
    Halfedge * he = new Halfedge ();
    bool bTest = false;
    for (unsigned int i = 0; i < iHEsize; i++)
        _m->vHEdges.at(i)->paire = NULL;
    for (unsigned int i = 0; i < iHEsize; i++)
    {
        if (_m->vHEdges.at(i)->paire == NULL)
        {
            iHEID = _m->vHEdges.at(i)->id;
            iHETete = _m->vHEdges.at(i)->tete->id;
            iHEQueue = _m->vHEdges.at(i)->queue->id;
            iVoisinSize = _m->vHEdges.at(i)->tete->voisin.size();
            for (unsigned int j = 0; j < iVoisinSize; j++)
            {
                iHE2ID = _m->vHEdges.at(i)->tete->voisin.at(j)->id;
                iHE2Tete = _m->vHEdges.at(i)->tete->voisin.at(j)->tete->id;
                iHE2Queue = _m->vHEdges.at(i)->tete->voisin.at(j)->queue->id;
                if ((iHETete == iHE2Queue)&&(iHEQueue == iHE2Tete))
                {
                    bTest = true;
                    _m->vHEdges.at(i)->paire = _m->vHEdges.at(iHE2ID);
                    _m->vHEdges.at(iHE2ID)->paire = _m->vHEdges.at(i);
                    //On remplit les arètes paires de chaque face
                    for (unsigned int k = 0; k < iFsize;k++)
                    {
                        iFHE = _m->vFaces.at(k)->he->id;
                        if (iFHE == iHEID)
                            _m->vFaces.at(k)->he->paire = _m->vHEdges.at(iHE2ID);
                        else if (iFHE == iHE2ID)
                            _m->vFaces.at(k)->he->paire = _m->vHEdges.at(i);
                    }
                }
            }
            if (bTest == false)
            {
                iVoisinSize = _m->vHEdges.at(i)->queue->voisin.size();
                for (unsigned int j = 0; j < iVoisinSize; j++)
                {
                    iHE2ID = _m->vHEdges.at(i)->queue->voisin.at(j)->id;
                    iHE2Tete = _m->vHEdges.at(i)->queue->voisin.at(j)->tete->id;
                    iHE2Queue = _m->vHEdges.at(i)->queue->voisin.at(j)->queue->id;
                    if ((iHETete == iHE2Queue)&&(iHEQueue == iHE2Tete))
                    {
                        bTest = true;
                        _m->vHEdges.at(i)->paire = _m->vHEdges.at(iHE2ID);
                        _m->vHEdges.at(iHE2ID)->paire = _m->vHEdges.at(i);
                        //On remplit les arètes paires de chaque face
                        for (unsigned int k = 0; k < iFsize;k++)
                        {
                            iFHE = _m->vFaces.at(k)->he->id;
                            if (iFHE == iHEID)
                                _m->vFaces.at(k)->he->paire = _m->vHEdges.at(iHE2ID);
                            else if (iFHE == iHE2ID)
                                _m->vFaces.at(k)->he->paire = _m->vHEdges.at(i);
                        }
                    }
                }
            }
            bTest = false;
        }
    }
    for (unsigned int i = 0; i < iHEsize; i++)
    {
        if(_m->vHEdges.at(i)->paire == NULL)
        {
            he->id = _m->vHEdges.size();
            he->f = NULL;
            he->nouveau = NULL;
            he->paire = _m->vHEdges.at(i);
            he->precedente = NULL;
            he->queue = _m->vHEdges.at(i)->tete;
            he->tete = _m->vHEdges.at(i)->queue;
            he->suivante = NULL;
            _m->vHEdges.push_back(he);
            iHEsize = _m->vHEdges.size();
            _m->vHEdges.at(i)->paire = _m->vHEdges.at(iHEsize-1);
            iNbEdges++;
        }
    }
}
void VertNormal (Mesh * _m)
{
    unsigned int iVsize = _m->vVerts.size();//Taille du vector Vert
    unsigned int iVoisinSize;//Taille du vector voisin
    unsigned int iVertId;
    unsigned int iVertTete;
    unsigned int iVertQueue;
    double v[3],v1[3], v2[3];
    double dMoy [3];
    double dNorme;

    dMoy[0]=0;
    dMoy[1]=0;
    dMoy[2]=0;

    for (unsigned int i = 0; i < iVsize; i++)
    {
        iVertId = _m->vVerts.at(i)->id;
        iVoisinSize = _m->vVerts.at(i)->voisin.size();
        for (unsigned int j = 0; j < iVoisinSize; j++)
        {
            iVertTete = _m->vVerts.at(i)->voisin.at(j)->tete->id;
            iVertQueue = _m->vVerts.at(i)->voisin.at(j)->queue->id;
            {
                //Vecteur entre iVertId et iVertTete
                v1[0]=_m->vVerts.at(iVertId)->pos[0]-_m->vVerts.at(iVertTete)->pos[0];
                v1[1]=_m->vVerts.at(iVertId)->pos[1]-_m->vVerts.at(iVertTete)->pos[1];
                v1[2]=_m->vVerts.at(iVertId)->pos[2]-_m->vVerts.at(iVertTete)->pos[2];
                //Vecteur entre iVertId et iVertQueue
                v2[0]=_m->vVerts.at(iVertId)->pos[0]-_m->vVerts.at(iVertQueue)->pos[0];
                v2[1]=_m->vVerts.at(iVertId)->pos[1]-_m->vVerts.at(iVertQueue)->pos[1];
                v2[2]=_m->vVerts.at(iVertId)->pos[2]-_m->vVerts.at(iVertQueue)->pos[2];
                //Produit Vectoriel entre V1 et V2
                v[0]=v1[1]*v2[2]- v1[2]*v2[1];// Y1*Z2-Z1*Y2
                v[1]=v1[2]*v2[0]-v1[0]*v2[2];//Z1*X2-X1*Z2
                v[2]=v1[0]*v2[1]-v1[1]*v2[0];//X1*Y2-Y1*X2

                //Normalisation du vector
                dNorme = sqrt(pow(v[0], 2) + pow(v[1], 2) + pow(v[2], 2));

                v[0]=-v[0]/dNorme;
                v[1]=-v[1]/dNorme;
                v[2]=-v[2]/dNorme;
            }
            dMoy[0]+=v[0];
            dMoy[1]+=v[1];
            dMoy[2]+=v[2];
        }
        iVoisinSize = _m->vVerts.at(i)->normal[0] = dMoy[0] / iVoisinSize;
        iVoisinSize = _m->vVerts.at(i)->normal[1] = dMoy[1] / iVoisinSize;
        iVoisinSize = _m->vVerts.at(i)->normal[2] = dMoy[2] / iVoisinSize;
    }
}
int MeshType (Mesh * _m)//Définit le Type du Maillage, 0 : Quelconque, 1 : Triangulaire, 2 : Carré
{
    int type = 0;
    //Face
    unsigned int FVertID;
    unsigned int HEVertID;
    unsigned int HEID;
    unsigned int iCpt = 0;
    unsigned int iMax = 0;
    unsigned int iMin = 100;
    for (unsigned int k = 0; k < 6; k++)
    {
        FVertID = _m->vFaces.at(k)->he->queue->id;

        HEID = _m->vFaces.at(k)->he->suivante->id;
        HEVertID = _m->vHEdges.at(HEID)->tete->id;

        while (HEVertID != FVertID)
        {
            iCpt++;
            HEID = _m->vHEdges.at(HEID)->suivante->id;
            HEVertID = _m->vHEdges.at(HEID)->tete->id;
        }
        if (iCpt < iMin)
            iMin = iCpt;
        else if (iCpt > iMax)
            iMax = iCpt;
        iCpt=0;

    }
    std::cout<<"iMax ("<<iMax<<") iMin ("<<iMin<<")"<<std::endl;
    if ((iMin == 1)&&(iMax == 1))
        type = 1;
    else if ((iMin == 2)&&(iMax == 2))
        type = 1;
    return type;
}
void Affichage(Mesh * _m)
{
    //Vert
    for (unsigned int i = 0; i < _m->vVerts.size(); i++)
    {
        std::cout<<"V ("<<i<<") "<<_m->vVerts.at(i)->pos[0]<<" "<<_m->vVerts.at(i)->pos[1]<<" "<<_m->vVerts.at(i)->pos[2]<<std::endl;
    }
    std::cout<<"----------------"<<std::endl;
    //HE
    for (unsigned int j = 0; j < _m->vHEdges.size(); j++)
    {
        std::cout<<"HE ("<<j<<") "<<_m->vHEdges.at(j)->queue->id<<" "<<_m->vHEdges.at(j)->tete->id;
        std::cout<<" pre("<<_m->vHEdges.at(j)->precedente->id<<") suiv("<<_m->vHEdges.at(j)->suivante->id<<")"<<std::endl;
    }
    std::cout<<"----------------"<<std::endl;
    std::cout<<"----------------"<<std::endl;
    //Face
    unsigned int FVertID;
    unsigned int FHEID;
    unsigned int HEVertID;
    unsigned int HEID;
    for (unsigned int k = 0; k <_m->vFaces.size(); k++)
    {
        std::cout<<"F ("<<k<<")"<<std::endl;

        //HEs
        FHEID = _m->vFaces.at(k)->he->id;
        std::cout<<"HE ("<<FHEID<<") ";

        HEVertID = _m->vFaces.at(k)->he->suivante->id;
        //On parcours les arètes de la face
        //Si VID == FVertID alors on a parcourut toutes les arètes.
        while (HEVertID != FHEID)
        {
            std::cout<<"| HE ("<<HEVertID<<") ";
            HEVertID = _m->vHEdges.at(HEVertID)->suivante->id;
        }
        std::cout<<std::endl;
        //Verts
        FVertID = _m->vFaces.at(k)->he->queue->id;
        std::cout<<FVertID<<" "<<_m->vFaces.at(k)->he->tete->id<<" ";

        HEID = _m->vFaces.at(k)->he->suivante->id;
        HEVertID = _m->vHEdges.at(HEID)->tete->id;
        //On parcours les arètes de la face
        //Si VID == FVertID alors on a parcourut toutes les arètes.
        while (HEVertID != FVertID)
        {
            std::cout<<HEVertID<<" ";
            HEID = _m->vHEdges.at(HEID)->suivante->id;
            HEVertID = _m->vHEdges.at(HEID)->tete->id;
        }
        std::cout<<std::endl;
    }
    std::cout<<"----------------"<<std::endl;
    std::cout<<"----------------"<<std::endl;
    std::cout<<"----------------"<<std::endl;
}
