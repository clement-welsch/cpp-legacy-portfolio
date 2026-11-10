#ifndef OCEANMANAGER_H
#define OCEANMANAGER_H

#include "d3dUtil.h"

class Camera;
struct DirectionalLight;

class OceanManager
{
public:
	struct InitInfo
	{
		std::wstring WavesHighFilename;
		std::wstring WavesLowFilename;
		float HeightScale;
		UINT MapWidth;
		UINT MapHeight;
		float CellSpacing;
	};

public:
	OceanManager();
	~OceanManager();

	float GetWidth()const;
	float GetDepth()const;
	float GetHeight(float x, float z)const;

	XMMATRIX GetWorld()const;
	void SetWorld(CXMMATRIX M);

	void Init(ID3D11Device* device, ID3D11DeviceContext* dc, const InitInfo& initInfo);

	void Draw(ID3D11DeviceContext* dc, const Camera& cam, DirectionalLight lights[3]);

private:
	void CalcAllPatchBoundsY();
	void CalcPatchBoundsY(UINT i, UINT j);
	void BuildQuadPatchVB(ID3D11Device* device);
	void BuildQuadPatchIB(ID3D11Device* device);

private:

	// Divide heightmap into patches such that each patch has CellsPerPatch cells
	// and CellsPerPatch+1 vertices.  Use 64 so that if we tessellate all the way 
	// to 64, we use all the data from the heightmap.  
	static const int CellsPerPatch = 64;

	ID3D11Buffer* mQuadPatchVB;
	ID3D11Buffer* mQuadPatchIB;

	ID3D11ShaderResourceView* mWavesNormalTexSRVHigh;
	ID3D11ShaderResourceView* mWavesNormalTexSRVLow;

	InitInfo mInfo;

	UINT mNumPatchVertices;
	UINT mNumPatchQuadFaces;

	UINT mNumPatchVertRows;
	UINT mNumPatchVertCols;

	XMFLOAT4X4 mWorld;

	XMFLOAT2 mWavesDispOffsetHigh;
	XMFLOAT2 mWavesDispOffsetLow;

	XMFLOAT2 mWavesNormalOffsetHigh;
	XMFLOAT2 mWavesNormalOffsetLow;

	XMFLOAT4X4 mWavesDispTexTransformHigh;
	XMFLOAT4X4 mWavesDispTexTransformLow;
	XMFLOAT4X4 mWavesNormalTexTransformHigh;
	XMFLOAT4X4 mWavesNormalTexTransformLow;

	Material mMat;

	std::vector<XMFLOAT2> mPatchBoundsY;
	std::vector<float> mHeightmap;
};

#endif // OceanManager_H