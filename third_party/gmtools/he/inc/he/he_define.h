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


#ifndef __HE_DEFINE__
#define __HE_DEFINE__


#include <ml.h>


typedef struct _heVert		 heVert;
typedef struct _heVert *	 hePtrVert;

typedef struct _heEdge		 heEdge;
typedef struct _heEdge *	 hePtrEdge;

typedef struct _heFace		 heFace;
typedef struct _heFace *	 hePtrFace;

typedef struct _heMesh		 heMesh;
typedef struct _heMesh *	 hePtrMesh;

typedef struct _heListItem	 heListItem;
typedef struct _heListItem * hePtrListItem;

typedef void *				 ptrVoid;


struct _heListItem
{
	hePtrListItem prev;
	hePtrListItem next;
	
	ptrVoid data;
};


typedef struct _heList
{
	hePtrListItem head;
	hePtrListItem tail;
	
	int size;
} heList, * hePtrList;


struct _heVert
{
	int id;
	
	mlVec3 pos;
	mlVec3 normal;
	mlVec3 color;
	mlVec2 tex;
	
	heList edges;
};


struct _heEdge
{
	int id;
	
	hePtrVert tail;
	hePtrVert head;
	hePtrEdge twin;
	
	heList faces;
};


struct _heFace
{
	int id;
	
	mlVec3 normal;
	
	heList edges;
};


struct _heMesh
{
	int id;
	
	int	nVerts;
	int	nEdges;
	int	nFaces;
	
	hePtrVert * verts;
	hePtrEdge * edges;
	hePtrFace * faces;
};


#endif
