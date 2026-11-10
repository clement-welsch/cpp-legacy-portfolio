///////////////////////////////////////////////////////////////////////////////
// texture.cpp
// ----------------------------------------------------------------------------
// Auteur         : Sébastien Thon
// Création       : 3 novembre 2004
// Dernière modif.: 3 novembre 2004
// ----------------------------------------------------------------------------
// Classe permettant de gérer une texture OpenGL.
///////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <stdio.h>
#include "texture.h"
#include "tga.h"



///////////////////////////////////////////////////////////////////////////////
// Constructeur de la classe Texture.
//-----------------------------------------------------------------------------
// Paramètres :
//    _
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
Texture::Texture()
{
	ident = 0;
}



///////////////////////////////////////////////////////////////////////////////
// Destructeur de la classe Texture.
//-----------------------------------------------------------------------------
// Paramètres :
//    _
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
Texture::~Texture()
{
	supprime();
}



///////////////////////////////////////////////////////////////////////////////
// Définit la texture comme étant la texture courante
//-----------------------------------------------------------------------------
// Paramètres :
//    _
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
void Texture::utilise()
{
	glBindTexture(GL_TEXTURE_2D, ident);
}



///////////////////////////////////////////////////////////////////////////////
// Définit le mode de filtrage de la texture
//-----------------------------------------------------------------------------
// Paramètres :
//    type  (in) : type de filtrage (GL_TEXTURE_MIN_FILTER ou GL_TEXTURE_MAG_FILTER
//    mode  (in) : mode de filtrage (GL_NEAREST ou GL_LINEAR ou
//                                   GL_NEAREST_MIPMAP_NEAREST ou GL_LINEAR_MIPMAP_LINEAR).
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
void Texture::definit_filtrage( GLenum type, GLint mode )
{
	glBindTexture(GL_TEXTURE_2D, ident);
	glTexParameteri(GL_TEXTURE_2D, type, mode);
}



///////////////////////////////////////////////////////////////////////////////
// Définit le mode de bouclage de la texture
//-----------------------------------------------------------------------------
// Paramètres :
//    axe  (in) : axe concerné par le bouclage (GL_TEXTURE_WRAP_S ou GL_TEXTURE_WRAP_T)
//    mode (in) : mode de bouclage (GL_REPEAT ou GL_CLAMP).
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
void Texture::definit_bouclage( GLenum axe, GLint mode )
{
	glBindTexture(GL_TEXTURE_2D, ident);
	glTexParameteri( GL_TEXTURE_2D, axe, mode );
}



///////////////////////////////////////////////////////////////////////////////
// Contrôle la manière selon laquelle la texture est mélangée à la couleur de
// l'objet
//-----------------------------------------------------------------------------
// Paramètres :
//    mode  (in) : mode de mélange (GL_DECAL ou GL_MODULATE ou GL_BLEND)
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
void Texture::definit_melange( GLint mode, GLfloat *env_color )
{
	glBindTexture(GL_TEXTURE_2D, ident);
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mode );

	if( mode == GL_BLEND )
		glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, env_color);
}



///////////////////////////////////////////////////////////////////////////////
// Supprime la texture de la mémoire
//-----------------------------------------------------------------------------
// Paramètres :
//    _
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
void Texture::supprime()
{
	glDeleteTextures(1, &ident);
}



///////////////////////////////////////////////////////////////////////////////
// Chargement d'une image TGA, utilisée pour créer une texture OpenGL.
//-----------------------------------------------------------------------------
// Paramètres :
//    nom_fichier (in): nom du fichier TGA à charger.
//    mipmap      (in): indique si on veut une texture en mipmaping ou non.
//
// Retour :
//    true si la texture a été chargée, false sinon.
///////////////////////////////////////////////////////////////////////////////
bool Texture::charger( char *nom_fichier, bool mipmap )
{
	BYTE	*img;
	int		bpp;

	// Supprime la texture précédente
	if( glIsTexture(ident) )
		supprime();

	// Génère un identificateur de texture
	glGenTextures(1, &ident);

	// Chargement de l'image au format TGA
	img = load_tga( nom_fichier, &largeur, &hauteur, &bpp );

	if( img == NULL )
		return false;

	// On déclare cette texture comme la texture courante
	glBindTexture(GL_TEXTURE_2D, ident);

	if( mipmap == true )
	{
		// Chargement de l'image en mémoire vidéo en tant que mip-map
		if( bpp == 24 )
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, largeur, hauteur, GL_RGB, GL_UNSIGNED_BYTE, img);
		else
			gluBuild2DMipmaps(GL_TEXTURE_2D, 4, largeur, hauteur, GL_RGBA, GL_UNSIGNED_BYTE, img);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else
	{
		// Chargement de l'image en mémoire vidéo
		if( bpp == 24 )
			glTexImage2D(GL_TEXTURE_2D, 0, 3, largeur, hauteur, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, 4, largeur, hauteur, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	}

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	// On supprime le tableau dynamique alloué par load_tga(),
    // l'image ayant été recopiée en mémoire vidéo
	delete[] img;

	// Le nom de la texture reçoit le nom du fichier
	nom = nom_fichier;

	return true;
}
