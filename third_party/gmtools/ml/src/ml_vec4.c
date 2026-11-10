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


#include <ml/ml_vec4.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


void mlVec4_Set(mlVec4 _in, double _w, double _x, double _y, double _z)
{
	_in[0] = _w;
	_in[1] = _x;
	_in[2] = _y;
	_in[3] = _z;
}


void mlVec4_Zero(mlVec4 _out)
{
	mlVec4_Set(_out, 0.0, 0.0, 0.0, 0.0);
}


void mlVec4_Copy(mlVec4 _out, const mlVec4 _in)
{
	mlVec4_Set(_out, _in[0], _in[1], _in[2], _in[3]);
}


void mlVec4_Add(mlVec4 _out, const mlVec4 _in, double _k)
{
	mlVec4_Set(_out, _in[0] + _k, _in[1] + _k, _in[2] + _k, _in[3] + _k);
}


void mlVec4_Sub(mlVec4 _out, const mlVec4 _in, double _k)
{
	mlVec4_Add(_out, _in, -_k);
}


void mlVec4_Scale(mlVec4 _out, const mlVec4 _in, double _k)
{
	mlVec4_Set(_out, _in[0] * _k, _in[1] * _k, _in[2] * _k, _in[3] * _k);
}


void mlVec4_Neg(mlVec4 _out, const mlVec4 _in)
{
	mlVec4_Set(_out, -_in[0], -_in[1], -_in[2], -_in[3]);
}


void mlVec4_AddVec(mlVec4 _out, const mlVec4 _in1, const mlVec4 _in2)
{
	mlVec4_Set(_out, _in1[0] + _in2[0], _in1[1] + _in2[1], _in1[2] + _in2[2], _in1[3] + _in2[3]);
}


void mlVec4_SubVec(mlVec4 _out, const mlVec4 _in1, const mlVec4 _in2)
{
	mlVec4_Set(_out, _in1[0] - _in2[0], _in1[1] - _in2[1], _in1[2] - _in2[2], _in1[3] - _in2[3]);
}


void mlVec4_MultMat(mlVec4 _out, const mlVec4 _in1, const mlMat4 _in2)
{
	mlVec4 v;
	
	v[0] = _in1[0] * _in2[0] + _in1[1] * _in2[4] + _in1[2] * _in2[ 8] + _in1[3] * _in2[12];
	v[1] = _in1[0] * _in2[1] + _in1[1] * _in2[5] + _in1[2] * _in2[ 9] + _in1[3] * _in2[13];
	v[2] = _in1[0] * _in2[2] + _in1[1] * _in2[6] + _in1[2] * _in2[10] + _in1[3] * _in2[14];
	v[3] = _in1[0] * _in2[3] + _in1[1] * _in2[7] + _in1[2] * _in2[11] + _in1[3] * _in2[15];
	
	mlVec4_Copy(_out, v);
}


void mlVec4_Normalize(mlVec4 _out, const mlVec4 _in)
{
	double norm = mlVec4_Norm(_in);
	
	if(fabs(norm) > mlEps)
		mlVec4_Scale(_out, _in, 1.0 / norm);
	else
		mlVec4_Set(_out, 1.0, 0.0, 0.0, 0.0);
}


double mlVec4_Norm(const mlVec4 _in)
{
	return sqrt(mlVec4_Norm2(_in));
}


double mlVec4_Norm2(const mlVec4 _in)
{
	return _in[0] * _in[0] + _in[1] * _in[1] + _in[2] * _in[2] + _in[3] * _in[3];
}


double mlVec4_Dot(const mlVec4 _in1, const mlVec4 _in2)
{
	return _in1[0] * _in2[0] + _in1[1] * _in2[1] + _in1[2] * _in2[2] + _in1[3] * _in2[3];
}

void mlVec4_Print(const mlVec4 _in)
{
	printf("%+.05f\t\t%+.05f\t\t%+.05f\t\t%+.05f\n", _in[0], _in[1], _in[2], _in[3]);
}
