#include "OceanApp.h"

OceanApp::OceanApp(HINSTANCE hInstance)
: D3DApp(hInstance), mSky(0), mWalkCamMode(false), mSubMarineTexSRV(0),mSubMarineNormalTexSRV(0)
{
	mMainWndCaption = L"Terrain Demo";
	mEnable4xMsaa = false;

	mSpeed = 50.0f;

	mLastMousePos.x = 0;
	mLastMousePos.y = 0;

	mCam.SetPosition(0.0f, 2.0f, 100.0f);

	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mSubMarineWorld, I);
	XMStoreFloat4x4(&mSubMarineWorld, XMMatrixTranslation(0.0f, -6.0f, 0.0f));

	mSubMarineMat.Ambient  = XMFLOAT4(0.2f, 0.2f, 0.1f, 1.0f);
	mSubMarineMat.Diffuse  = XMFLOAT4(0.4f, 0.4f, 0.2f, 1.0f);
	mSubMarineMat.Specular = XMFLOAT4(0.6f, 0.6f, 0.4f, 1.0f);
	mSubMarineMat.Reflect  = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	mDirLights[0].Ambient  = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mDirLights[0].Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mDirLights[0].Specular = XMFLOAT4(0.8f, 0.8f, 0.7f, 1.0f);
	mDirLights[0].Direction = XMFLOAT3(0.707f, -0.707f, 0.0f);

	mDirLights[1].Ambient  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[1].Diffuse  = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[1].Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[1].Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

	mDirLights[2].Ambient  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[2].Diffuse  = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[2].Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[2].Direction = XMFLOAT3(-0.57735f, -0.57735f, -0.57735f);
}

OceanApp::~OceanApp()
{
	md3dImmediateContext->ClearState();

	ReleaseCOM(mSubMarineTexSRV);
	ReleaseCOM(mSubMarineNormalTexSRV);
	
	SafeDelete(mSky);

	Effects::DestroyAll();
	InputLayouts::DestroyAll();
	RenderStates::DestroyAll();
}

