#define MAXLIGNE 256

typedef struct
	{
	double reel; 
        double imaginaire;
        }Complex;

typedef short Pixel;

typedef struct
	{
	int width, height, size;
	Pixel *data;
	}Image;


int CreerImage(Image *image, int largeur, int hauteur);
int LireImage(char *nom, Image *image);
int EcrireImage(char *nom, Image *image);
void LibererImage(Image *i);
