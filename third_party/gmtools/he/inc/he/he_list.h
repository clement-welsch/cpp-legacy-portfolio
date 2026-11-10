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


#ifndef __HE_LIST__
#define __HE_LIST__


#include <he/he_define.h>


hePtrList		heList_New		 ();																				/* Create a new list */
hePtrList		heList_New_Copy	 (const hePtrList _in);																/* Create a new copy of the _in list */
void			heList_Free		 (hePtrList * _out);																/* Delete the _out list */

void			heList_Init		 (hePtrList _out);																	/* Initialize the _out list */
void			heList_Clear	 (hePtrList _out);																	/* Clear the _out list */
void			heList_Copy		 (hePtrList _out, const hePtrList _in);												/* Copy _in into _out */

void			heList_Push_Front(hePtrList _out, ptrVoid _data);													/* Add a new item to the front of _out */
void			heList_Push_Back (hePtrList _out, ptrVoid _data);													/* Add a new item to the back of _out */
int				heList_Del_Item	 (hePtrList _out, ptrVoid _data);													/* Remove the item corresponding to _data */

hePtrListItem	heList_Find_Item (hePtrList _out, ptrVoid _data);													/* Find the item corresponding to _data in _out*/
hePtrListItem	heList_Find_Data (hePtrList _out, ptrVoid _data, int (*_compare)(ptrVoid _a, ptrVoid _b));			/* Find a particular item of _out*/

int				heList_Size		 (const hePtrList _in);																/* Return the size of the _in list */


#endif
