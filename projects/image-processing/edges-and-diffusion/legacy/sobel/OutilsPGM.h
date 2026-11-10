#ifndef __OutilsPGM_h__
#define __OutilsPGM_h__


	/* Definition du type Pixel : en l'occurence, entier signe */
	typedef int Pixel;

	/* Definition de la structure d'une image */
	typedef struct {
		int width;					/* largeur	 			*/
		int height;					/* hauteur 				*/
		int size;					/* largeur * hauteur	*/
	
		Pixel * data;				/* buffer				*/
	} Image, * PtrImage;


	/* Definition des types de formats */
	#define PGMAscii  0
	#define PGMBinary 1


	/* Creation d'une image vide de taille _w x _h x 1 Pixel */
	int CreerImagePGM   (PtrImage _pimg, int _w, int _h);

	/* Lecture d'un fichier PGM */
	int LireImagePGM    (PtrImage _pimg, const char * _nom);

	/* Ecriture d'un fichier PGM dans le format specifie (PGMAscii ou PGMBinary) */
	int EcrireImagePGM  (PtrImage _pimg, const char * _nom, int _format);

	/* Liberation de l'image */
	int LibererImagePGM (PtrImage _pimg);

	/* Preparation de l'image pour l'affichage OpenGL */
	int BasculerImagePGM(PtrImage _pimg, unsigned char * _dest);


#endif
