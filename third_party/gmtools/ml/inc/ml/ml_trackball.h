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


#ifndef __ML_TRACKBALL__
#define __ML_TRACKBALL__


#include <ml/ml_define.h>


void 		mlTbInit			(int _w, int _h);									/* Initialize the trackball with the dimensions of the current window */

void 		mlTbClick			(int _x, int _y);									/* Record a simple mouse click event at the point (x, y) */
void 		mlTbMotion			(int _x, int _y);									/* Rotate the object according to the new coordinates (x, y) */
void 		mlTbRelease			(int _x, int _y);									/* Record a simple mouse release event at the point (x, y) */

double * 	mlTbGetRotation		();													/* Return a pointer to the rotation matrix (to send to the glMultMatrixd(...) procedure) */


#endif
