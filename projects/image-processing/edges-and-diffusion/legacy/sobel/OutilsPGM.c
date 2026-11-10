#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "OutilsPGM.h"


#define BUFLINESIZE 256


/* Allocation d'un buffer image de taille _w * _h * 1 Pixel */
int CreerImagePGM(PtrImage _pimg, int _w, int _h)
{
	/* Verification de l'allocation de l'image */
	if(_pimg == NULL)
	{
		fprintf(stderr, "Erreur: Image non allouee.\n");
		return -1;
	}
	
	/* Creation de l'image et allocation du buffer */
	_pimg->width  = _w;
	_pimg->height = _h;
	_pimg->size	  = _pimg->width * _pimg->height;
	_pimg->data	  = (Pixel *)calloc(_pimg->size, sizeof(Pixel));
	
	/* Verification de l'allocation buffer */
	if(_pimg->data == NULL)
	{
		fprintf(stderr, "Erreur: Impossible d'allouer une taille de %d octets pour le buffer.\n", _pimg->size);
		return -1;
	}
	
	/* Tout est ok */
	return 0;
}


/* Lecture d'un fichier PGM */
int LireImagePGM(PtrImage _pimg, const char * _nom)
{
	char   s[BUFLINESIZE];
	FILE * f = fopen(_nom, "rb");
	int	   k = PGMAscii;
	int	   w, h, m, i, v;
	Pixel	 * p;
	unsigned char   px;
	
	/* Verification de l'allocation de l'image */
	if(_pimg == NULL)
	{
		fprintf(stderr, "Erreur: Image non allouee.\n");
		return -1;
	}
	
	/* Test d'ouverture du fichier */
	if(f == NULL)
	{
		fprintf(stderr, "Erreur: Impossible d'ouvrir le fichier \"%s\" en lecture.\n", _nom);
		return -1;
	}
	
	/* Recuperation de la premiere ligne */
	fgets(s, BUFLINESIZE, f);
	
	/* Test sur le Magic Number */
	if(s[0] != 'P' || (s[1] != '2' && s[1] != '5'))
	{
		printf("%s\n", s);
		fprintf(stderr, "Erreur: Magic Number invalide.\n");
		fclose(f);
		return -1;
	}
	
	/* Test sur les methodes de lecture (ASCII ou Binaire) */
	if(s[1] == '5')
		k = PGMBinary;
	
	/* Suppression des commentaires */
	do
		fgets(s, BUFLINESIZE, f);
	while(s[0] == '#');
	
	/* Lecture de la taille de l'image */
	sscanf(s, "%d %d", &w, &h);
	
	/* Suppression des commentaires */
	do
		fgets(s, BUFLINESIZE, f);
	while(s[0] == '#');
	
	/* Lecture du niveau de gris maximum */
	sscanf(s, "%d", &m);
	
	/* Test sur les dimensions de l'image */
	if(w * h < 1)
	{
		fprintf(stderr, "Erreur: Dimensions d'image invalides (%dx%d).\n", w, h);
		fclose(f);
		return -1;
	}
	
	/* Test sur l'allocation de l'image */
	if(CreerImagePGM(_pimg, w, h) < 0)
	{
		fclose(f);
		return -1;
	}
	
	i = 0;
	p = _pimg->data;
	
	/* Lecture des donnees de l'image */
	if(k == PGMAscii)
	{
		/* Tant qu'il n'y a pas n pixels de lus */
		while(i++ < _pimg->size)
		{
			if(fscanf(f, "%d", &v) != 1)
			{
				fprintf(stderr, "Erreur: Probleme lors de la lecture de l'image.\n");
				fclose(f);
				return -1;
			}
			
			*p++ = (Pixel)v;
		}
	}
	else
	{
		for(i = 0; i < _pimg->size; ++i)
		{
			if(fread(&px, sizeof(unsigned char), 1, f) != 1)
			{
				fprintf(stderr, "Erreur: Probleme lors de la lecture de l'image.\n");
				fclose(f);
				return -1;
			}
			else
			{
				*p++ = (Pixel)px;
			}
		}
	}
	
	/* Fermeture du fichier, tout est ok */
	fclose(f);
	return 0;
}


