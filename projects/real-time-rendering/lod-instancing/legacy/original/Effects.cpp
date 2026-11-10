//***************************************************************************************
// Effects.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "Effects.h"

#pragma region Effect
Effect::Effect(ID3D11Device* device, const std::wstring& filename)
	: mFX(0)
{
	std::ifstream fin(filename, std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();
	
	HR(D3DX11CreateEffectFromMemory(&compiledShader[0], size, 
		0, device, &mFX));
}

Effect::~Effect()
{
	ReleaseCOM(mFX);
}
#pragma endregion

#pragma region InstancedBasicEffect
InstancedBasicEffect::InstancedBasicEffect(ID3D11Device* device, const std::wstring& filename)
	: Effect(device, filename)
{
	LODInstancingTech    = mFX->GetTechniqueByName("LODInstancing");

	WorldViewProj     = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	ViewProj          = mFX->GetVariableByName("gViewProj")->AsMatrix();
	World             = mFX->GetVariableByName("gWorld")->AsMatrix();
	WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	TexTransform      = mFX->GetVariableByName("gTexTransform")->AsMatrix();
	EyePosW           = mFX->GetVariableByName("gEyePosW")->AsVector();
	FogColor          = mFX->GetVariableByName("gFogColor")->AsVector();
	FogStart          = mFX->GetVariableByName("gFogStart")->AsScalar();
	FogRange          = mFX->GetVariableByName("gFogRange")->AsScalar();
	DirLights         = mFX->GetVariableByName("gDirLights");
	Mat               = mFX->GetVariableByName("gMaterial");
	DiffuseMap        = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
}

InstancedBasicEffect::~InstancedBasicEffect()
{
}
#pragma endregion

#pragma region Effects

InstancedBasicEffect* Effects::InstancedBasicFX = 0;

void Effects::InitAll(ID3D11Device* device)
{
	//InstancedBasicFX = new InstancedBasicEffect(device, L"FX/LODInstancing.fxo");
	InstancedBasicFX = new InstancedBasicEffect(device, L"FX/TessellationInstancing.fxo");
}

void Effects::DestroyAll()
{
	SafeDelete(InstancedBasicFX);
}
#pragma endregion