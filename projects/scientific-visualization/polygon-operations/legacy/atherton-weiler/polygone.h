#include <cstring>
#include <cstdlib>
#include <vector>
#include <iostream>


using namespace std;
#define MAXLIGNE 256

struct POINT
{
	double x;
	double y;
	POINT(double _x, double _y)
	{
		x = _x;
		y = _y;
	}	
};

struct INTERSECT
{
	int indice1;
	int indice2;
	double x;
	double y;
	INTERSECT(int _i1, int _i2, double _x, double _y)
	{
		indice1 = _i1;
		indice2 = _i2;
		x = _x;
		y = _y;	
	}	
};

class Polygone{

public :
	 Polygone();
	~Polygone();
	
	vector<POINT> vecPoints;
	vector<POINT> vecTrou;
	
	void ajouterPoints(int _indice, POINT _point);
	void supprimerPoints(int indice);
	void supprimerPointsTrou(int indice);
	void ajouterPointsTrou(int _indice, POINT _point);
};

