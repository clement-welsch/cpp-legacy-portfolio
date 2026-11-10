//***************************************************************************************
// OceanManager.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "OceanManager.h"
#include "Camera.h"
#include "LightHelper.h"
#include "Effects.h"
#include "Vertex.h"
#include <fstream>
#include <sstream>

OceanManager::OceanManager() : 
	mQuadPatchVB(0), 
	mQuadPatchIB(0),
	mWavesNormalTexSRVHigh(0), 
	mWavesNormalTexSRVLow(0),
	mWavesDispOffsetHigh(0.0f, 0.0f), 
	mWavesDispOffsetLow(0.0f, 0.0f),
	mWavesNormalOffsetHigh(0.0f, 0.0f), 
	mWavesNormalOffsetLow(0.0f, 0.0f),
	mNumPatchVertices(0),
	mNumPatchQuadFaces(0),
	mNumPatchVertRows(0),
	mNumPatchVertCols(0)
{
	XMStoreFloat4x4(&mWorld, XMMatrixIdentity());

	mMat.Ambient  = XMFLOAT4(0.6f, 0.8f, 0.8f, 1.0f);
	mMat.Diffuse  = XMFLOAT4(0.4f, 0.6f, 0.8f, 0.5f);
	mMat.Specular = XMFLOAT4(0.8f, 0.8f, 1.0f, 32.0f);
	mMat.Reflect  = XMFLOAT4(0.7f, 0.8f, 0.9f, 1.0f);
}

OceanManager::~OceanManager()
{
	ReleaseCOM(mQuadPatchVB);
	ReleaseCOM(mQuadPatchIB);
	ReleaseCOM(mWavesNormalTexSRVHigh);
	ReleaseCOM(mWavesNormalTexSRVLow);
}

float OceanManager::GetWidth()const
{
	// Total OceanManager width.
	return (mInfo.MapWidth-1)*mInfo.CellSpacing;
}

float OceanManager::GetDepth()const
{
	// Total OceanManager depth.
	return (mInfo.MapHeight-1)*mInfo.CellSpacing;
}

float OceanManager::GetHeight(float x, float z)const
{
	// Transform from OceanManager local space to "cell" space.
	float c = (x + 0.5f*GetWidth()) /  mInfo.CellSpacing;
	float d = (z - 0.5f*GetDepth()) / -mInfo.CellSpacing;

	// Get the row and column we are in.
	int row = (int)floorf(d);
	int col = (int)floorf(c);

	// Grab the heights of the cell we are in.
	// A*--*B
	//  | /|
	//  |/ |
	// C*--*D
	float A = mHeightmap[row*mInfo.MapWidth + col];
	float B = mHeightmap[row*mInfo.MapWidth + col + 1];
	float C = mHeightmap[(row+1)*mInfo.MapWidth + col];
	float D = mHeightmap[(row+1)*mInfo.MapWidth + col + 1];

	// Where we are relative to the cell.
	float s = c - (float)col;
	float t = d - (float)row;

	// If upper triangle ABC.
	if( s + t <= 1.0f)
	{
		float uy = B - A;
		float vy = C - A;
		return A + s*uy + t*vy;
	}
	else // lower triangle DCB.
	{
		float uy = C - D;
		float vy = B - D;
		return D + (1.0f-s)*uy + (1.0f-t)*vy;
	}
}

XMMATRIX OceanManager::GetWorld()const
{
	return XMLoadFloat4x4(&mWorld);
}

void OceanManager::SetWorld(CXMMATRIX M)
{
	XMStoreFloat4x4(&mWorld, M);
}

void OceanManager::Init(ID3D11Device* device, ID3D11DeviceContext* dc, const InitInfo& initInfo)
{
	mInfo = initInfo;

	// Divide heightmap into patches such that each patch has CellsPerPatch.
	mNumPatchVertRows = ((mInfo.MapHeight-1) / CellsPerPatch) + 1;
	mNumPatchVertCols = ((mInfo.MapWidth-1) / CellsPerPatch) + 1;

	mNumPatchVertices  = mNumPatchVertRows*mNumPatchVertCols;
	mNumPatchQuadFaces = (mNumPatchVertRows-1)*(mNumPatchVertCols-1);

	CalcAllPatchBoundsY();

	BuildQuadPatchVB(device);
	BuildQuadPatchIB(device);

	HR(D3DX11CreateShaderResourceViewFromFile(device, 
		mInfo.WavesHighFilename, 0, 0, &mWavesNormalTexSRVHigh, 0 ));

	HR(D3DX11CreateShaderResourceViewFromFile(device, 
		mInfo.WavesLowFilename, 0, 0, &mWavesNormalTexSRVLow, 0 ));
}