/* Ecriture d'un fichier PGM dans le format specifie (PGMAscii ou PGMBinary) */
int EcrireImagePGM(PtrImage _pimg, const char * _nom, int _format)
{
	FILE * f = fopen(_nom, "wb");
	Pixel * p;
	unsigned char px;
	int i;
	
	/* Verification de l'allocation de l'image */
	if(_pimg == NULL)
	{
		fprintf(stderr, "Erreur: Image non allouee.\n");
		return -1;
	}
	
	/* Test d'ouverture du fichier */
	if(f == NULL)
	{
		fprintf(stderr, "Erreur: Impossible d'ouvrir le fichier \"%s\" en ecriture.\n", _nom);
		return -1;
	}
	
	i = 0;
	p = _pimg->data;
	
	/* Ecriture des donnees de l'image */
	if(_format == PGMAscii)
	{
		/* Ecriture de l'entete */
		if(fprintf(f, "P2\n%d %d\n255\n", _pimg->width, _pimg->height) < 1)
		{	
			fprintf(stderr, "Erreur: Probleme lors de l'ecriture de l'entete du fichier.\n");
			fclose(f);
			return -1;
		}
		
		/* Tant qu'il n'y a pas n pixels d'ecrits */
		while(i++ < _pimg->size)
		{
			if(fprintf(f, "%d ", (int)*p++) < 1)
			{
				fprintf(stderr, "Erreur: Probleme lors de l'ecriture de l'image.\n");
				fclose(f);
				return -1;
			}
		}
	}
	else
	{
		/* Ecriture de l'entete */
		if(fprintf(f, "P5\n%d %d\n255\n", _pimg->width, _pimg->height) < 1)
		{	
			fprintf(stderr, "Erreur: Probleme lors de l'ecriture de l'entete du fichier.\n");
			fclose(f);
			return -1;
		}
		
		for(i = 0; i < _pimg->size; ++i)
		{
			px = (unsigned char)_pimg->data[i];
			if(fwrite(&px, sizeof(unsigned char), 1, f) != 1)
			{
				fprintf(stderr, "Erreur: Probleme lors de l'ecriture de l'image.\n");
				fclose(f);
				return -1;
			}
		}
	}
	
	/* Fermeture du fichier, tout est ok */
	fclose(f);
	return 0;
}


/* Liberation de l'image */
int LibererImagePGM(PtrImage _pimg)
{
	/* Verification de l'allocation de l'image */
	if(_pimg == NULL)
	{
		fprintf(stderr, "Erreur: Image non allouee.\n");
		return -1;
	}
	
	/* Verification de l'allocation du buffer image */
	if(_pimg->data == NULL)
	{
		fprintf(stderr, "Erreur: Buffer image non alloue.\n");
		return -1;
	}
	
	/* Desallocation du buffer, tout est ok */
	free(_pimg->data);
	_pimg->data = NULL;
	
	return 0;
}


/* Preparation de l'image pour l'affichage OpenGL */
int BasculerImagePGM(PtrImage _pimg, unsigned char * _dest)
{
	int i, j;
	Pixel * p;
	unsigned char * g;

	/* Verification de l'allocation de l'image */
	if(_pimg == NULL)
	{
		fprintf(stderr, "Erreur: Image non allouee.\n");
		return -1;
	}
	
	/* Verification de l'allocation du buffer */
	if(_dest == NULL)
	{
		fprintf(stderr, "Erreur: Buffer non alloue.\n");
		return -1;
	}
	
	/* Basculement de l'image */
	for(i = 0; i < _pimg->height; ++i)
	{
		p = &_pimg->data  [i * _pimg->width];
		g = &_dest[(_pimg->height - i - 1) * _pimg->width];
		
		for(j = 0; j < _pimg->width; ++j)
			*g++ = (unsigned char)*p++;
	}
	
	/* Tout est ok */
	return 0;
}
