#define NB_NV_GRIS 256

typedef struct {
	unsigned long nb_pixels;
	double max;
	int classeMax;
	double t[NB_NV_GRIS];

}Histo;


void souris (int bouton, int etat, int x, int y);
void redim(int largeur, int hauteur);
void affichage(void);
void affichage2(void);
void BasculeImage (Image *image, GLubyte *I);
void LirePGM (GLubyte **_pImage, Image* imgOut);
void traitement();
void initHisto(Histo * _histo);
void calculHisto (Image * im1, Histo * _histo);
void calculHistoCumul(Image * im1, Histo * _histo);
void copieImg(Image *src, Image* out);

