#include <iostream>
#include <stdlib.h>
#include "ChuteDomino.h"

using namespace std;

ChuteDomino::ChuteDomino()
{}
ChuteDomino::~ChuteDomino()
{}
void ChuteDomino::charger_textures(char * nomfichier)
{
    FILE    *fp;
    if ((fp=fopen(nomfichier,"r"))==NULL)
    {
        cerr<<"Erreur a l'ouverture du fichier"<<nomfichier<<endl;
        exit(1);
    }
    char nombre[10];
    fscanf(fp,"%s\n",nombre);
    char nomText[50];
    for(int i = 0;i< atoi(nombre);i++)
	{
        fscanf(fp,"%s\n",nomText);
        Texture temp;
        if( !temp.charger(nomText,false))
        {
            printf( "Image %s introuvable\n",nomText );
            exit(1);
		}
        listTexture.push_back(temp);
        cout<<"Texture : "<<nomText<<" chargée"<<endl;
    }
}
/* Note du programmeur : Dans la méthode suivante j'ai décidé d'utiliser un fichier texte listant les textures à utiliser.
Cela me permet de gagner en clarté dans mon programme.
*/
void ChuteDomino::charger_dominos(char * nomfichier)
{
    charger_textures((char*)"liste_textures.txt"); //ouverture du fichier "liste_textures.txt"
    FILE    *fp;
    if ((fp=fopen(nomfichier,"r"))==NULL)
    {
        cerr<<"Erreur a l'ouverture du fichier"<<nomfichier<<endl;
        exit(1);
    }
    char nombre[10];
    fscanf(fp,"%s\n",nombre);
    for(int i = 0;i< atoi(nombre);i++)
	{
               
        float tmpPosX,tmpPosY,tmpPosZ,tmpDimX,tmpDimY,tmpDimZ,tmpOrient;
        int tmpNumText;
        fscanf(fp,"%f %f %f %f %f %f %f %d",&tmpPosX,&tmpPosY,&tmpPosZ,&tmpDimX,&tmpDimY,&tmpDimZ,&tmpOrient,&tmpNumText);
        Domino tmpDomino(tmpPosX,tmpPosY,tmpPosZ,tmpDimX,tmpDimY,tmpDimZ,tmpOrient,&listTexture.at(tmpNumText));
        listDomino.push_back(tmpDomino);
    }
}
void ChuteDomino::afficher(int i)
{listDomino.at(i).afficher();}

void ChuteDomino::DefinitPhysique(dWorldID world,dSpaceID space)
{
    for(int i=0;i<listDomino.size();i++)
        listDomino.at(i).DefinitPhysique(world,space);
}

