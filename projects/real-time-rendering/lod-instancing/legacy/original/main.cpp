//***************************************************************************************
// That Demo present a mix of different techniques
// + Tessellation
// + Level of Detail
// + Instancing
// + Frustum Culling
// That Demo is based on the samples made by Frank Luna (C) 2011 All Rights Reserved.
//
// Demonstrates hardware instancing, frustum culling and tessellation of icosahedrons.  Only objects intersecting
// the frustum are instanced and drawn.
//
// Controls:
//		Hold the left mouse button down and move the mouse to rotate.
//      Hold the right mouse button down to zoom in and out.
//      '1' Turn on frustum culling.
//      '2' Turn off frustum culling.
//      '3' Turn on WireFrame View
//      '4' Turn off WireFrame View
//
//***************************************************************************************
#include "LODInstancing.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	LODInstancing theApp(hInstance);
	
	if( !theApp.Init() )
		return 0;
	
	return theApp.Run();
}