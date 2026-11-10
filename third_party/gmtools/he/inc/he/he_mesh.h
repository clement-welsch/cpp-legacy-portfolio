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


#ifndef __HE_MESH__
#define __HE_MESH__


#include <he/he_define.h>


hePtrMesh	heMesh_New				();											/* Create a new mesh */
hePtrMesh	heMesh_New_Copy			(const hePtrMesh _in);						/* Create a new copy of _in */
void		heMesh_Free				(hePtrMesh * _out);							/* Delete the _out mesh */

void		heMesh_Clear			(hePtrMesh _out);							/* Clear the _out mesh */
void		heMesh_Copy				(hePtrMesh _out, const hePtrMesh _in);		/* Copy _in into _out */

int			heMesh_Load				(hePtrMesh _out, const char * _path);		/* Load the file into the _out mesh */
void		heMesh_Normalize		(hePtrMesh _out);							/* Resize the _out mesh to a unit-length box */
void		heMesh_Compute_Normals	(hePtrMesh _out);							/* Compute the normals of the _out mesh faces and vertices */


#endif
