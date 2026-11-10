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


#include <ml/ml_mat2.h>
#include <ml/ml_vec2.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


void mlMat2_Set(mlMat2 _out, double _a, double _b, double _c, double _d)
{
	_out[0] = _a;
	_out[1] = _b;
	_out[2] = _c;
	_out[3] = _d;
}


void mlMat2_Zero(mlMat2 _out)
{
	mlMat2_Set(_out, 0.0, 0.0, 0.0, 0.0);
}


void mlMat2_Identity(mlMat2 _out)
{
	mlMat2_Set(_out, 1.0, 0.0, 0.0, 1.0);
}


void mlMat2_Copy(mlMat2 _out, const mlMat2 _in)
{
	mlMat2_Set(_out, _in[0], _in[1], _in[2], _in[3]);
}


void mlMat2_FromCols(mlMat2 _out, const mlVec2 _col1, const mlVec2 _col2)
{
	mlMat2_Set(_out, _col1[0], _col2[0], _col1[1], _col2[1]);
}


void mlMat2_FromRows(mlMat2 _out, const mlVec2 _row1, const mlVec2 _row2)
{
	mlMat2_Set(_out, _row1[0], _row1[1], _row2[0], _row2[1]);
}


void mlMat2_Add(mlMat2 _out, const mlMat2 _in, double _k)
{
	mlMat2_Set(_out, _in[0] + _k, _in[1] + _k, _in[2] + _k, _in[3] + _k);
}


void mlMat2_Sub(mlMat2 _out, const mlMat2 _in, double _k)
{
	mlMat2_Add(_out, _in, -_k);
}


void mlMat2_Scale(mlMat2 _out, const mlMat2 _in, double _k)
{
	mlMat2_Set(_out, _in[0] * _k, _in[1] * _k, _in[2] * _k, _in[3] * _k);
}


void mlMat2_Neg(mlMat2 _out, const mlMat2 _in)
{
	mlMat2_Set(_out, -_in[0], -_in[1], -_in[2], -_in[3]);
}


void mlMat2_AddMat(mlMat2 _out, const mlMat2 _in1, const mlMat2 _in2)
{
	mlMat2_Set(_out, _in1[0] + _in2[0], _in1[1] + _in2[1], _in1[2] + _in2[2], _in1[3] + _in2[3]);
}


void mlMat2_SubMat(mlMat2 _out, const mlMat2 _in1, const mlMat2 _in2)
{
	mlMat2_Set(_out, _in1[0] - _in2[0], _in1[1] - _in2[1], _in1[2] - _in2[2], _in1[3] - _in2[3]);
}


void mlMat2_MultMat(mlMat2 _out, const mlMat2 _in1, const mlMat2 _in2)
{
	mlMat2 m;
	
	m[0] = _in1[0] * _in2[0] + _in1[1] * _in2[2];
	m[1] = _in1[0] * _in2[1] + _in1[1] * _in2[3];
	m[2] = _in1[2] * _in2[0] + _in1[3] * _in2[2];
	m[3] = _in1[2] * _in2[1] + _in1[3] * _in2[3];
	
	mlMat2_Copy(_out, m);
}


void mlMat2_MultVec(mlVec2 _out, const mlMat2 _in1, const mlVec2 _in2)
{
	mlVec2 v;
	
	v[0] = _in1[0] * _in2[0] + _in1[1] * _in2[1];
	v[1] = _in1[2] * _in2[0] + _in1[3] * _in2[1];
	
	mlVec2_Copy(_out, v);
}


double mlMat2_Det(const mlMat2 _in)
{
	return _in[0] * _in[3] - _in[2] * _in[1];
}


void mlMat2_Inv(mlMat2 _out, const mlMat2 _in)
{
	mlMat2 m;
	double det = mlMat2_Det(_in);
	
	if(fabs(det) > mlEps)
	{
		det = 1.0 / det;
	
		m[0] =  _in[3] * det;
		m[1] = -_in[1] * det;
		m[2] = -_in[2] * det;
		m[3] =  _in[0] * det;
		
		mlMat2_Copy(_out, m);
	}
	else
	{
		mlMat2_Identity(_out);
	}
}


void mlMat2_Transpose(mlMat2 _out, const mlMat2 _in)
{
	mlMat2 m;
	
	m[0] = _in[0];
	m[1] = _in[2];
	m[2] = _in[1];
	m[3] = _in[3];
	
	mlMat2_Copy(_out, m);
}


void mlMat2_Print(const mlMat2 _in)
{
	printf("%+.05f\t\t%+.05f\n", _in[0], _in[1]);
	printf("%+.05f\t\t%+.05f\n", _in[2], _in[3]);
}
