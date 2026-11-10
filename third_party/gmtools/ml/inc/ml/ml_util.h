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


#ifndef __ML_UTIL__
#define __ML_UTIL__


#include <ml/ml_define.h>


double	mlDistance2(mlVec2 _a, mlVec2 _b);						/* Compute the distance in 2D between _a and _b */
double	mlDistance3(mlVec3 _a, mlVec3 _b);						/* Compute the distance in 3D between _a and _b */
double	mlDistance4(mlVec4 _a, mlVec4 _b);						/* Compute the distance in 4D between _a and _b */

double	mlMax2	   (double _a, double _b);						/* Compute the max between _a and _b */
double	mlMax3	   (double _a, double _b, double _c);			/* Compute the max between _a, _b and _c */
double	mlMin2	   (double _a, double _b);						/* Compute the min between _a and _b */
double	mlMin3	   (double _a, double _b, double _c);			/* Compute the min between _a, _b and _c */

void	mlSwap	   (double * _a, double * _b);					/* Swap _a and _b */

double	mlSquare   (double _x);									/* Compute _x * _x */
double	mlCube	   (double _x);									/* Compute _x * _x * _x */

int		mlSign	   (double _x);									/* Return 1 if _x > 0, -1 otherwise */
int		mlNearZero (double _x);									/* Return 1 if _x =~ 0, 0 otherwise */
int		mlNearValue(double _x, double _a);						/* Return 1 if _x =~ -_a, 0 otherwise */

double	mlClamp	   (double _x, double _a, double _b);			/* Return _a if _x <_a, _b if _x > _b, _x otherwise */ 		

double	mlDegToRad (double _x);									/* Convert _x from degrees to radians */
double	mlRadToDeg (double _x);									/* Convert _x from radians to degress */


#endif
