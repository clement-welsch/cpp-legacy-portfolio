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


#include <he/he_mesh.h>
#include <he/he_vert.h>
#include <he/he_edge.h>
#include <he/he_face.h>
#include <he/he_list.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>


static int __heMeshCounter__ = 0;
static int __heVertCounter__ = 0;
static int __heEdgeCounter__ = 0;
static int __heFaceCounter__ = 0;


int __heEdge_Find_Edge__(ptrVoid _a, ptrVoid _b)
{
	return ((hePtrEdge)_a)->head != (hePtrVert)_b;
}


hePtrFace __heFace_Build__(hePtrList _verts, hePtrList _edges)
{
	hePtrFace	  out;
	hePtrEdge	  e;

	hePtrListItem it, itf;
	hePtrVert	  vh, vt;
	
	if(_verts->size < 3)
		return NULL;
	
	out = heFace_New(__heFaceCounter__++);
	
	heList_Init(&out->edges);
	mlVec3_Zero(out->normal);

	for(it = _verts->head; it != NULL; it = it->next)
	{
		vt = (hePtrVert)it->data;
		
		if(it->next == NULL)
			vh = (hePtrVert)_verts->head->data;
		else
			vh = (hePtrVert)it->next->data;
		
		itf = heList_Find_Data(&vt->edges, vh, __heEdge_Find_Edge__);
		
		if(itf == NULL)
		{
			e  = heEdge_New(__heEdgeCounter__++, vh, vt);
			heList_Push_Back(_edges, e);
			heList_Push_Back(&vh->edges, e);
			
			e->twin = heEdge_New(__heEdgeCounter__++, vt, vh);
			e->twin->twin = e;
			e = e->twin;
			
			heList_Push_Back(_edges, e);
			heList_Push_Back(&vt->edges, e);
		}
		else
		{
			e = (hePtrEdge)itf->data;
		}
		
		heList_Push_Back(&out->edges, e);
		heList_Push_Back(&e->faces, out);
	}
	
	return out;
}


int __heMesh_LoadOBJ__(hePtrMesh _out, FILE * _file)
{
	char			buf[512], * c;
	int				i, err, v, n, t;
	int 			nv, nf, nt;
	hePtrList		edges;
	hePtrListItem	it;
	heList			face;
	hePtrVert *		currentV;
	hePtrFace *		currentF;
	mlVec2 *		texes;
	
	nv = 0;
	nf = 0;
	nt = 0;
	
	rewind(_file);
	while(fgets(buf, sizeof(buf), _file) != NULL)
	{
		if(buf[0] == 'v')
		{
			if(buf[1] == ' ' || buf[1] == '\t')
				++nv;
			else if(buf[1] == 't')
				++nt;
		}
		else if(buf[0] == 'f')
		{
			if(buf[1] == ' ' || buf[1] == '\t')
				++nf;
		}
	}
	
	_out->verts  = (hePtrVert *)malloc(sizeof(hePtrVert) * nv);
	_out->faces  = (hePtrFace *)malloc(sizeof(hePtrFace) * nf);
	texes		 = (mlVec2 *)malloc(sizeof(mlVec2) * nt);
	edges		 = heList_New();
	_out->nVerts = nv;
	_out->nFaces = nf;
	currentV	 = _out->verts;
	currentF	 = _out->faces;
	nt			 = 0;
	
	heList_Init(&face);
	
	rewind(_file);	
	while(fgets(buf, sizeof(buf), _file) != NULL)
	{
		if(buf[0] == 'v')
		{
			if(buf[1] == ' ' || buf[1] == '\t')
			{
				*currentV = heVert_New(__heVertCounter__++);
				
				if(sscanf(buf + 1, "%lf %lf %lf", &(*currentV)->pos[0], &(*currentV)->pos[1], &(*currentV)->pos[2]) != 3)
					return -1;
				
				++currentV;
			}
			else if(buf[1] == 't')
			{
				if(sscanf(buf + 2, "%lf %lf", &texes[nt][0], &texes[nt][1]) != 2)
					return -1;
				
				++nt;
			}
		}
		else if(buf[0] == 'f')
		{
			heList_Clear(&face);
			
			c = buf;
			err = 0;
			
			while(err == 0)
			{
				while(*c && *c != '\n' && !isspace(*c))
					c++;
				
				while(*c && isspace(*c))
					c++;
				
				err = 1;
				if(sscanf(c, " %d/%d", &v, &t) == 2 || sscanf(c, " %d/%d/%d", &v, &t, &n) == 3)
				{
					err = 0;
					
					if (v < 0)
						v += _out->nVerts;
					else
						--v;
					
					mlVec2_Copy(_out->verts[v]->tex, texes[t - 1]);
				}
				else if(sscanf(c, " %d", &v) == 1 || sscanf(c, " %d//%d", &v, &n) == 2)
				{
					err = 0;
					
					if (v < 0)
						v += _out->nVerts;
					else
						--v;
				}
				
				if(err == 0)
					heList_Push_Back(&face, _out->verts[v]);
			}
			
			if((*currentF = __heFace_Build__(&face, edges)) == NULL)
				return -1;
			
			++currentF;
		}
	}
	
	_out->nEdges = edges->size;
	_out->edges  = (hePtrEdge *)malloc(sizeof(hePtrEdge) * edges->size);
	
	for(i = 0, it = edges->head; it != NULL; it = it->next, ++i)
		_out->edges[i] = (hePtrEdge)it->data;
	
	free(texes);
	heList_Free(&edges);
	return 0;
}


