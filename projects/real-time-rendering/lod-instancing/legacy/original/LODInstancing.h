#pragma once
#include "d3dApp.h"
#include "d3dx11Effect.h"
#include "GeometryGenerator.h"
#include "MathHelper.h"
#include "LightHelper.h"
#include "Effects.h"
#include "Vertex.h"
#include "Camera.h"
#include "xnacollision.h"
#include "RenderStates.h"

struct InstancedData
{
	XMFLOAT4X4 World;
	XMFLOAT4 Color;
};

class LODInstancing : public D3DApp 
{
public:
	LODInstancing(HINSTANCE hInstance);
	~LODInstancing();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene(); 

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

private:
	float VectorLength(XMFLOAT3 _v1, XMFLOAT3 _v2);
	void BuildIcosahedronGeometryBuffers();
	void BuildInstancedBuffer();

private:
	ID3D11Buffer* mIcoVB;
	ID3D11Buffer* mIcoIB;
	ID3D11Buffer* mInstancedBuffer;

	ID3D11ShaderResourceView* mBrickTexSRV;

	// Bounding box of the Icosahedron.
	XNA::AxisAlignedBox mIcoBox;
	XNA::Sphere mIcoSphere;
	XNA::Frustum mCamFrustum;
	
	UINT mVisibleObjectCount;

	// Keep a system memory copy of the world matrices for culling.
	std::vector<InstancedData> mInstancedData;

	bool mFrustumCullingEnabled;
	bool mWireFrameEnabled;

	DirectionalLight mDirLights[3];
	Material mIcoMat;

	// Define transformations from local spaces to world space.
	XMFLOAT4X4 mIcoWorld;

	UINT mIcoIndexCount;

	Camera mCam;

	POINT mLastMousePos;
};
