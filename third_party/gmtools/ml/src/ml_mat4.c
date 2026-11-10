/********************************************************************************
* ___  ___        _    _             _      _  _                                *
* |  \/  |       | |  | |           | |    (_)| |                               *
* | .  . |  __ _ | |_ | |__   ___   | |     _ | |__   _ __   __ _  _ __  _   _  *
* | |\/| | / _` || __|| '_ \ / __|  | |    | || '_ \ | '__| / _` || '__|| | | | *
* | |  | || (_| || |_ | | | |\__ \  | |____| || |_) || |   | (_| || |   | |_| | *
* \_|  |_/ \__,_| \__||_| |_||___/  \_____/|_||_.__/ |_|    \__,_||_|    \__, | *
*                                                                        __/ |  *
*  Copyright (c) 2010                                                   |___/   *
*  Author: Dimitri KUDELSKI                                                     *
*  Contact: kudelski [at] univmed [dot] fr                                      *
*                                                                               *
*                                                                               *
* This program is free software: you can redistribute it and/or modify it under *
* the terms of the GNU General Public License as published by the Free Software *
* Foundation, either version 3 of the License, or (at your option) any later    * 
* version.                                                                      *
*                                                                               *
* This program is distributed in the hope that it will be useful, but WITHOUT   *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS *
* FOR A PARTICULAR PURPOSE. See the GNU General Public License for more         *
* details.                                                                      *
*                                                                               *
*********************************************************************************/    


#include <ml/ml_mat4.h>
#include <ml/ml_vec4.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


void mlMat4_Set(mlMat4 _out, double _a, double _b, double _c, double _d, double _e, double _f, double _g, double _h, double _i, double _j, double _k, double _l, double _m, double _n, double _o, double _p)
{
	_out[ 0] = _a;
	_out[ 1] = _b;
	_out[ 2] = _c;
	_out[ 3] = _d;
	_out[ 4] = _e;
	_out[ 5] = _f;
	_out[ 6] = _g;
	_out[ 7] = _h;
	_out[ 8] = _i;
	_out[ 9] = _j;
	_out[10] = _k;
	_out[11] = _l;
	_out[12] = _m;
	_out[13] = _n;
	_out[14] = _o;
	_out[15] = _p;
}


