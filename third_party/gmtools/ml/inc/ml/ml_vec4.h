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


#ifndef __ML_VEC4__
#define __ML_VEC4__


#include <ml/ml_define.h>


void	mlVec4_Set		(mlVec4 _out, double _w, double _x, double _y, double _z);		/* Fill the _out vector with |_w _x _y _z| */
void	mlVec4_Zero		(mlVec4 _out);													/* Fill the _out vector with zeros */
void	mlVec4_Copy		(mlVec4 _out, const mlVec4 _in);								/* Copy the _in vector into _out */

void	mlVec4_Add		(mlVec4 _out, const mlVec4 _in, double _k);						/* _out = _in + _k */
void	mlVec4_Sub		(mlVec4 _out, const mlVec4 _in, double _k);						/* _out = _in - _k */
void	mlVec4_Scale	(mlVec4 _out, const mlVec4 _in, double _k);						/* _out = _in * _k */
void	mlVec4_Neg		(mlVec4 _out, const mlVec4 _in);								/* _out = -_in */

void	mlVec4_AddVec	(mlVec4 _out, const mlVec4 _in1, const mlVec4 _in2);			/* _out = _in1 + _in2 */
void	mlVec4_SubVec	(mlVec4 _out, const mlVec4 _in1, const mlVec4 _in2);			/* _out = _in1 - _in2 */

void	mlVec4_MultMat	(mlVec4 _out, const mlVec4 _in1, const mlMat4 _in2);			/* _out = _in1 (row vector) * _in2 */

void	mlVec4_Normalize(mlVec4 _out, const mlVec4 _in);								/* _out = _in / ||_in|| */
double	mlVec4_Norm		(const mlVec4 _in);												/* Return ||_in|| */
double	mlVec4_Norm2	(const mlVec4 _in);												/* Return ||_in|| * ||_in|| */
double	mlVec4_Dot		(const mlVec4 _in1, const mlVec4 _in2);							/* Return _in1 . _in2 */

void	mlVec4_Print	(const mlVec4 _in);												/* Print the _in vector */


#endif