hePtrMesh heMesh_New()
{
	hePtrMesh out = (hePtrMesh)malloc(sizeof(heMesh));
	
	out->id		= __heMeshCounter__++;
	out->nVerts = 0;
	out->nEdges = 0;
	out->nFaces = 0;
	
	out->verts	= NULL;
	out->edges	= NULL;
	out->faces	= NULL;
	
	return out;
}


hePtrMesh heMesh_New_Copy(const hePtrMesh _in)
{
	hePtrMesh out = heMesh_New();
	
	heMesh_Copy(out, _in);
	
	return out;
}


void heMesh_Free(hePtrMesh * _out)
{
	heMesh_Clear(*_out);
	
	free(*_out);
	*_out = NULL;
}


void heMesh_Clear(hePtrMesh _out)
{
	int i;
	
	for(i = 0; i < _out->nVerts; ++i)
		heVert_Free(&_out->verts[i]);
	
	for(i = 0; i < _out->nEdges; ++i)
		heEdge_Free(&_out->edges[i]);
	
	for(i = 0; i < _out->nFaces; ++i)
		heFace_Free(&_out->faces[i]);
	
	free(_out->verts);
	free(_out->faces);
	free(_out->edges);
	
	_out->nVerts = 0;
	_out->nEdges = 0;
	_out->nFaces = 0;
	
	_out->verts	= NULL;
	_out->edges	= NULL;
	_out->faces	= NULL;
	
	__heVertCounter__ = 0;
	__heEdgeCounter__ = 0;
	__heFaceCounter__ = 0;
}


void heMesh_Copy(hePtrMesh _out, const hePtrMesh _in)
{
	int		  		i;
	hePtrList		edges;
	hePtrListItem	it;
	heList			face;
	
	heMesh_Clear(_out);
	
	_out->nVerts = _in->nVerts;
	_out->nEdges = _in->nEdges;
	_out->nFaces = _in->nFaces;
	
	edges		= heList_New();
	_out->verts	= (hePtrVert *)malloc(sizeof(hePtrVert) * _out->nVerts);
	_out->edges	= (hePtrEdge *)malloc(sizeof(hePtrEdge) * _out->nEdges);
	_out->faces	= (hePtrFace *)malloc(sizeof(hePtrFace) * _out->nFaces);
	
	for(i = 0; i < _out->nVerts; ++i)
	{
		_out->verts[i] = heVert_New(i);
		
		mlVec3_Copy(_out->verts[i]->pos, _in->verts[i]->pos);
		mlVec3_Copy(_out->verts[i]->normal, _in->verts[i]->normal);
		mlVec3_Copy(_out->verts[i]->color, _in->verts[i]->color);
		mlVec2_Copy(_out->verts[i]->tex, _in->verts[i]->tex);
	}
	
	heList_Init(&face);
	for(i = 0; i < _out->nFaces; ++i)
	{
		_out->faces[i] = heFace_New(i);
		heList_Clear(&face);
		
		for(it = _in->faces[i]->edges.head; it != NULL; it = it->next)
			heList_Push_Back(&face, _out->verts[((hePtrEdge)it->data)->tail->id]);
		
		_out->faces[i] = __heFace_Build__(&face, edges);
		mlVec3_Copy(_out->faces[i]->normal, _in->faces[i]->normal);
	}
	
	for(i = 0, it = edges->head; it != NULL; it = it->next, ++i)
		_out->edges[i] = (hePtrEdge)it->data;
	
	heList_Free(&edges);
}


