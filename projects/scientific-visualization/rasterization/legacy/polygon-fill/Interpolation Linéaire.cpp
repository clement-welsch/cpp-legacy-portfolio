void ComputeInterpolationInformations_x(int x1, int y1, int x2, int y2, int *inc1, int *inc2, int *D, int *m, int *mult)
{
	int Dx, Dy;

	if (y1 > y2)
	{
		*mult = -1;
		y2 = (*mult)*y2;
		y1 = (*mult)*y1;
	}
	else
		*mult = 1;
	Dx = x2 - x1;
	Dy = y2 - y1;
	if (Dy != 0)
		*m = Dx/Dy;
	else
		*m = 1024;
	*inc1 = 2*Dx - 2*(*m)*Dy;
	*inc2 = 2*Dx - 2*((*m)+1)*Dy;
	*D = 2*Dx - (2*(*m)+1)*Dy;
}
void interpolation(Point A, Point B, int id)
{
	int i, j, x1, y1, x2, y2, x, y, inc1, inc2, D, m, mult;
	Point res;
	int k = poly.Couleur[id].Nb_Point;
	/*if(putain(A, id) == -1)
		ajouter_tab(A, k, &poly.Couleur[id]);
	if(putain(B, id) == -1)
	ajouter_tab(B, k, &poly.Couleur[id]);*/

	x1 = A.x;
	y1 = A.y;
	x2 = B.x;
	y2 = B.y;

	if (x1 > x2)
	{
		x  = x1;
		x1 = x2;
		x2 = x;

		y  = y1;
		y1 = y2;
		y2 = y;
	}

	ComputeInterpolationInformations_x(x1, y1, x2, y2, &inc1, &inc2, &D, &m, &mult);

	x = x1;
	y = mult * y1;

	glColor3f(1.0,1.0,0.0);
	glPointSize(2.0);
	glBegin(GL_POINTS);
	while (y < mult*y2)
	{
		y += 1;
		if (D <= 0)
		{
			D += inc1;
			x +=  m;
		}
		else
		{
			D += inc2;
			x += m+1;
		}

		glVertex2i(x, mult * y);
		res.x = x;
		res.y = (mult * y);
		ajouter_tab(res, k, &poly.Couleur[id]);
		//ajouter(res,k);
		//k++;
	}
	glEnd();
}
