#pragma once
#include "d3dApp.h"
#include "d3dx11Effect.h"
#include "GeometryGenerator.h"
#include "MathHelper.h"
#include "LightHelper.h"
#include "Effects.h"
#include "Vertex.h"
#include "RenderStates.h"
#include "Camera.h"
#include "Sky.h"
#include "OceanManager.h"
 
class OceanApp : public D3DApp
{
public:
	OceanApp(HINSTANCE hInstance);
	~OceanApp();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene(); 

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

private:
	Sky* mSky;
	OceanManager mOcean;

	DirectionalLight mDirLights[3];

	Camera mCam;

	bool mWalkCamMode;

	POINT mLastMousePos;

	float mSpeed;

	ID3D11Buffer* mSubVB;
	ID3D11Buffer* mSubIB;

	ID3D11ShaderResourceView* mSubMarineTexSRV;
	ID3D11ShaderResourceView* mSubMarineNormalTexSRV;

	Material mSubMarineMat;

	XMFLOAT4X4 mSubMarineWorld;

	int mSubMarineVertexOffset;
	UINT mSubMarineIndexOffset;
	UINT mSubMarineIndexCount;
};
