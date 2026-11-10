/********************************************************************************
*    ___  ___            _       _      _  _                                    *
*    |  \/  |           | |     | |    (_)| |                                   *
*    | .  . |  ___  ___ | |__   | |     _ | |__   _ __   __ _  _ __  _   _      *
*    | |\/| | / _ \/ __|| '_ \  | |    | || '_ \ | '__| / _` || '__|| | | |     *
*    | |  | ||  __/\__ \| | | | | |____| || |_) || |   | (_| || |   | |_| |     *
*    \_|  |_/ \___||___/|_| |_| \_____/|_||_.__/ |_|    \__,_||_|    \__, |     *
*                                                                   __/ |       *
*  Copyright (c) 2010                                              |___/        *
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


#ifndef __HE_VERT__
#define __HE_VERT__


#include <he/he_define.h>


hePtrVert	heVert_New (int _id);					/* Create a new vertex */
void		heVert_Free(hePtrVert * _out);			/* Delete the _out vertex */


#endif
