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


#ifndef __ML_DEFINE__
#define __ML_DEFINE__


static const double mlEps = 1.0e-09;				/* Definition of epsilon */


typedef double mlVec2[ 2];							/* Definition of 2D vector as an array of 2 doubles */
typedef double mlVec3[ 3];							/* Definition of 3D vector as an array of 3 doubles */
typedef double mlVec4[ 4];							/* Definition of 4D vector as an array of 4 doubles */

typedef double mlMat2[ 4];							/* Definition of 2D matrix as an array of  4 doubles */
typedef double mlMat3[ 9];							/* Definition of 3D matrix as an array of  9 doubles */
typedef double mlMat4[16];							/* Definition of 4D matrix as an array of 16 doubles */


#endif
