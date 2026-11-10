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


#ifndef __ML_MAT2__
#define __ML_MAT2__


#include <ml/ml_define.h>


void	mlMat2_Set		(mlMat2 _out, double _a, double _b, double _c, double _d);
																							/* Fill the _out matrix with |_a _b| */
																							/* 							 |_c _d| */

void	mlMat2_Zero		(mlMat2 _out);														/* Fill the _out matrix with zeros */
void	mlMat2_Identity	(mlMat2 _out);														/* Copy the identity matrix into _out */
void	mlMat2_Copy		(mlMat2 _out, const mlMat2 _in);									/* Copy the matrix _in into _out */


void	mlMat2_FromCols	(mlMat2 _out, const mlVec2 _col1, const mlVec2 _col2);				/* Copy the 2 columns vectors into _out */
void	mlMat2_FromRows	(mlMat2 _out, const mlVec2 _row1, const mlVec2 _row2);				/* Copy the 2 rows vectors into _out */

void	mlMat2_Add		(mlMat2 _out, const mlMat2 _in, double _k);							/* _out = _in + _k */
void	mlMat2_Sub		(mlMat2 _out, const mlMat2 _in, double _k);							/* _out = _in - _k */
void	mlMat2_Scale	(mlMat2 _out, const mlMat2 _in, double _k);							/* _out = _in * _k */
void	mlMat2_Neg		(mlMat2 _out, const mlMat2 _in);									/* _out = -_out */

void	mlMat2_AddMat	(mlMat2 _out, const mlMat2 _in1, const mlMat2 _in2);				/* _out = _in1 + _in2 */
void	mlMat2_SubMat	(mlMat2 _out, const mlMat2 _in1, const mlMat2 _in2);				/* _out = _in1 - _in2 */
void	mlMat2_MultMat	(mlMat2 _out, const mlMat2 _in1, const mlMat2 _in2);				/* _out = _in1 * _in2 */

void	mlMat2_MultVec	(mlVec2 _out, const mlMat2 _in1, const mlVec2 _in2);				/* _out = _in1 * _in2 (column vector) */

double	mlMat2_Det		(const mlMat2 _in);													/* Compute the determinant of _in */
void	mlMat2_Inv		(mlMat2 _out, const mlMat2 _in);									/* _out = _in^-1 */
void	mlMat2_Transpose(mlMat2 _out, const mlMat2 _in);									/* _out = _in' */

void	mlMat2_Print	(const mlMat2 _in);													/* Print the _in matrix */


#endif
