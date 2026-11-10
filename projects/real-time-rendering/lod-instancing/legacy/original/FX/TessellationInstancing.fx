//=============================================================================
// Basic.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Basic effect that currently supports transformations, lighting, and texturing.
//=============================================================================

#include "LightHelper.fx"

cbuffer cbPerFrame
{
	DirectionalLight gDirLights[3];
	float3 gEyePosW;

	float  gFogStart;
	float  gFogRange;
	float4 gFogColor;
};

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;
	float4x4 gWorldViewProj;
	float4x4 gViewProj;
	float4x4 gTexTransform;
	Material gMaterial;
}; 

// Nonnumeric values cannot be added to a cbuffer.
Texture2D gDiffuseMap;

SamplerState samAnisotropic
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;

	AddressU = WRAP;
	AddressV = WRAP;
};

struct VertexIn
{
	float3 PosL     : POSITION;
	float3 NormalL  : NORMAL;
	float2 Tex      : TEXCOORD;
	row_major float4x4 World  : WORLD;
	float4 Color    : COLOR;
	uint InstanceId : SV_InstanceID;
};

struct VertexOut
{
    float4 PosH    : SV_POSITION;
    float3 PosW    : POSITION;
    float3 NormalW : NORMAL;
	float2 Tex     : TEXCOORD;
	float4 Color   : COLOR;
	int distTess : INTEGER;
	row_major float4x4 World  : WORLD;
};
//-------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------Vertex Shader------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to world space space.
	//vout.PosW    = mul(float4(vin.PosL, 1.0f), vin.World).xyz;
	vout.PosW    = vin.PosL;
	vout.NormalW = mul(vin.NormalL, (float3x3)vin.World);
	vout.World = vin.World;
		
	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vout.PosW, 1.0f), gViewProj);
	
	// Output vertex attributes for interpolation across triangle.
	vout.Tex   = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;
	float3 worldPoint = mul(float4(vout.PosW,1.0f), vout.World).xyz;
	float d = distance(worldPoint, gEyePosW);
	vout.Color = float4(1.0f, 1.0f, 0.0f, 1.0f);
	vout.distTess = d;
	if (d <20)
	{
		vout.Color = float4(1.0f, 1.0f, 0.0f, 1.0f);
		vout.distTess = 5;
	}
	else if (d < 40)
	{
		vout.Color = float4(0.0f, 1.0f, 1.0f, 1.0f);
		vout.distTess = 4;
	}
	else if(d < 50)
	{
		vout.Color = float4(1.0f, 0.0f, 1.0f, 1.0f);
		vout.distTess = 3;
	}
	else
	{
		vout.Color = float4(1.0f, 0.0f, 0.0f, 1.0f);
		vout.distTess = 1;
	}
	

	return vout;
}
//-------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------Tessalation Shader-------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
struct PatchTess
{
	float EdgeTess[3]   : SV_TessFactor;
	float InsideTess[1] : SV_InsideTessFactor;
};

PatchTess ConstantHS(InputPatch<VertexOut, 3> patch, uint patchID : SV_PrimitiveID)
{
	PatchTess pt;
	
	float3 centerL = 0.25f*(patch[0].PosW + patch[1].PosW + patch[2].PosW);
	float3 centerW = mul(float4(centerL, 1.0f), patch[0].World).xyz;
	
	float d = distance(patch[0].PosW, gEyePosW);

	// Tessellate the patch based on distance from the eye such that
	// the tessellation is 0 if d >= d1 and 60 if d <= d0.  The interval
	// [d0, d1] defines the range we tessellate in.
	
	const float d0 = 20.0f;
	const float d1 = 100.0f;
	float tess = 64.0f*saturate( (d1-d)/(d1-d0) );


	//---------------------------
	//We are overwritting the tess compute to make a threshold of the Tessellation
	//So we have a better difference between two levels of details
	//---------------------------
	tess = patch[0].distTess;

	// Uniformly tessellate the patch.

	pt.EdgeTess[0] = tess;
	pt.EdgeTess[1] = tess;
	pt.EdgeTess[2] = tess;
	
	pt.InsideTess[0] = tess;
	
	return pt;
}

