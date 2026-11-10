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


#ifndef __ML_MAT4__
#define __ML_MAT4__


#include <ml/ml_define.h>


void	mlMat4_Set		(mlMat4 _out, double _a, double _b, double _c, double _d, double _e, double _f, double _g, double _h, double _i, double _j, double _k, double _l, double _m, double _n, double _o, double _p);
																																	/* Fill the _out matrix with |_a _b _c _d| */
																																    /* 							 |_e _f _g _h| */
																																    /* 							 |_i _j _k _l| */
																																    /* 							 |_m _n _o _p| */

void	mlMat4_Zero		(mlMat4 _out);																								/* Fill the _out matrix with zeros */
void	mlMat4_Identity	(mlMat4 _out);																								/* Copy the identity matrix into _out */
void	mlMat4_Copy		(mlMat4 _out, const mlMat4 _in);																			/* Copy the matrix _in into _out */


void	mlMat4_FromCols	(mlMat4 _out, const mlVec4 _col1, const mlVec4 _col2, const mlVec4 _col3, const mlVec4 _col4);				/* Copy the 4 columns vectors into _out */
void	mlMat4_FromRows	(mlMat4 _out, const mlVec4 _row1, const mlVec4 _row2, const mlVec4 _row3, const mlVec4 _row4);				/* Copy the 4 rows vectors into _out */

void	mlMat4_Add		(mlMat4 _out, const mlMat4 _in, double _k);																	/* _out = _in + _k */
void	mlMat4_Sub		(mlMat4 _out, const mlMat4 _in, double _k);																	/* _out = _in - _k */
void	mlMat4_Scale	(mlMat4 _out, const mlMat4 _in, double _k);																	/* _out = _in * _k */
void	mlMat4_Neg		(mlMat4 _out, const mlMat4 _in);																			/* _out = -_out */

void	mlMat4_AddMat	(mlMat4 _out, const mlMat4 _in1, const mlMat4 _in2);														/* _out = _in1 + _in2 */
void	mlMat4_SubMat	(mlMat4 _out, const mlMat4 _in1, const mlMat4 _in2);														/* _out = _in1 - _in2 */
void	mlMat4_MultMat	(mlMat4 _out, const mlMat4 _in1, const mlMat4 _in2);														/* _out = _in1 * _in2 */

void	mlMat4_MultVec	(mlVec4 _out, const mlMat4 _in1, const mlVec4 _in2);														/* _out = _in1 * _in2 (column vector) */

double	mlMat4_Det		(const mlMat4 _in);																							/* Compute the determinant of _in */
void	mlMat4_Inv		(mlMat4 _out, const mlMat4 _in);																			/* _out = _in^-1 */
void	mlMat4_Transpose(mlMat4 _out, const mlMat4 _in);																			/* _out = _in' */

void	mlMat4_Print	(const mlMat4 _in);																							/* Print the _in matrix */


#endif
