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


#include <ml/ml_vec2.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


void mlVec2_Set(mlVec2 _in, double _x, double _y)
{
	_in[0] = _x;
	_in[1] = _y;
}


void mlVec2_Zero(mlVec2 _out)
{
	mlVec2_Set(_out, 0.0, 0.0);
}


void mlVec2_Copy(mlVec2 _out, const mlVec2 _in)
{
	mlVec2_Set(_out, _in[0], _in[1]);
}


void mlVec2_Add(mlVec2 _out, const mlVec2 _in, double _k)
{
	mlVec2_Set(_out, _in[0] + _k, _in[1] + _k);
}


void mlVec2_Sub(mlVec2 _out, const mlVec2 _in, double _k)
{
	mlVec2_Add(_out, _in, -_k);
}


void mlVec2_Scale(mlVec2 _out, const mlVec2 _in, double _k)
{
	mlVec2_Set(_out, _in[0] * _k, _in[1] * _k);
}


void mlVec2_Neg(mlVec2 _out, const mlVec2 _in)
{
	mlVec2_Set(_out, -_in[0], -_in[1]);
}


void mlVec2_AddVec(mlVec2 _out, const mlVec2 _in1, const mlVec2 _in2)
{
	mlVec2_Set(_out, _in1[0] + _in2[0], _in1[1] + _in2[1]);
}


void mlVec2_SubVec(mlVec2 _out, const mlVec2 _in1, const mlVec2 _in2)
{
	mlVec2_Set(_out, _in1[0] - _in2[0], _in1[1] - _in2[1]);
}


void mlVec2_MultMat(mlVec2 _out, const mlVec2 _in1, const mlMat2 _in2)
{
	mlVec2 v;
	
	v[0] = _in1[0] * _in2[0] + _in1[1] * _in2[2];
	v[1] = _in1[0] * _in2[1] + _in1[1] * _in2[3];
	
	mlVec2_Copy(_out, v);
}


void mlVec2_Normalize(mlVec2 _out, const mlVec2 _in)
{
	double norm = mlVec2_Norm(_in);
	
	if(fabs(norm) > mlEps)
		mlVec2_Scale(_out, _in, 1.0 / norm);
	else
		mlVec2_Set(_out, 1.0, 0.0);
}


double mlVec2_Norm(const mlVec2 _in)
{
	return sqrt(mlVec2_Norm2(_in));
}


double mlVec2_Norm2(const mlVec2 _in)
{
	return _in[0] * _in[0] + _in[1] * _in[1];
}


double mlVec2_Dot(const mlVec2 _in1, const mlVec2 _in2)
{
	return _in1[0] * _in2[0] + _in1[1] * _in2[1];
}


void mlVec2_Print(const mlVec2 _in)
{
	printf("%+.05f\t\t%+.05f\n", _in[0], _in[1]);
}
