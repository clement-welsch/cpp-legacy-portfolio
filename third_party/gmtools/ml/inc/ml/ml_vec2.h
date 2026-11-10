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


#ifndef __ML_VEC2__
#define __ML_VEC2__


#include <ml/ml_define.h>


void	mlVec2_Set		(mlVec2 _out, double _x, double _y);							/* Fill the _out vector with |_x _y| */
void	mlVec2_Zero		(mlVec2 _out);													/* Fill the _out vector with zeros */
void	mlVec2_Copy		(mlVec2 _out, const mlVec2 _in);								/* Copy the _in vector into _out */

void	mlVec2_Add		(mlVec2 _out, const mlVec2 _in, double _k);						/* _out = _in + _k */
void	mlVec2_Sub		(mlVec2 _out, const mlVec2 _in, double _k);						/* _out = _in - _k */
void	mlVec2_Scale	(mlVec2 _out, const mlVec2 _in, double _k);						/* _out = _in * _k */
void	mlVec2_Neg		(mlVec2 _out, const mlVec2 _in);								/* _out = -_in */

void	mlVec2_AddVec	(mlVec2 _out, const mlVec2 _in1, const mlVec2 _in2);			/* _out = _in1 + _in2 */
void	mlVec2_SubVec	(mlVec2 _out, const mlVec2 _in1, const mlVec2 _in2);			/* _out = _in1 - _in2 */

void	mlVec2_MultMat	(mlVec2 _out, const mlVec2 _in1, const mlMat2 _in2);			/* _out = _in1 (row vector) * _in2 */

void	mlVec2_Normalize(mlVec2 _out, const mlVec2 _in);								/* _out = _in / ||_in|| */
double	mlVec2_Norm		(const mlVec2 _in);												/* Return ||_in|| */
double	mlVec2_Norm2	(const mlVec2 _in);												/* Return ||_in|| * ||_in|| */
double	mlVec2_Dot		(const mlVec2 _in1, const mlVec2 _in2);							/* Return _in1 . _in2 */

void	mlVec2_Print	(const mlVec2 _in);												/* Print the _in vector */


#endif