bool OceanApp::Init()
{
	if(!D3DApp::Init())
		return false;

	// Must init Effects first since InputLayouts depend on shader signatures.
	Effects::InitAll(md3dDevice);
	InputLayouts::InitAll(md3dDevice);
	RenderStates::InitAll(md3dDevice);

	mSky = new Sky(md3dDevice, L"Textures/OutputCube.dds", 5000.0f);

	OceanManager::InitInfo oii;
	oii.WavesHighFilename = L"Textures/wavesHigh.dds";
	oii.WavesLowFilename = L"Textures/wavesHigh.dds";
	oii.HeightScale = 50.0f;
	oii.MapWidth = 2049;
	oii.MapHeight = 2049;
	oii.CellSpacing = 0.5f;

	mOcean.Init(md3dDevice, md3dImmediateContext, oii);

	HR(D3DX11CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures/SubmarineFloor.dds", 0, 0, &mSubMarineTexSRV, 0 ));

	HR(D3DX11CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures/SubmarineFloor_NRM.dds", 0, 0, &mSubMarineNormalTexSRV, 0 ));
	return true;
}

void OceanApp::OnResize()
{
	D3DApp::OnResize();

	mCam.SetLens(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 3000.0f);
}

void OceanApp::UpdateScene(float dt)
{
	const XMFLOAT3 mPosW = mCam.GetPosition();
	//
	// Control the camera.
	//
	if(( GetAsyncKeyState('W') & 0x8000 )||( GetAsyncKeyState('Z') & 0x8000 ))
		mCam.Walk(10.0f*dt);

	if( GetAsyncKeyState('S') & 0x8000 )
		mCam.Walk(-10.0f*dt);

	if(( GetAsyncKeyState('A') & 0x8000 )||( GetAsyncKeyState('Q') & 0x8000 ))
		mCam.Strafe(-10.0f*dt);

	if( GetAsyncKeyState('D') & 0x8000 )
		mCam.Strafe(10.0f*dt);

	//
	// Walk/fly mode
	//
	if( GetAsyncKeyState('2') & 0x8000 )
		mWalkCamMode = true;
	if( GetAsyncKeyState('3') & 0x8000 )
		mWalkCamMode = false;

	// 
	// Clamp camera to terrain surface in walk mode.
	//
	if( mWalkCamMode )
	{
		//19.7.1
		//Move at mSpeed along net direction
		//camPosOrigin
		//camPos
		XMFLOAT3 newPos = mCam.GetPosition();
		float y = mOcean.GetHeight(newPos.x, newPos.z);
		mCam.SetPosition(newPos.x, y + 4.0f, newPos.z);
		newPos = mCam.GetPosition();

		//The difference of the new position and old
		//position approximates a tangent vector on the terrain
		XMFLOAT3 tangent = XMFLOAT3(newPos.x - mPosW.x, newPos.y - mPosW.y, newPos.z - mPosW.z);
		//Normalize Tangent
		float dist = sqrt(pow(tangent.x,2)+pow(tangent.y,2)+pow(tangent.z,2));
		if(dist != 0)
		{
			tangent = XMFLOAT3(tangent.x/dist, tangent.y/dist, tangent.z/dist);
			//Move the camera along tangent vector
			newPos = XMFLOAT3(mPosW.x + (tangent.x*dt*mSpeed), mPosW.y + (tangent.y*dt*mSpeed), mPosW.z + (tangent.z*dt*mSpeed));

			//Compute the new Y
			y = mOcean.GetHeight(newPos.x, newPos.z);
			mCam.SetPosition(newPos.x, y + 4.0f, newPos.z);
		}
	}

	mCam.UpdateViewMatrix();
}
void OceanApp::DrawScene()
{
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::Silver));
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

	md3dImmediateContext->IASetInputLayout(InputLayouts::Basic32);
    md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
 
	float blendFactor[] = {0.0f, 0.0f, 0.0f, 0.0f};

	if( GetAsyncKeyState('1') & 0x8000 )
		md3dImmediateContext->RSSetState(RenderStates::WireframeRS);

	mOcean.Draw(md3dImmediateContext, mCam, mDirLights);

	

	Effects::DisplacementMapFX->SetDirLights(mDirLights);
	Effects::DisplacementMapFX->SetEyePosW(mCam.GetPosition());
	Effects::DisplacementMapFX->SetCubeMap(mSky->CubeMapSRV());
	Effects::DisplacementMapFX->SetFogColor(Colors::LightSteelBlue);
	Effects::DisplacementMapFX->SetFogStart(2.0f);
	Effects::DisplacementMapFX->SetFogRange(175.0f);

	// These properties could be set per object if needed.
	Effects::DisplacementMapFX->SetHeightScale(0.07f);
	Effects::DisplacementMapFX->SetMaxTessDistance(1.0f);
	Effects::DisplacementMapFX->SetMinTessDistance(25.0f);
	Effects::DisplacementMapFX->SetMinTessFactor(1.0f);
	Effects::DisplacementMapFX->SetMaxTessFactor(5.0f);
	Effects::DisplacementMapFX->SetFogColor(Colors::LightSteelBlue);
	Effects::DisplacementMapFX->SetFogStart(2.0f);
	Effects::DisplacementMapFX->SetFogRange(175.0f);

	ID3DX11EffectTechnique* activeTech = Effects::DisplacementMapFX->Light3TexTech;
	XMMATRIX world;
	XMMATRIX worldInvTranspose;
	XMMATRIX worldViewProj;

	XMMATRIX view     = mCam.View();
	XMMATRIX proj     = mCam.Proj();
	XMMATRIX viewProj = mCam.ViewProj();

	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc( &techDesc );
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
		//Transparency
		md3dImmediateContext->OMSetBlendState(RenderStates::TransparentBS, blendFactor, 0xffffffff);
		// Draw the SubMarine.
		world = XMLoadFloat4x4(&mSubMarineWorld);
		worldInvTranspose = MathHelper::InverseTranspose(world);
		worldViewProj = world*view*proj;

		Effects::DisplacementMapFX->SetWorld(world);
		Effects::DisplacementMapFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::DisplacementMapFX->SetViewProj(viewProj);
		Effects::DisplacementMapFX->SetWorldViewProj(worldViewProj);
		Effects::DisplacementMapFX->SetTexTransform(XMMatrixScaling(1000.0f, 1000.0f, 1000.0f));
		Effects::DisplacementMapFX->SetMaterial(mSubMarineMat);
		Effects::DisplacementMapFX->SetDiffuseMap(mSubMarineTexSRV);
		Effects::DisplacementMapFX->SetNormalMap(mSubMarineNormalTexSRV);

		activeTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mSubMarineIndexCount, mSubMarineIndexOffset, mSubMarineVertexOffset);

		//End of Transparency
		md3dImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff);
    }

	md3dImmediateContext->RSSetState(0);

	mSky->Draw(md3dImmediateContext, mCam);

	// restore default states, as the SkyFX changes them in the effect file.
	md3dImmediateContext->RSSetState(0);
	md3dImmediateContext->OMSetDepthStencilState(0, 0);

	HR(mSwapChain->Present(0, 0));
}
void OceanApp::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(mhMainWnd);
}
void OceanApp::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}
void OceanApp::OnMouseMove(WPARAM btnState, int x, int y)
{
	if( (btnState & MK_LBUTTON) != 0 )
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));

		mCam.Pitch(dy);
		mCam.RotateY(dx);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}