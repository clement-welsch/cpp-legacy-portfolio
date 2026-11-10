#define MAXLIGNE 256

typedef struct
	{
	double reel; 
        double imaginaire;
        }Complex;

typedef short Pixel;
typedef Complex PixelC;
typedef double PixelD;

typedef struct
	{
	int width, height, size;
	Pixel *data;
	}Image;

typedef struct
{
	int width, height, size;
	PixelC *data;
}ImageC;

typedef struct
{
	int width, height, size;
	PixelD *data;
}ImageD;

int CreerImage(Image *image, int largeur, int hauteur);
int LireImage(char *nom, Image *image);
int EcrireImage(char *nom, Image *image);
void LibererImage(Image *i);
int CreerImageD(ImageD *image_d, int largeur, int hauteur);
void LibererImageD(ImageD *i_d);
