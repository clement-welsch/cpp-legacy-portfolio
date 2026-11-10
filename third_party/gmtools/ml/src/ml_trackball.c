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


#include <ml/ml_util.h>
#include <ml/ml_vec3.h>
#include <ml/ml_vec4.h>
#include <ml/ml_mat4.h>
#include <ml/ml_trackball.h>
#include <math.h>
#include <stdio.h>


static int		mlTbW		= 0;
static int		mlTbH		= 0;
static int		mlTbX		= 0;
static int		mlTbY		= 0;
static mlVec4	mlTbQuat;
static mlMat4	mlTbMatrix;


void __mlTbQuatNormalize__(mlVec4 _qout, mlVec4 _qin)
{
	mlVec4 r;
	double n = mlVec4_Norm(_qin);
	
	if(n < mlEps)
	{
		mlVec4_Zero(r);
		r[0] = 1.0;
	}
	else
	{
		mlVec4_Scale(r, _qin, 1.0 / n);
	}
	
	mlVec4_Copy(_qout, r);
}


void __mlTbToSphere__(mlVec3 _v, int _x, int _y)
{
	_v[0] = (2.0 * _x - mlTbW) / mlTbW;
	_v[1] = (mlTbH - 2.0 * _y) / mlTbH;
	_v[2] = 1.0 - mlSquare(_v[0]) - mlSquare(_v[1]);
	
	if(_v[2] > 0.0)
		_v[2] = sqrt(_v[2]);
	else
		_v[2] = 0.0;
	
	mlVec3_Normalize(_v, _v);
}


void __mlTbToQuat__(mlVec4 _q, int _ox, int _oy, int _nx, int _ny)
{
	mlVec3 ov, nv, axis;
	double cosx, sinx;
	
	__mlTbToSphere__(ov, _ox, _oy);
	__mlTbToSphere__(nv, _nx, _ny);
	
	mlVec3_Cross(axis, nv, ov);
	mlVec3_Normalize(axis, axis);
	
	cosx = mlVec3_Dot(ov, nv);
	cosx = mlClamp(cosx, -1.0, 1.0);
	sinx = sqrt(1.0 - mlSquare(cosx));
	
	_q[0] = cosx;
	_q[1] = sinx * axis[0];
	_q[2] = sinx * axis[1];
	_q[3] = sinx * axis[2];
}


void __mlTbToMatrix__(mlMat4 _m, mlVec4 _q)
{
	double xw = _q[1] * _q[0];
	double xx = _q[1] * _q[1];
	double xy = _q[1] * _q[2];
	double xz = _q[1] * _q[3];
	
	double yw = _q[2] * _q[0];
	double yy = _q[2] * _q[2];
	double yz = _q[2] * _q[3];
	
	double zw = _q[3] * _q[0];
	double zz = _q[3] * _q[3];
	
	_m[ 0] = 1.0 - 2.0 * (yy + zz);
	_m[ 1] = 	   2.0 * (xy - zw);
	_m[ 2] = 	   2.0 * (xz + yw);
	_m[ 3] = 0.0;

	_m[ 4] = 	   2.0 * (xy + zw);
	_m[ 5] = 1.0 - 2.0 * (xx + zz);
	_m[ 6] = 	   2.0 * (yz - xw);
	_m[ 7] = 0.0;

	_m[ 8] = 	   2.0 * (xz - yw);
	_m[ 9] = 	   2.0 * (yz + xw);
	_m[10] = 1.0 - 2.0 * (xx + yy);
	_m[11] = 0.0;

	_m[12] = 0.0;
	_m[13] = 0.0;
	_m[14] = 0.0;
	_m[15] = 1.0;
}


void __mlTbUpdate__(int _nx, int _ny)
{
	mlVec4 q, r;
	
	__mlTbToQuat__(q, mlTbX, mlTbY, _nx, _ny);
	__mlTbQuatNormalize__(q, q);
	
	r[0] = mlTbQuat[0] * q[0] - mlTbQuat[1] * q[1] - mlTbQuat[2] * q[2] - mlTbQuat[3] * q[3];
	r[1] = mlTbQuat[0] * q[1] + mlTbQuat[1] * q[0] + mlTbQuat[2] * q[3] - mlTbQuat[3] * q[2];
	r[2] = mlTbQuat[0] * q[2] - mlTbQuat[1] * q[3] + mlTbQuat[2] * q[0] + mlTbQuat[3] * q[1];
	r[3] = mlTbQuat[0] * q[3] + mlTbQuat[1] * q[2] - mlTbQuat[2] * q[1] + mlTbQuat[3] * q[0];
	
	mlVec4_Copy(mlTbQuat, r);
	__mlTbToMatrix__(mlTbMatrix, mlTbQuat);
	
	mlTbX = _nx;
	mlTbY = _ny;
}


void mlTbInit(int _w, int _h)
{
	mlTbW = _w;
	mlTbH = _h;
	mlTbX = 0;
	mlTbY = 0;
	
	mlVec4_Zero(mlTbQuat);
	mlTbQuat[0] = 1.0;
	
	__mlTbToMatrix__(mlTbMatrix, mlTbQuat);
}


void mlTbClick(int _x, int _y)
{
	mlTbX = _x;
	mlTbY = _y;
	
	__mlTbUpdate__(_x, _y);
}


void mlTbMotion(int _x, int _y)
{
	__mlTbUpdate__(_x, _y);
}


void mlTbRelease(int _x, int _y)
{
	__mlTbUpdate__(_x, _y);
}


double * mlTbGetRotation()
{
	return mlTbMatrix;
}
