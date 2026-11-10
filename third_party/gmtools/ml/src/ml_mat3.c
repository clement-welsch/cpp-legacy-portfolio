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


#include <ml/ml_mat3.h>
#include <ml/ml_vec3.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


void mlMat3_Set(mlMat3 _out, double _a, double _b, double _c, double _d, double _e, double _f, double _g, double _h, double _i)
{
	_out[0] = _a;
	_out[1] = _b;
	_out[2] = _c;
	_out[3] = _d;
	_out[4] = _e;
	_out[5] = _f;
	_out[6] = _g;
	_out[7] = _h;
	_out[8] = _i;
}


void mlMat3_Zero(mlMat3 _out)
{
	mlMat3_Set(_out, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
}


void mlMat3_Identity(mlMat3 _out)
{
	mlMat3_Set(_out, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
}


void mlMat3_Copy(mlMat3 _out, const mlMat3 _in)
{
	mlMat3_Set(_out, _in[0], _in[1], _in[2], _in[3], _in[4], _in[5], _in[6], _in[7], _in[8]);
}


void mlMat3_FromCols(mlMat3 _out, const mlVec3 _col1, const mlVec3 _col2, const mlVec3 _col3)
{
	mlMat3_Set(_out, _col1[0], _col2[0], _col3[0], _col1[1], _col2[1], _col3[1], _col1[2], _col2[2], _col3[2]);
}


void mlMat3_FromRows(mlMat3 _out, const mlVec3 _row1, const mlVec3 _row2, const mlVec3 _row3)
{
	mlMat3_Set(_out, _row1[0], _row1[1], _row1[2], _row2[0], _row2[1], _row2[2], _row3[0], _row3[1], _row3[2]);
}


void mlMat3_Add(mlMat3 _out, const mlMat3 _in, double _k)
{
	mlMat3_Set(_out, _in[0] + _k, _in[1] + _k, _in[2] + _k, 
					 _in[3] + _k, _in[4] + _k, _in[5] + _k, 
					 _in[6] + _k, _in[7] + _k, _in[8] + _k);
}


void mlMat3_Sub(mlMat3 _out, const mlMat3 _in, double _k)
{
	mlMat3_Add(_out, _in, -_k);
}


void mlMat3_Scale(mlMat3 _out, const mlMat3 _in, double _k)
{
	mlMat3_Set(_out, _in[0] * _k, _in[1] * _k, _in[2] * _k, 
					 _in[3] * _k, _in[4] * _k, _in[5] * _k, 
					 _in[6] * _k, _in[7] * _k, _in[8] * _k);
}


void mlMat3_Neg(mlMat3 _out, const mlMat3 _in)
{
	mlMat3_Set(_out, -_in[0], -_in[1], -_in[2], 
					 -_in[3], -_in[4], -_in[5], 
					 -_in[6], -_in[7], -_in[8]);
}


void mlMat3_AddMat(mlMat3 _out, const mlMat3 _in1, const mlMat3 _in2)
{
	mlMat3_Set(_out, _in1[0] + _in2[0], _in1[1] + _in2[1], _in1[2] + _in2[2], 
					 _in1[3] + _in2[3], _in1[4] + _in2[4], _in1[5] + _in2[5], 
					 _in1[6] + _in2[6], _in1[7] + _in2[7], _in1[8] + _in2[8]);
}


void mlMat3_SubMat(mlMat3 _out, const mlMat3 _in1, const mlMat3 _in2)
{
	mlMat3_Set(_out, _in1[0] - _in2[0], _in1[1] - _in2[1], _in1[2] - _in2[2], 
					 _in1[3] - _in2[3], _in1[4] - _in2[4], _in1[5] - _in2[5], 
					 _in1[6] - _in2[6], _in1[7] - _in2[7], _in1[8] - _in2[8]);
}


void mlMat3_MultMat(mlMat3 _out, const mlMat3 _in1, const mlMat3 _in2)
{
	mlMat3 m;

	m[0] = _in1[0] * _in2[0] + _in1[1] * _in2[3] + _in1[2] * _in2[6];
	m[1] = _in1[0] * _in2[1] + _in1[1] * _in2[4] + _in1[2] * _in2[7];
	m[2] = _in1[0] * _in2[2] + _in1[1] * _in2[5] + _in1[2] * _in2[8];
	m[3] = _in1[3] * _in2[0] + _in1[4] * _in2[3] + _in1[5] * _in2[6];
	m[4] = _in1[3] * _in2[1] + _in1[4] * _in2[4] + _in1[5] * _in2[7];
	m[5] = _in1[3] * _in2[2] + _in1[4] * _in2[5] + _in1[5] * _in2[8];
	m[6] = _in1[6] * _in2[0] + _in1[7] * _in2[3] + _in1[8] * _in2[6];
	m[7] = _in1[6] * _in2[1] + _in1[7] * _in2[4] + _in1[8] * _in2[7];
	m[8] = _in1[6] * _in2[2] + _in1[7] * _in2[5] + _in1[8] * _in2[8];
	
	mlMat3_Copy(_out, m);
}


void mlMat3_MultVec(mlVec3 _out, const mlMat3 _in1, const mlVec3 _in2)
{
	mlVec3 v;
	
	v[0] = _in1[0] * _in2[0] + _in1[1] * _in2[1] + _in1[2] * _in2[2];
	v[1] = _in1[3] * _in2[0] + _in1[4] * _in2[1] + _in1[5] * _in2[2];
	v[2] = _in1[6] * _in2[0] + _in1[7] * _in2[1] + _in1[8] * _in2[2];
	
	mlVec3_Copy(_out, v);
}


double mlMat3_Det(const mlMat3 _in)
{
	return _in[0] * (_in[4] * _in[8] - _in[7] * _in[5]) -
		   _in[1] * (_in[3] * _in[8] - _in[6] * _in[5]) +
		   _in[2] * (_in[3] * _in[7] - _in[6] * _in[4]);
}


void mlMat3_Inv(mlMat3 _out, const mlMat3 _in)
{
	mlMat3 m;
	double det = mlMat3_Det(_in);
	
	if(fabs(det) > mlEps)
	{
		det = 1.0 / det;
	
		m[0] =  (_in[4] * _in[8] - _in[7] * _in[5]) * det;
		m[1] = -(_in[1] * _in[8] - _in[7] * _in[2]) * det;
		m[2] =  (_in[1] * _in[5] - _in[4] * _in[2]) * det;
		m[3] = -(_in[3] * _in[8] - _in[6] * _in[5]) * det;
		m[4] =  (_in[0] * _in[8] - _in[6] * _in[2]) * det;
		m[5] = -(_in[0] * _in[5] - _in[3] * _in[2]) * det;
		m[6] =  (_in[3] * _in[7] - _in[6] * _in[4]) * det;
		m[7] = -(_in[0] * _in[7] - _in[6] * _in[1]) * det;
		m[8] =  (_in[0] * _in[4] - _in[3] * _in[1]) * det;
		
		mlMat3_Copy(_out, m);
	}
	else
	{
		mlMat3_Identity(_out);
	}
}


void mlMat3_Transpose(mlMat3 _out, const mlMat3 _in)
{
	mlMat3 m;
	
	m[0] = _in[0];
	m[1] = _in[3];
	m[2] = _in[6];
	m[3] = _in[1];
	m[4] = _in[4];
	m[5] = _in[7];
	m[6] = _in[2];
	m[7] = _in[5];
	m[8] = _in[8];

	mlMat3_Copy(_out, m);;
}


void mlMat3_Print(const mlMat3 _in)
{
	printf("%+.05f\t\t%+.05f\t\t%+.05f\n", _in[0], _in[1], _in[2]);
	printf("%+.05f\t\t%+.05f\t\t%+.05f\n", _in[3], _in[4], _in[5]);
	printf("%+.05f\t\t%+.05f\t\t%+.05f\n", _in[6], _in[7], _in[8]);
}
