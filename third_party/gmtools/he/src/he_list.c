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


#include <he/he_list.h>
#include <stdlib.h>
#include <stdio.h>


hePtrListItem __heListItem_New__(ptrVoid * _data)
{
	hePtrListItem out = (hePtrListItem)malloc(sizeof(heListItem));
	
	out->prev = NULL;
	out->next = NULL;
	out->data = _data;
	
	return out;
}


hePtrList heList_New()
{
	hePtrList out = (hePtrList)malloc(sizeof(heList));
	
	heList_Init(out);
	
	return out;
}


hePtrList heList_New_Copy(const hePtrList _in)
{
	hePtrList out = heList_New();
	
	heList_Copy(out, _in);
	
	return out;
}


void heList_Free(hePtrList * _out)
{
	heList_Clear(*_out);
	
	free(*_out);
	
	_out = NULL;
}


void heList_Init(hePtrList _out)
{
	_out->head = NULL;
	_out->tail = NULL;
	_out->size = 0;
}


void heList_Clear(hePtrList _out)
{
	hePtrListItem it, del;
	
	for(it = _out->head; it != NULL; )
	{
		del = it;
		it	= it->next;
		
		free(del);
	}
	
	_out->head = NULL;
	_out->tail = NULL;
	_out->size = 0;
}


void heList_Copy(hePtrList _out, const hePtrList _in)
{
	hePtrListItem it;
	
	heList_Clear(_out);
	for(it = _in->head; it != NULL; it = it->next)
	{
		heList_Push_Back(_out, it->data);
	}
}


void heList_Push_Front(hePtrList _out, ptrVoid _data)
{
	hePtrListItem item = __heListItem_New__(_data);
	
	item->next = _out->head;
	item->prev = NULL;
	
	if(_out->tail == NULL)
		_out->tail = item;
	else
		_out->head->prev = item;
	
	_out->head = item;
	++_out->size;
}


void heList_Push_Back(hePtrList _out, ptrVoid _data)
{
	hePtrListItem item = __heListItem_New__(_data);
	
	item->next = NULL;
	item->prev = _out->tail;
	
	if(_out->head == NULL)
		_out->head = item;
	else
		_out->tail->next = item;
	
	_out->tail = item;
	++_out->size;
}


int heList_Del_Item(hePtrList _out, ptrVoid _data)
{
	hePtrListItem it, del;
	int n = 0;
	
	for(it = _out->head; it != NULL;)
	{
		if(it->data == _data)
		{
			del = it;
			it  = it->next;
			
			if(del != _out->head && del != _out->tail)
			{
				del->prev->next = del->next;
				del->next->prev = del->prev;
			}
			else
			{
				if(del == _out->head)
				{
					_out->head = _out->head->next;
				
					if(_out->head)
						_out->head->prev = NULL;
				}
			
				if(del == _out->tail)
				{
					_out->tail = _out->tail->prev;
				
					if(_out->tail)
						_out->tail->next = NULL;
				}
			}
			
			free(del);
			
			++n;
			--_out->size;
		}
		else
			it = it->next;
	}
	
	return n;
}


hePtrListItem heList_Find_Item(hePtrList _out, ptrVoid _data)
{
	hePtrListItem it;
	
	for(it = _out->head; it != NULL; it = it->next)
	{
		if(it->data == _data)
			return it;
	}
	
	return NULL;
}


hePtrListItem heList_Find_Data(hePtrList _out, ptrVoid _data, int (*_compare)(ptrVoid _a, ptrVoid _b))
{
	hePtrListItem it;
	
	for(it = _out->head; it != NULL; it = it->next)
	{
		if((*_compare)(it->data, _data) == 0)
			return it;
	}
	
	return NULL;
}


int heList_Size(const hePtrList _in)
{
	return _in->size;
}
