#include "polygone.h"
#include <vector>

Polygone::Polygone()
{}

Polygone::~Polygone()
{
	vecPoints.clear();
	vecTrou.clear();
}

void Polygone::ajouterPoints(int _indice, POINT _point)
{	
	vecPoints.insert(vecPoints.begin() + _indice, _point);
}

void Polygone::supprimerPoints(int indice)
{
	vecPoints.erase(vecPoints.begin() + indice);
}

void Polygone::ajouterPointsTrou(int _indice, POINT _point)
{
	vecTrou.insert(vecTrou.begin() + _indice, _point);
}

void Polygone::supprimerPointsTrou(int indice)
{
	vecTrou.erase(vecTrou.begin() + indice);
}
