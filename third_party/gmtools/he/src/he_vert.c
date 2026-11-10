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


#include <he/he_vert.h>
#include <he/he_list.h>
#include <ml.h>
#include <stdlib.h>


hePtrVert heVert_New(int _id)
{
	hePtrVert out = (hePtrVert)malloc(sizeof(heVert));
	
	out->id = _id;
	
	mlVec3_Zero(out->pos   );
	mlVec3_Zero(out->color );
	mlVec3_Zero(out->normal);
	mlVec2_Zero(out->tex   );
	
	heList_Init(&out->edges);
	
	return out;
}


void heVert_Free(hePtrVert * _out)
{
	heList_Clear(&(*_out)->edges);
	free(*_out);
}