int heMesh_Load(hePtrMesh _out, const char * _path)
{
	char * extension;
	FILE * file;
	
	__heVertCounter__ = 0;
	__heEdgeCounter__ = 0;
	__heFaceCounter__ = 0;
	
	if((extension = strrchr(_path, '.')) == NULL)
		return -1;
	
	if((file = fopen(_path, "rb")) == NULL)
		return -1;
	
	heMesh_Clear(_out);
	
	if(strcasecmp(extension, ".obj") == 0)
	{
		if(__heMesh_LoadOBJ__(_out, file) == 1)
		{
			fclose(file);
			
			heMesh_Clear(_out);
			return -1;
		}
		
		fclose(file);
		return 0;
	}
	
	fclose(file);
	return -1;
}


void heMesh_Normalize(hePtrMesh _out)
{
	mlVec3	size;
	mlVec3	pos, bboxmin, bboxmax;
	int		i, j;
	double	scale = 1.0;
	
	mlVec3_Copy(bboxmin, _out->verts[0]->pos);
	mlVec3_Copy(bboxmax, _out->verts[0]->pos);
	
	for(i = 1; i < _out->nVerts; ++i)
	{
		for(j = 0; j < 3; ++j)
		{
			if(_out->verts[i]->pos[j] > bboxmax[j])
				bboxmax[j] = _out->verts[i]->pos[j];
			
			if(_out->verts[i]->pos[j] < bboxmin[j])
				bboxmin[j] = _out->verts[i]->pos[j];
		}
	}
	
	mlVec3_AddVec(pos, bboxmin, bboxmax);
	mlVec3_Scale (pos, pos, 0.5);

	for(i = 0; i < 3; ++i)
		size[i] = fabs(bboxmax[i] - bboxmin[i]);
	
	if(size[0] > size[1])
	{
		if(size[0] > size[2])
			scale = 1.0 / size[0];
		else
			scale = 1.0 / size[2];
	}
	else
	{
		if(size[1] > size[2])
			scale = 1.0 / size[1];
		else
			scale = 1.0 / size[2];
	}
	
	for(i = 0; i < _out->nVerts; ++i)
	{
		mlVec3_SubVec(_out->verts[i]->pos, _out->verts[i]->pos, pos  );
		mlVec3_Scale (_out->verts[i]->pos, _out->verts[i]->pos, scale);
	}
}


void heMesh_Compute_Normals(hePtrMesh _out)
{
	int i;
	hePtrListItem it;
	hePtrEdge	  e;
	mlVec3 acc, v1, v2;
	
	for(i = 0; i < _out->nVerts; ++i)
		mlVec3_Zero(_out->verts[i]->normal);
	
	for(i = 0; i < _out->nFaces; ++i)
	{
		e = (hePtrEdge)_out->faces[i]->edges.head->data;
		mlVec3_SubVec(v1, e->head->pos, e->tail->pos);
		
		e = (hePtrEdge)_out->faces[i]->edges.tail->data;
		mlVec3_SubVec(v2, e->tail->pos, e->head->pos);
		
		mlVec3_Cross	(_out->faces[i]->normal, v1, v2);
		mlVec3_Normalize(_out->faces[i]->normal, _out->faces[i]->normal);
		
		for(it = _out->faces[i]->edges.head; it != NULL; it = it->next)
		{
			e = (hePtrEdge)it->data;
			mlVec3_AddVec(e->tail->normal, e->tail->normal, _out->faces[i]->normal);
		}
	}
	
	for(i = 0; i < _out->nVerts; ++i)
		mlVec3_Normalize(_out->verts[i]->normal, _out->verts[i]->normal);
}