struct HullOut
{
	float4 PosH    : SV_POSITION;
    float3 PosW    : POSITION;
	float3 NormalW : NORMAL;
	float2 Tex     : TEXCOORD;
	float4 Color   : COLOR;
	row_major float4x4 World  : WORLD;
};

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(64.0f)]
HullOut HS(InputPatch<VertexOut, 3> patch, uint i : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
	HullOut hout;
	
	hout.PosH = patch[i].PosH;
	hout.PosW = patch[i].PosW;
	hout.NormalW = patch[i].NormalW;
	hout.Tex = patch[i].Tex;
	hout.Color = patch[i].Color;
	
	hout.World = patch[i].World;
	
	return hout;
}

struct DomainOut
{
	float4 PosH    : SV_POSITION;
    float3 PosW    : POSITION;
    float3 NormalW : NORMAL;
	float2 Tex     : TEXCOORD;
	float4 Color   : COLOR;
};

// The domain shader is called for every vertex created by the tessellator.  
// It is like the vertex shader after tessellation.
[domain("tri")]
DomainOut DS(PatchTess patchTess, float3 bary : SV_DomainLocation, const OutputPatch<HullOut, 3> tri )
{
	DomainOut dout;
	// Transform to world space space.
	dout.PosW     = bary.x*tri[0].PosW     + bary.y*tri[1].PosW     + bary.z*tri[2].PosW;
	dout.NormalW  = bary.x*tri[0].NormalW  + bary.y*tri[1].NormalW  + bary.z*tri[2].NormalW;
	// Interpolating normal can unnormalize it, so normalize it.
	dout.PosW = normalize(dout.PosW);
	dout.NormalW = normalize(dout.NormalW);

	dout.PosW     = mul(float4(dout.PosW, 1.0f), tri[0].World).xyz;
	//dout.NormalW  = mul(tri[0].NormalW, (float3x3)tri[0].World);
	// Project to homogeneous clip space.
	dout.PosH = mul(float4(dout.PosW, 1.0f), gViewProj);
	dout.Tex      = bary.x*tri[0].Tex      + bary.y*tri[1].Tex      + bary.z*tri[2].Tex;
	dout.Color    = bary.x*tri[0].Color     + bary.y*tri[1].Color     + bary.z*tri[2].Color;

	
	

	return dout;
} 
//-------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------Pixel Shader-------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
float4 PS(DomainOut pin, uniform int gLightCount, uniform bool gUseTexure, uniform bool gAlphaClip, uniform bool gFogEnabled) : SV_Target
{
	// Interpolating normal can unnormalize it, so normalize it.
    pin.NormalW = normalize(pin.NormalW);

	// The toEye vector is used in lighting.
	float3 toEye = gEyePosW - pin.PosW;

	// Cache the distance to the eye from this surface point.
	float distToEye = length(toEye);

	// Normalize.
	toEye /= distToEye;
	
    // Default to multiplicative identity.
    float4 texColor = float4(1, 1, 1, 1);
    if(gUseTexure)
	{
		// Sample texture.
		texColor = gDiffuseMap.Sample( samAnisotropic, pin.Tex );

		if(gAlphaClip)
		{
			// Discard pixel if texture alpha < 0.1.  Note that we do this
			// test as soon as possible so that we can potentially exit the shader 
			// early, thereby skipping the rest of the shader code.
			clip(texColor.a - 0.1f);
		}
	}
	 
	//
	// Lighting.
	//

	float4 litColor = texColor;
	if( gLightCount > 0  )
	{  
		// Start with a sum of zero. 
		float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

		// Sum the light contribution from each light source.  
		[unroll]
		for(int i = 0; i < gLightCount; ++i)
		{
			float4 A, D, S;
			ComputeDirectionalLight(gMaterial, gDirLights[i], pin.NormalW, toEye, 
				A, D, S);

			ambient += A*pin.Color;
			diffuse += D*pin.Color;
			spec    += S;
		}

		// Modulate with late add.
		litColor = texColor * ((ambient/2) + (diffuse * 2)) + spec;
	}

	//
	// Fogging
	//

	if( gFogEnabled )
	{
		float fogLerp = saturate( (distToEye - gFogStart) / gFogRange ); 

		// Blend the fog color and the lit color.
		litColor = lerp(litColor, gFogColor, fogLerp);
	}

	// Common to take alpha from diffuse material and texture.
	litColor.a = gMaterial.Diffuse.a * texColor.a;

    return litColor * pin.Color;
}

technique11 LODInstancing
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetHullShader( CompileShader( hs_5_0, HS() ) );
        SetDomainShader( CompileShader( ds_5_0, DS() ) );
        SetPixelShader( CompileShader( ps_5_0, PS(3,true,false,false) ) );
    }
}