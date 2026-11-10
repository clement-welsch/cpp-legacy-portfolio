///////////////////////////////////////////////////////////////////////////////
// texture.h
// ----------------------------------------------------------------------------
// Auteur         : Sébastien Thon
// Création       : 3 novembre 2004
// Dernière modif.: 3 novembre 2004
// ----------------------------------------------------------------------------
// Classe permettant de gérer une texture OpenGL.
///////////////////////////////////////////////////////////////////////////////

#ifndef TEXTURE_H
#define TEXTURE_H


#include <string>
#include "glut.h"



class Texture
{
public :
	// Constructeur
	Texture();

	// Destructeur
	~Texture();
	
	// Charge une image de type TGA, et en fait une texture OpenGL
	bool charger( char *nom_fichier, bool mipmap=false );

	// Définit la texture comme étant la texture courante 
	void utilise();

	// Définit le mode de filtrage de la texture
	void definit_filtrage( GLenum type, GLint mode );

	// Définit le mode de bouclage de la texture
	void definit_bouclage( GLenum axe, GLint mode );

	// Contrôle la manière selon laquelle la texture est mélangée à la couleur de l'objet
	void definit_melange( GLint mode, GLfloat *env_color=0 );

	// Supprime la texture de la mémoire 
	void supprime();

	GLuint get_ident() { return ident; }

	std::string get_nom() { return nom; }

private:
	// Identificateur de texture donné par la fonction glGenTextures()
	GLuint	ident;

	// Largeur et hauteur de la texture, données par les dimensions de l'image TGA
	int		largeur, hauteur;

	// Nom de la texture
	std::string	nom;
};



#endif