void OceanManager::Draw(ID3D11DeviceContext* dc, const Camera& cam, DirectionalLight lights[3])
{
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
	dc->IASetInputLayout(InputLayouts::OceanManager);

	UINT stride = sizeof(Vertex::OceanManager);
    UINT offset = 0;
    dc->IASetVertexBuffers(0, 1, &mQuadPatchVB, &stride, &offset);
	dc->IASetIndexBuffer(mQuadPatchIB, DXGI_FORMAT_R16_UINT, 0);

	XMMATRIX viewProj = cam.ViewProj();
	XMMATRIX world  = XMLoadFloat4x4(&mWorld);
	XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
	XMMATRIX worldViewProj = world*viewProj;

	XMFLOAT4 worldPlanes[6];
	ExtractFrustumPlanes(worldPlanes, viewProj);

	// Set per frame constants.
	Effects::OceanFX->SetViewProj(viewProj);
	Effects::OceanFX->SetEyePosW(cam.GetPosition());
	Effects::OceanFX->SetDirLights(lights);
	Effects::OceanFX->SetFogColor(Colors::Silver);
	Effects::OceanFX->SetFogStart(15.0f);
	Effects::OceanFX->SetFogRange(175.0f);
	Effects::OceanFX->SetMinDist(20.0f);
	Effects::OceanFX->SetMaxDist(500.0f);
	Effects::OceanFX->SetMinTess(0.0f);
	Effects::OceanFX->SetMaxTess(6.0f);
	Effects::OceanFX->SetTexelCellSpaceU(1.0f / mInfo.MapWidth);
	Effects::OceanFX->SetTexelCellSpaceV(1.0f / mInfo.MapHeight);
	Effects::OceanFX->SetWorldCellSpace(mInfo.CellSpacing);
	Effects::OceanFX->SetWorldFrustumPlanes(worldPlanes);

	Effects::OceanFX->SetMaterial(mMat);

	ID3DX11EffectTechnique* tech = Effects::OceanFX->Light1Tech;
    D3DX11_TECHNIQUE_DESC techDesc;
    tech->GetDesc( &techDesc );

    for(UINT i = 0; i < techDesc.Passes; ++i)
    {
        ID3DX11EffectPass* pass = tech->GetPassByIndex(i);
		pass->Apply(0, dc);

		dc->DrawIndexed(mNumPatchQuadFaces*4, 0, 0);
	}	

	// FX sets tessellation stages, but it does not disable them.  So do that here
	// to turn off tessellation.
	dc->HSSetShader(0, 0, 0);
	dc->DSSetShader(0, 0, 0);
}

void OceanManager::CalcAllPatchBoundsY()
{
	mPatchBoundsY.resize(mNumPatchQuadFaces);

	// For each patch
	for(UINT i = 0; i < mNumPatchVertRows-1; ++i)
	{
		for(UINT j = 0; j < mNumPatchVertCols-1; ++j)
		{
			CalcPatchBoundsY(i, j);
		}
	}
}

void OceanManager::CalcPatchBoundsY(UINT i, UINT j)
{
	// Scan the heightmap values this patch covers and compute the min/max height.

	UINT x0 = j*CellsPerPatch;
	UINT x1 = (j+1)*CellsPerPatch;

	UINT y0 = i*CellsPerPatch;
	UINT y1 = (i+1)*CellsPerPatch;

	float minY = +MathHelper::Infinity;
	float maxY = -MathHelper::Infinity;
	for(UINT y = y0; y <= y1; ++y)
	{
		for(UINT x = x0; x <= x1; ++x)
		{
			UINT k = y*mInfo.MapWidth + x;
			minY = MathHelper::Min(minY, mHeightmap[k]);
			maxY = MathHelper::Max(maxY, mHeightmap[k]);
		}
	}

	UINT patchID = i*(mNumPatchVertCols-1)+j;
	mPatchBoundsY[patchID] = XMFLOAT2(minY, maxY);
}

void OceanManager::BuildQuadPatchVB(ID3D11Device* device)
{
	std::vector<Vertex::OceanManager> patchVertices(mNumPatchVertRows*mNumPatchVertCols);

	float halfWidth = 0.5f*GetWidth();
	float halfDepth = 0.5f*GetDepth();

	float patchWidth = GetWidth() / (mNumPatchVertCols-1);
	float patchDepth = GetDepth() / (mNumPatchVertRows-1);
	float du = 1.0f / (mNumPatchVertCols-1);
	float dv = 1.0f / (mNumPatchVertRows-1);

	for(UINT i = 0; i < mNumPatchVertRows; ++i)
	{
		float z = halfDepth - i*patchDepth;
		for(UINT j = 0; j < mNumPatchVertCols; ++j)
		{
			float x = -halfWidth + j*patchWidth;

			patchVertices[i*mNumPatchVertCols+j].Pos = XMFLOAT3(x, 0.0f, z);

			// Stretch texture over grid.
			patchVertices[i*mNumPatchVertCols+j].Tex.x = j*du;
			patchVertices[i*mNumPatchVertCols+j].Tex.y = i*dv;
		}
	}

	// Store axis-aligned bounding box y-bounds in upper-left patch corner.
	for(UINT i = 0; i < mNumPatchVertRows-1; ++i)
	{
		for(UINT j = 0; j < mNumPatchVertCols-1; ++j)
		{
			UINT patchID = i*(mNumPatchVertCols-1)+j;
			patchVertices[i*mNumPatchVertCols+j].BoundsY = mPatchBoundsY[patchID];
		}
	}

    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::OceanManager) * patchVertices.size();
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &patchVertices[0];
    HR(device->CreateBuffer(&vbd, &vinitData, &mQuadPatchVB));
}

void OceanManager::BuildQuadPatchIB(ID3D11Device* device)
{
	std::vector<USHORT> indices(mNumPatchQuadFaces*4); // 4 indices per quad face

	// Iterate over each quad and compute indices.
	int k = 0;
	for(UINT i = 0; i < mNumPatchVertRows-1; ++i)
	{
		for(UINT j = 0; j < mNumPatchVertCols-1; ++j)
		{
			// Top row of 2x2 quad patch
			indices[k]   = i*mNumPatchVertCols+j;
			indices[k+1] = i*mNumPatchVertCols+j+1;

			// Bottom row of 2x2 quad patch
			indices[k+2] = (i+1)*mNumPatchVertCols+j;
			indices[k+3] = (i+1)*mNumPatchVertCols+j+1;

			k += 4; // next quad
		}
	}

	D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(USHORT) * indices.size();
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = &indices[0];
    HR(device->CreateBuffer(&ibd, &iinitData, &mQuadPatchIB));
}