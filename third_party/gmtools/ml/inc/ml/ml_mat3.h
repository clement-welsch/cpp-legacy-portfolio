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


#ifndef __ML_MAT3__
#define __ML_MAT3__


#include <ml/ml_define.h>


void	mlMat3_Set		(mlMat3 _out, double _a, double _b, double _c, double _d, double _e, double _f, double _g, double _h, double _i);
																												/* Fill the _out matrix with |_a _b _c| */
																												/* 							 |_d _e _f| */
																												/* 							 |_g _h _i| */

void	mlMat3_Zero		(mlMat3 _out);																			/* Fill the _out matrix with zeros */
void	mlMat3_Identity	(mlMat3 _out);																			/* Copy the identity matrix into _out */
void	mlMat3_Copy		(mlMat3 _out, const mlMat3 _in);														/* Copy the matrix _in into _out */


void	mlMat3_FromCols	(mlMat3 _out, const mlVec3 _col1, const mlVec3 _col2, const mlVec3 _col3);				/* Copy the 3 columns vectors into _out */
void	mlMat3_FromRows	(mlMat3 _out, const mlVec3 _row1, const mlVec3 _row2, const mlVec3 _row3);				/* Copy the 3 rows vectors into _out */

void	mlMat3_Add		(mlMat3 _out, const mlMat3 _in, double _k);												/* _out = _in + _k */
void	mlMat3_Sub		(mlMat3 _out, const mlMat3 _in, double _k);												/* _out = _in - _k */
void	mlMat3_Scale	(mlMat3 _out, const mlMat3 _in, double _k);												/* _out = _in * _k */
void	mlMat3_Neg		(mlMat3 _out, const mlMat3 _in);														/* _out = -_out */

void	mlMat3_AddMat	(mlMat3 _out, const mlMat3 _in1, const mlMat3 _in2);									/* _out = _in1 + _in2 */
void	mlMat3_SubMat	(mlMat3 _out, const mlMat3 _in1, const mlMat3 _in2);									/* _out = _in1 - _in2 */
void	mlMat3_MultMat	(mlMat3 _out, const mlMat3 _in1, const mlMat3 _in2);									/* _out = _in1 * _in2 */

void	mlMat3_MultVec	(mlVec3 _out, const mlMat3 _in1, const mlVec3 _in2);									/* _out = _in1 * _in2 (column vector) */

double	mlMat3_Det		(const mlMat3 _in);																		/* Compute the determinant of _in */
void	mlMat3_Inv		(mlMat3 _out, const mlMat3 _in);														/* _out = _in^-1 */
void	mlMat3_Transpose(mlMat3 _out, const mlMat3 _in);														/* _out = _in' */

void	mlMat3_Print	(const mlMat3 _in);																		/* Print the _in matrix */


#endif