void mlMat4_Zero(mlMat4 _out)
{
	mlMat4_Set(_out, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
}


void mlMat4_Identity(mlMat4 _out)
{
	mlMat4_Set(_out, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
}


void mlMat4_Copy(mlMat4 _out, const mlMat4 _in)
{
	mlMat4_Set(_out, _in[ 0], _in[ 1], _in[ 2], _in[ 3], 
					 _in[ 4], _in[ 5], _in[ 6], _in[ 7], 
					 _in[ 8], _in[ 9], _in[10], _in[11], 
					 _in[12], _in[13], _in[14], _in[15]);
}


void mlMat4_FromCols(mlMat4 _out, const mlVec4 _col1, const mlVec4 _col2, const mlVec4 _col3, const mlVec4 _col4)
{
	mlMat4_Set(_out, _col1[0], _col2[0], _col3[0], _col4[0],
					 _col1[1], _col2[1], _col3[1], _col4[1],
					 _col1[2], _col2[2], _col3[2], _col4[2],
					 _col1[3], _col2[3], _col3[3], _col4[3]);
}


void mlMat4_FromRows(mlMat4 _out, const mlVec4 _row1, const mlVec4 _row2, const mlVec4 _row3, const mlVec4 _row4)
{
	mlMat4_Set(_out, _row1[0], _row1[1], _row1[2], _row1[3],
					 _row2[0], _row2[1], _row2[2], _row2[3],
					 _row3[0], _row3[1], _row3[2], _row3[3],
					 _row4[0], _row4[1], _row4[2], _row4[3]);
}


void mlMat4_Add(mlMat4 _out, const mlMat4 _in, double _k)
{
	mlMat4_Set(_out, _in[ 0] + _k, _in[ 1] + _k, _in[ 2] + _k, _in[ 3] + _k, 
					 _in[ 4] + _k, _in[ 5] + _k, _in[ 6] + _k, _in[ 7] + _k, 
					 _in[ 8] + _k, _in[ 9] + _k, _in[10] + _k, _in[11] + _k, 
					 _in[12] + _k, _in[13] + _k, _in[14] + _k, _in[15] + _k);
}


void mlMat4_Sub(mlMat4 _out, const mlMat4 _in, double _k)
{
	mlMat4_Add(_out, _in, -_k);
}


void mlMat4_Scale(mlMat4 _out, const mlMat4 _in, double _k)
{
	mlMat4_Set(_out, _in[ 0] * _k, _in[ 1] * _k, _in[ 2] * _k, _in[ 3] * _k, 
					 _in[ 4] * _k, _in[ 5] * _k, _in[ 6] * _k, _in[ 7] * _k, 
					 _in[ 8] * _k, _in[ 9] * _k, _in[10] * _k, _in[11] * _k, 
					 _in[12] * _k, _in[13] * _k, _in[14] * _k, _in[15] * _k);
}


void mlMat4_Neg(mlMat4 _out, const mlMat4 _in)
{
	mlMat4_Set(_out, -_in[ 0], -_in[ 1], -_in[ 2], -_in[ 3], 
					 -_in[ 4], -_in[ 5], -_in[ 6], -_in[ 7], 
					 -_in[ 8], -_in[ 9], -_in[10], -_in[11], 
					 -_in[12], -_in[13], -_in[14], -_in[15]);
}


void mlMat4_AddMat(mlMat4 _out, const mlMat4 _in1, const mlMat4 _in2)
{
	mlMat4_Set(_out, _in1[ 0] + _in2[ 0], _in1[ 1] + _in2[ 1], _in1[ 2] + _in2[ 2], _in1[ 3] + _in2[ 3], 
					 _in1[ 4] + _in2[ 4], _in1[ 5] + _in2[ 5], _in1[ 6] + _in2[ 6], _in1[ 7] + _in2[ 7], 
					 _in1[ 8] + _in2[ 8], _in1[ 9] + _in2[ 9], _in1[10] + _in2[10], _in1[11] + _in2[11], 
					 _in1[12] + _in2[12], _in1[13] + _in2[13], _in1[14] + _in2[14], _in1[15] + _in2[15]);
}


void mlMat4_SubMat(mlMat4 _out, const mlMat4 _in1, const mlMat4 _in2)
{
	mlMat4_Set(_out, _in1[ 0] - _in2[ 0], _in1[ 1] - _in2[ 1], _in1[ 2] - _in2[ 2], _in1[ 3] - _in2[ 3], 
					 _in1[ 4] - _in2[ 4], _in1[ 5] - _in2[ 5], _in1[ 6] - _in2[ 6], _in1[ 7] - _in2[ 7], 
					 _in1[ 8] - _in2[ 8], _in1[ 9] - _in2[ 9], _in1[10] - _in2[10], _in1[11] - _in2[11], 
					 _in1[12] - _in2[12], _in1[13] - _in2[13], _in1[14] - _in2[14], _in1[15] - _in2[15]);
}


void mlMat4_MultMat(mlMat4 _out, const mlMat4 _in1, const mlMat4 _in2)
{
	mlMat4 m;
	
	m[ 0] = _in1[ 0] * _in2[ 0] + _in1[ 1] * _in2[ 4] + _in1[ 2] * _in2[ 8] + _in1[ 3] * _in2[12];
	m[ 1] = _in1[ 0] * _in2[ 1] + _in1[ 1] * _in2[ 5] + _in1[ 2] * _in2[ 9] + _in1[ 3] * _in2[13];
	m[ 2] = _in1[ 0] * _in2[ 2] + _in1[ 1] * _in2[ 6] + _in1[ 2] * _in2[10] + _in1[ 3] * _in2[14];
	m[ 3] = _in1[ 0] * _in2[ 3] + _in1[ 1] * _in2[ 7] + _in1[ 2] * _in2[11] + _in1[ 3] * _in2[15];
	m[ 4] = _in1[ 4] * _in2[ 0] + _in1[ 5] * _in2[ 4] + _in1[ 6] * _in2[ 8] + _in1[ 7] * _in2[12];
	m[ 5] = _in1[ 4] * _in2[ 1] + _in1[ 5] * _in2[ 5] + _in1[ 6] * _in2[ 9] + _in1[ 7] * _in2[13];
	m[ 6] = _in1[ 4] * _in2[ 2] + _in1[ 5] * _in2[ 6] + _in1[ 6] * _in2[10] + _in1[ 7] * _in2[14];
	m[ 7] = _in1[ 4] * _in2[ 3] + _in1[ 5] * _in2[ 7] + _in1[ 6] * _in2[11] + _in1[ 7] * _in2[15];
	m[ 8] = _in1[ 8] * _in2[ 0] + _in1[ 9] * _in2[ 4] + _in1[10] * _in2[ 8] + _in1[11] * _in2[12];
	m[ 9] = _in1[ 8] * _in2[ 1] + _in1[ 9] * _in2[ 5] + _in1[10] * _in2[ 9] + _in1[11] * _in2[13];
	m[10] = _in1[ 8] * _in2[ 2] + _in1[ 9] * _in2[ 6] + _in1[10] * _in2[10] + _in1[11] * _in2[14];
	m[11] = _in1[ 8] * _in2[ 3] + _in1[ 9] * _in2[ 7] + _in1[10] * _in2[11] + _in1[11] * _in2[15];
	m[12] = _in1[12] * _in2[ 0] + _in1[13] * _in2[ 4] + _in1[14] * _in2[ 8] + _in1[15] * _in2[12];
	m[13] = _in1[12] * _in2[ 1] + _in1[13] * _in2[ 5] + _in1[14] * _in2[ 9] + _in1[15] * _in2[13];
	m[14] = _in1[12] * _in2[ 2] + _in1[13] * _in2[ 6] + _in1[14] * _in2[10] + _in1[15] * _in2[14];
	m[15] = _in1[12] * _in2[ 3] + _in1[13] * _in2[ 7] + _in1[14] * _in2[11] + _in1[15] * _in2[15];
	
	mlMat4_Copy(_out, m);
}


void mlMat4_MultVec(mlVec4 _out, const mlMat4 _in1, const mlVec4 _in2)
{
	mlVec4 v;
	
	v[0] = _in1[ 0] * _in2[0] + _in1[ 1] * _in2[1] + _in1[ 2] * _in2[2] + _in1[ 3] * _in2[3];
	v[1] = _in1[ 4] * _in2[0] + _in1[ 5] * _in2[1] + _in1[ 6] * _in2[2] + _in1[ 7] * _in2[3];
	v[2] = _in1[ 8] * _in2[0] + _in1[ 9] * _in2[1] + _in1[10] * _in2[2] + _in1[11] * _in2[3];
	v[3] = _in1[12] * _in2[0] + _in1[13] * _in2[1] + _in1[14] * _in2[2] + _in1[15] * _in2[3];
	
	mlVec4_Copy(_out, v);
}


void __mlMat4_SwapRows__(mlMat4 _out, int _i, int _j)
{
	mlVec4 m;
	
	m[0] = _out[4 * _i + 0];
	m[1] = _out[4 * _i + 1];
	m[2] = _out[4 * _i + 2];
	m[3] = _out[4 * _i + 3];
	
	_out[4 * _i + 0] = _out[4 * _j + 0];
	_out[4 * _i + 1] = _out[4 * _j + 1];
	_out[4 * _i + 2] = _out[4 * _j + 2];
	_out[4 * _i + 3] = _out[4 * _j + 3];
	
	_out[4 * _j + 0] = m[0];
	_out[4 * _j + 1] = m[1];
	_out[4 * _j + 2] = m[2];
	_out[4 * _j + 3] = m[3];
}


double mlMat4_Det(const mlMat4 _in)
{
	double det = 1.0;
	double s;
	mlMat4 q, r;
	
	int i, j, k, h;
	
	mlMat4_Copy	   (q, _in);
	mlMat4_Identity(r);
	
	for(j = 0; j < 4; ++j)
	{
		h = -1;
		s = 0.0;
		
		for(i = j; i < 4; ++i)
		{
			if(fabs(q[i * 4 + j]) > fabs(s))
			{
				s = q[i * 4 + j];
				h = i;
			}
		}
		
		if(h == -1)
		{
			return 0.0;
		}
		
		if(h != j)
		{
			__mlMat4_SwapRows__(q, h, j);
			__mlMat4_SwapRows__(r, h, j);
			
			det *= -1.0;
		}
		
		det *= s;
		s    = 1.0 / s;
		
		for(k = 0; k < 4; ++k)
		{
			q[j * 4 + k] *= s;
			r[j * 4 + k] *= s;
		}
		
		for(k = 0; k < 4; ++k)
		{
			if(k != j)
			{
				s = q[k * 4 + j];
				
				for(i = 0; i < 4; ++i)
				{
					q[k * 4 + i] -= s * q[j * 4 + i];
					r[k * 4 + i] -= s * r[j * 4 + i];
				}
			}
		}
	}
	
	return det;
}


void mlMat4_Inv(mlMat4 _out, const mlMat4 _in)
{
	double det = 1.0;
	double s;
	mlMat4 q;
	
	int i, j, k, h;
	
	mlMat4_Copy(q, _in);
	for(j = 0; j < 4; ++j)
	{
		h = -1;
		s = 0.0;
		
		for(i = j; i < 4; ++i)
		{
			if(fabs(q[i * 4 + j]) > fabs(s))
			{
				s = q[i * 4 + j];
				h = i;
			}
		}
		
		if(h == -1)
		{
			mlMat4_Identity(_out);
			return;
		}
		
		if(h != j)
		{
			__mlMat4_SwapRows__(q,    h, j);
			__mlMat4_SwapRows__(_out, h, j);
			
			det *= -1.0;
		}
		
		det *= s;
		s    = 1.0 / s;
		
		for(k = 0; k < 4; ++k)
		{
			q   [j * 4 + k] *= s;
			_out[j * 4 + k] *= s;
		}
		
		for(k = 0; k < 4; ++k)
		{
			if(k != j)
			{
				s = q[k * 4 + j];
				
				for(i = 0; i < 4; ++i)
				{
					q   [k * 4 + i] -= s * q   [j * 4 + i];
					_out[k * 4 + i] -= s * _out[j * 4 + i];
				}
			}
		}
	}
}


void mlMat4_Transpose(mlMat4 _out, const mlMat4 _in)
{
	mlMat4 m;
	
	m[ 0] = _in[ 0];
	m[ 1] = _in[ 4];
	m[ 2] = _in[ 8];
	m[ 3] = _in[12];
	m[ 4] = _in[ 1];
	m[ 5] = _in[ 5];
	m[ 6] = _in[ 9];
	m[ 7] = _in[13];
	m[ 8] = _in[ 2];
	m[ 9] = _in[ 6];
	m[10] = _in[10];
	m[11] = _in[14];
	m[12] = _in[ 3];
	m[13] = _in[ 7];
	m[14] = _in[11];
	m[15] = _in[15];
	
	mlMat4_Copy(_out, m);
}


void mlMat4_Print(const mlMat4 _in)
{
	printf("%+.05f\t\t%+.05f\t\t%+.05f\t\t%+.05f\n", _in[ 0], _in[ 1], _in[ 2], _in[ 3]);
	printf("%+.05f\t\t%+.05f\t\t%+.05f\t\t%+.05f\n", _in[ 4], _in[ 5], _in[ 6], _in[ 7]);
	printf("%+.05f\t\t%+.05f\t\t%+.05f\t\t%+.05f\n", _in[ 8], _in[ 9], _in[10], _in[11]);
	printf("%+.05f\t\t%+.05f\t\t%+.05f\t\t%+.05f\n", _in[12], _in[13], _in[14], _in[15]);
}
