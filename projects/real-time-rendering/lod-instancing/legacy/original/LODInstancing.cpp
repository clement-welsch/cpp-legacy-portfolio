#include "LODInstancing.h"

LODInstancing::LODInstancing(HINSTANCE hInstance): D3DApp(hInstance), mIcoVB(0), mIcoIB(0), mBrickTexSRV(0),mIcoIndexCount(0), mInstancedBuffer(0), mVisibleObjectCount(0), mFrustumCullingEnabled(true)
{
	mMainWndCaption = L"Tesselation, Instancing and Culling Demo";
	
	srand((unsigned int)time((time_t *)NULL));

	mWireFrameEnabled = false;

	mLastMousePos.x = 0;
	mLastMousePos.y = 0;

	mCam.SetPosition(0.0f, 2.0f, -15.0f);

	XMMATRIX I = XMMatrixIdentity();

	XMMATRIX icoScale = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	XMMATRIX icoOffset = XMMatrixTranslation(0.0f, 1.0f, 0.0f);
	XMStoreFloat4x4(&mIcoWorld, XMMatrixMultiply(icoScale, icoOffset));


	mDirLights[0].Ambient  = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[0].Diffuse  = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLights[0].Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLights[0].Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

	mDirLights[1].Ambient  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[1].Diffuse  = XMFLOAT4(0.20f, 0.20f, 0.20f, 1.0f);
	mDirLights[1].Specular = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	mDirLights[1].Direction = XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);

	mDirLights[2].Ambient  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[2].Diffuse  = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[2].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[2].Direction = XMFLOAT3(0.0f, -0.707f, -0.707f);

	mIcoMat.Ambient  = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	mIcoMat.Diffuse  = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	mIcoMat.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);
}
LODInstancing::~LODInstancing()
{
	ReleaseCOM(mIcoVB);
	ReleaseCOM(mIcoIB);
	ReleaseCOM(mBrickTexSRV);
	ReleaseCOM(mInstancedBuffer);

	Effects::DestroyAll();
	InputLayouts::DestroyAll(); 
	RenderStates::DestroyAll();
}
bool LODInstancing::Init()
{
	if(!D3DApp::Init())
		return false;

	// Must init Effects first since InputLayouts depend on shader signatures.
	Effects::InitAll(md3dDevice);
	InputLayouts::InitAll(md3dDevice);
	RenderStates::InitAll(md3dDevice);

	HR(D3DX11CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures/bricks.dds", 0, 0, &mBrickTexSRV, 0 ));

	BuildIcosahedronGeometryBuffers();
	BuildInstancedBuffer();

	return true;
}
void LODInstancing::OnResize()
{
	D3DApp::OnResize();

	mCam.SetLens(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);

	// Build the frustum from the projection matrix in view space.
	ComputeFrustumFromProjection(&mCamFrustum, &mCam.Proj());
}
void LODInstancing::UpdateScene(float dt)
{
	//
	// Control the camera.
	//
	if( GetAsyncKeyState('Z') & 0x8000 )
		mCam.Walk(10.0f*dt);

	if( GetAsyncKeyState('S') & 0x8000 )
		mCam.Walk(-10.0f*dt);

	if( GetAsyncKeyState('Q') & 0x8000 )
		mCam.Strafe(-10.0f*dt);

	if( GetAsyncKeyState('D') & 0x8000 )
		mCam.Strafe(10.0f*dt);
		
	if( GetAsyncKeyState('1') & 0x8000 )
		mFrustumCullingEnabled = true;

	if( GetAsyncKeyState('2') & 0x8000 )
		mFrustumCullingEnabled = false;

	if( GetAsyncKeyState('3') & 0x8000 )
	{
		mWireFrameEnabled = true;
	}
	if( GetAsyncKeyState('4') & 0x8000 )
	{
		mWireFrameEnabled = false;
	}

	//
	// Perform frustum culling.
	//

	mCam.UpdateViewMatrix();
	mVisibleObjectCount = 0;

	if(mFrustumCullingEnabled)
	{
		XMVECTOR detView = XMMatrixDeterminant(mCam.View());
		XMMATRIX invView = XMMatrixInverse(&detView, mCam.View());
	
		D3D11_MAPPED_SUBRESOURCE mappedData; 
		md3dImmediateContext->Map(mInstancedBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);

		InstancedData* dataView = reinterpret_cast<InstancedData*>(mappedData.pData);

		for(UINT i = 0; i < mInstancedData.size(); ++i)
		{
			XMMATRIX W = XMLoadFloat4x4(&mInstancedData[i].World);
			XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(W), W);

			// View space to the object's local space.
			XMMATRIX toLocal = XMMatrixMultiply(invView, invWorld);
		
			// Decompose the matrix into its individual parts.
			XMVECTOR scale;
			XMVECTOR rotQuat;
			XMVECTOR translation;
			XMMatrixDecompose(&scale, &rotQuat, &translation, toLocal);

			// Transform the camera frustum from view space to the object's local space.
			XNA::Frustum localspaceFrustum;
			XNA::TransformFrustum(&localspaceFrustum, &mCamFrustum, XMVectorGetX(scale), rotQuat, translation);

			// Perform the sphere/frustum intersection test in local space.
			if(XNA::IntersectSphereFrustum(&mIcoSphere, &localspaceFrustum) != 0)
			{
				// Write the instance data to dynamic VB of the visible objects.
				dataView[mVisibleObjectCount++] = mInstancedData[i];
			}
		}

		md3dImmediateContext->Unmap(mInstancedBuffer, 0);
	}
	else // No culling enabled, draw all objects.
	{
		D3D11_MAPPED_SUBRESOURCE mappedData; 
		md3dImmediateContext->Map(mInstancedBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);

		InstancedData* dataView = reinterpret_cast<InstancedData*>(mappedData.pData);

		for(UINT i = 0; i < mInstancedData.size(); ++i)
		{
			dataView[mVisibleObjectCount++] = mInstancedData[i];
		}

		md3dImmediateContext->Unmap(mInstancedBuffer, 0);
	}

	std::wostringstream outs;   
	outs.precision(6);
	outs << L"Instancing and Culling Demo" << 
		L"    " << mVisibleObjectCount << 
		L" objects visible out of " << mInstancedData.size();
	mMainWndCaption = outs.str();
}
void LODInstancing::DrawScene()
{
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::Silver));
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

	md3dImmediateContext->IASetInputLayout(InputLayouts::InstancedBasic32);
    //md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
 
	UINT stride[2] = {sizeof(Vertex::Basic32), sizeof(InstancedData)};
    UINT offset[2] = {0,0};

	ID3D11Buffer* vbs[2] = {mIcoVB, mInstancedBuffer};
 
	XMMATRIX view     = mCam.View();
	XMMATRIX proj     = mCam.Proj();
	XMMATRIX viewProj = mCam.ViewProj();

	// Set per frame constants.
	Effects::InstancedBasicFX->SetDirLights(mDirLights);
	Effects::InstancedBasicFX->SetEyePosW(mCam.GetPosition());
 
	ID3DX11EffectTechnique* activeTech = Effects::InstancedBasicFX->LODInstancingTech;

	if (mWireFrameEnabled == true)
		md3dImmediateContext->RSSetState(RenderStates::WireframeRS);
	else
		md3dImmediateContext->RSSetState(RenderStates::NoCullRS);
	 
    D3DX11_TECHNIQUE_DESC techDesc;
    activeTech->GetDesc( &techDesc );
	for(UINT p = 0; p < techDesc.Passes; ++p)
    {
		// Draw the Ico.

		md3dImmediateContext->IASetVertexBuffers(0, 2, vbs, stride, offset);
		md3dImmediateContext->IASetIndexBuffer(mIcoIB, DXGI_FORMAT_R32_UINT, 0);

		XMMATRIX world = XMLoadFloat4x4(&mIcoWorld);
		XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
		XMMATRIX worldViewProj = world*view*proj;

		Effects::InstancedBasicFX->SetWorld(world);
		Effects::InstancedBasicFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::InstancedBasicFX->SetWorldViewProj(worldViewProj);
		Effects::InstancedBasicFX->SetViewProj(viewProj);
		Effects::InstancedBasicFX->SetMaterial(mIcoMat);
		Effects::InstancedBasicFX->SetTexTransform(XMMatrixScaling(2.0f, 2.0f, 2.0f));
		Effects::InstancedBasicFX->SetDiffuseMap(mBrickTexSRV);

		activeTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexedInstanced(mIcoIndexCount, mVisibleObjectCount, 0, 0, 0);
	}
	
	HR(mSwapChain->Present(0, 0));
}
void LODInstancing::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(mhMainWnd);
}
void LODInstancing::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}
void LODInstancing::OnMouseMove(WPARAM btnState, int x, int y)
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
float LODInstancing::VectorLength(XMFLOAT3 _v1, XMFLOAT3 _v2)
{
	return sqrt(pow(_v2.x-_v1.x,2) + pow(_v2.y-_v1.y,2) + pow(_v2.z-_v1.z,2));
}
void LODInstancing::BuildIcosahedronGeometryBuffers()
{
	//Create vertex buffer
	//Position Buffer
	GeometryGenerator::MeshData ico;
	GeometryGenerator icoGeometry;
	icoGeometry.CreateGeosphere (1, 0, ico);
	//
	// Extract the vertex elements we are interested in and pack the
	// vertices of all the meshes into one vertex buffer.
	//

	XMFLOAT3 vMinf3(+MathHelper::Infinity, +MathHelper::Infinity, +MathHelper::Infinity);
	XMFLOAT3 vMaxf3(-MathHelper::Infinity, -MathHelper::Infinity, -MathHelper::Infinity);
	
	XMVECTOR vMin = XMLoadFloat3(&vMinf3);
	XMVECTOR vMax = XMLoadFloat3(&vMaxf3);
	std::vector<Vertex::Basic32> vertices(ico.Vertices.size());
	for(UINT i = 0; i < ico.Vertices.size(); ++i)
	{
		vertices[i].Pos    = ico.Vertices[i].Position;
		vertices[i].Normal = ico.Vertices[i].Normal;
		vertices[i].Tex    = ico.Vertices[i].TexC;

		XMVECTOR P = XMLoadFloat3(&vertices[i].Pos);
		vMin = XMVectorMin(vMin, P);
		vMax = XMVectorMax(vMax, P);
	}

	XMStoreFloat3(&mIcoBox.Center, 0.5f*(vMin+vMax));
	XMStoreFloat3(&mIcoBox.Extents, 0.5f*(vMax-vMin));

	XMStoreFloat3(&mIcoSphere.Center, 0.5f*(vMin+vMax));
	mIcoSphere.Radius = -1;

	for(UINT i = 0; i < ico.Vertices.size(); ++i)
	{
		float length = VectorLength(mIcoSphere.Center, vertices[i].Pos);
		if(length > mIcoSphere.Radius)
			mIcoSphere.Radius = length;
	}

    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex::Basic32) * ico.Vertices.size();
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &vertices[0];
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mIcoVB));

	//
	// Pack the indices of all the meshes into one index buffer.
	//

	D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * ico.Indices.size();
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = &ico.Indices[0];
    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIcoIB));

	mIcoIndexCount = ico.Indices.size();
}
void LODInstancing::BuildInstancedBuffer()
{
	const int n = 15;
	mInstancedData.resize(n*n*n);
	
	float width = 200.0f;
	float height = 200.0f;
	float depth = 200.0f;
	
	float x = -0.5f*width;
	float y = -0.5f*height;
	float z = -0.5f*depth;
	float dx = width / (n-1);
	float dy = height / (n-1);
	float dz = depth / (n-1);
	for(int k = 0; k < n; ++k)
	{
		for(int i = 0; i < n; ++i)
		{
			for(int j = 0; j < n; ++j)
			{
				// Position instanced along a 3D grid.
				mInstancedData[k*n*n + i*n + j].World = XMFLOAT4X4(
					1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					x+j*dx, y+i*dy, z+k*dz, 1.0f);
				
				// Random color.
				mInstancedData[k*n*n + i*n + j].Color.x = MathHelper::RandF(0.0f, 1.0f);
				mInstancedData[k*n*n + i*n + j].Color.y = MathHelper::RandF(0.0f, 1.0f);
				mInstancedData[k*n*n + i*n + j].Color.z = MathHelper::RandF(0.0f, 1.0f);
				mInstancedData[k*n*n + i*n + j].Color.w = 1.0f;
			}
		}
	}
	
	D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(InstancedData) * mInstancedData.size();
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	
    HR(md3dDevice->CreateBuffer(&vbd, 0, &mInstancedBuffer));
}