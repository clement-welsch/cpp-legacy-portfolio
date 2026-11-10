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


#include <ml/ml_vec3.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


void mlVec3_Set(mlVec3 _in, double _x, double _y, double _z)
{
	_in[0] = _x;
	_in[1] = _y;
	_in[2] = _z;
}


void mlVec3_Zero(mlVec3 _out)
{
	mlVec3_Set(_out, 0.0, 0.0, 0.0);
}


void mlVec3_Copy(mlVec3 _out, const mlVec3 _in)
{
	mlVec3_Set(_out, _in[0], _in[1], _in[2]);
}


void mlVec3_Add(mlVec3 _out, const mlVec3 _in, double _k)
{
	mlVec3_Set(_out, _in[0] + _k, _in[1] + _k, _in[2] + _k);
}


void mlVec3_Sub(mlVec3 _out, const mlVec3 _in, double _k)
{
	mlVec3_Add(_out, _in, -_k);
}


void mlVec3_Scale(mlVec3 _out, const mlVec3 _in, double _k)
{
	mlVec3_Set(_out, _in[0] * _k, _in[1] * _k, _in[2] * _k);
}


void mlVec3_Neg(mlVec3 _out, const mlVec3 _in)
{
	mlVec3_Set(_out, -_in[0], -_in[1], -_in[2]);
}


void mlVec3_AddVec(mlVec3 _out, const mlVec3 _in1, const mlVec3 _in2)
{
	mlVec3_Set(_out, _in1[0] + _in2[0], _in1[1] + _in2[1], _in1[2] + _in2[2]);
}


void mlVec3_SubVec(mlVec3 _out, const mlVec3 _in1, const mlVec3 _in2)
{
	mlVec3_Set(_out, _in1[0] - _in2[0], _in1[1] - _in2[1], _in1[2] - _in2[2]);
}


void mlVec3_MultMat(mlVec3 _out, const mlVec3 _in1, const mlMat3 _in2)
{
	mlVec3 v;
	
	v[0] = _in1[0] * _in2[0] + _in1[1] * _in2[4] + _in1[2] * _in2[7];
	v[1] = _in1[0] * _in2[1] + _in1[1] * _in2[5] + _in1[2] * _in2[8];
	v[2] = _in1[0] * _in2[2] + _in1[1] * _in2[6] + _in1[2] * _in2[9];
	
	mlVec3_Copy(_out, v);
}


void mlVec3_Normalize(mlVec3 _out, const mlVec3 _in)
{
	double norm = mlVec3_Norm(_in);
	
	if(fabs(norm) > mlEps)
		mlVec3_Scale(_out, _in, 1.0 / norm);
	else
		mlVec3_Set(_out, 1.0, 0.0, 0.0);
}


double mlVec3_Norm(const mlVec3 _in)
{
	return sqrt(mlVec3_Norm2(_in));
}


double mlVec3_Norm2(const mlVec3 _in)
{
	return _in[0] * _in[0] + _in[1] * _in[1] + _in[2] * _in[2];
}


double mlVec3_Dot(const mlVec3 _in1, const mlVec3 _in2)
{
	return _in1[0] * _in2[0] + _in1[1] * _in2[1] + _in1[2] * _in2[2];
}


void mlVec3_Cross(mlVec3 _out, const mlVec3 _in1, const mlVec3 _in2)
{
	mlVec3 v;
	
	v[0] = _in1[1] * _in2[2] - _in1[2] * _in2[1];
	v[1] = _in1[2] * _in2[0] - _in1[0] * _in2[2];
	v[2] = _in1[0] * _in2[1] - _in1[1] * _in2[0];
	
	mlVec3_Copy(_out, v);
}


void mlVec3_Print(const mlVec3 _in)
{
	printf("%+.05f\t\t%+.05f\t\t%+.05f\n", _in[0], _in[1], _in[2]);
}
