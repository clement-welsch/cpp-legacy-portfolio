//=============================================================================
// NormalMap.fx by Frank Luna (C) 2011 All Rights Reserved.
//=============================================================================

#include "LightHelper.fx"
#include "SubMarineViewHelper.fx"
 
cbuffer cbPerFrame
{
	DirectionalLight gDirLights[3];
	float3 gEyePosW;

	float  gFogStart;
	float  gFogRange;
	float4 gFogColor; 
	
	float gHeightScaleHigh;
	float gHeightScaleLow;
	float gMaxTessDistance;
	float gMinTessDistance;
	float gMinTessFactor;
	float gMaxTessFactor;

	float gOceanMode;
};

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;
	float4x4 gViewProj;
	float4x4 gWorldViewProj;
	float4x4 gTexTransform;

	float4x4 gWaveDispTexTransformHigh;
	float4x4 gWaveDispTexTransformLow;
	float4x4 gWaveNormalTexTransformHigh;
	float4x4 gWaveNormalTexTransformLow;

	Material gMaterial;
}; 

// Nonnumeric values cannot be added to a cbuffer.
Texture2D gDiffuseMap;
Texture2D gNormalMapHigh;
Texture2D gNormalMapLow;
TextureCube gCubeMap;

SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};
 
struct VertexIn
{
	float3 PosL     : POSITION;
	float3 NormalL  : NORMAL;
	float2 Tex      : TEXCOORD;
	float3 TangentL : TANGENT;
};

struct VertexOut
{
    float3 PosW					: POSITION;
    float3 NormalW				: NORMAL;
	float3 TangentW				: TANGENT;
	float2 Tex					: TEXCOORD0;
	float2 WaveDispTexHigh		: TEXCOORD1;
	float2 WaveDispTexLow		: TEXCOORD2;
	float2 WaveNormalTexHigh	: TEXCOORD3;
	float2 WaveNormalTexLow		: TEXCOORD4;
	float  TessFactor			: TESS;
};
//-------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------Vertex Shader------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to world space space.
	vout.PosW     = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.NormalW  = mul(vin.NormalL, (float3x3)gWorldInvTranspose);
	vout.TangentW = mul(vin.TangentL, (float3x3)gWorld);

	// Output vertex attributes for interpolation across triangle.
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;
	//Waves
	vout.WaveDispTexHigh   = mul(float4(vin.Tex, 0.0f, 1.0f), gWaveDispTexTransformHigh).xy;
	vout.WaveDispTexLow    = mul(float4(vin.Tex, 0.0f, 1.0f), gWaveDispTexTransformLow).xy;
	vout.WaveNormalTexHigh = mul(float4(vin.Tex, 0.0f, 1.0f), gWaveNormalTexTransformHigh).xy;
	vout.WaveNormalTexLow  = mul(float4(vin.Tex, 0.0f, 1.0f), gWaveNormalTexTransformLow).xy;
	
	float d = distance(vout.PosW, gEyePosW);

	// Normalized tessellation factor. 
	// The tessellation is 
	//   0 if d >= gMinTessDistance and
	//   1 if d <= gMaxTessDistance.  
	float tess = saturate( (gMinTessDistance - d) / (gMinTessDistance - gMaxTessDistance) );
	
	// Rescale [0,1] --> [gMinTessFactor, gMaxTessFactor].
	vout.TessFactor = gMinTessFactor + tess*(gMaxTessFactor-gMinTessFactor);

	/*if ((gOceanMode >= 1.0f) && (distance (float3(0.0f,0.0f,0.0f), vout.PosW) < 300))
	{
		vout.TessFactor = 1.0f;
		vout.WaveDispTexHigh = float2 (0.0f,0.0f);
		vout.WaveDispTexLow	= float2 (0.0f,0.0f);
		vout.WaveNormalTexHigh = float2 (0.0f,0.0f);
		vout.WaveNormalTexLow = float2 (0.0f,0.0f);
	}*/

	return vout;
}
//-------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------Tessalation Shader-------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
struct PatchTess
{
	float EdgeTess[3] : SV_TessFactor;
	float InsideTess  : SV_InsideTessFactor;
};

PatchTess PatchHS(InputPatch<VertexOut,3> patch, uint patchID : SV_PrimitiveID)
{
	PatchTess pt;

	float3 centerL = 0.25f*(patch[0].PosW + patch[1].PosW + patch[2].PosW);
	float3 centerW = mul(float4(centerL, 1.0f), gWorld).xyz;
	float d = distance(centerW, gEyePosW);

	// Tessellate the patch based on distance from the eye such that
	// the tessellation is 0 if d >= d1 and 60 if d <= d0.  The interval
	// [d0, d1] defines the range we tessellate in.
	
	const float d0 = 20.0f;
	const float d1 = 100.0f;
	float tess = 64.0f*saturate( (d1-d)/(d1-d0) );
	
	// Average tess factors along edges, and pick an edge tess factor for 
	// the interior tessellation.  It is important to do the tess factor
	// calculation based on the edge properties so that edges shared by 
	// more than one triangle will have the same tessellation factor.  
	// Otherwise, gaps can appear.
	pt.EdgeTess[0] = (0.5f*(patch[1].TessFactor + patch[2].TessFactor));
	pt.EdgeTess[1] = (0.5f*(patch[2].TessFactor + patch[0].TessFactor));
	pt.EdgeTess[2] = (0.5f*(patch[0].TessFactor + patch[1].TessFactor));
	pt.InsideTess  = pt.EdgeTess[0];
	
	return pt;
}
struct HullOut
{
	float3 PosW     : POSITION;
    float3 NormalW  : NORMAL;
	float3 TangentW : TANGENT;
	float2 Tex					: TEXCOORD0;
	float2 WaveDispTexHigh		: TEXCOORD1;
	float2 WaveDispTexLow		: TEXCOORD2;
	float2 WaveNormalTexHigh	: TEXCOORD3;
	float2 WaveNormalTexLow		: TEXCOORD4;
};

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("PatchHS")]
HullOut HS(InputPatch<VertexOut,3> p, uint i : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
	HullOut hout;
	
	// Pass through shader.
	hout.PosW     = p[i].PosW;
	hout.NormalW  = p[i].NormalW;
	hout.TangentW = p[i].TangentW;
	hout.Tex      = p[i].Tex;

	hout.WaveDispTexHigh   = p[i].WaveDispTexHigh;
	hout.WaveDispTexLow    = p[i].WaveDispTexLow;
	hout.WaveNormalTexHigh = p[i].WaveNormalTexHigh;
	hout.WaveNormalTexLow  = p[i].WaveNormalTexLow;
	
	return hout;
}

struct DomainOut
{
	float4 PosH     : SV_POSITION;
    float3 PosW     : POSITION;
    float3 NormalW  : NORMAL;
	float3 TangentW : TANGENT;
	float2 Tex					: TEXCOORD0;
	float2 WaveDispTexHigh		: TEXCOORD1;
	float2 WaveDispTexLow		: TEXCOORD2;
	float2 WaveNormalTexHigh	: TEXCOORD3;
	float2 WaveNormalTexLow		: TEXCOORD4;
};

// The domain shader is called for every vertex created by the tessellator.  
// It is like the vertex shader after tessellation.
[domain("tri")]
DomainOut DS(PatchTess patchTess, float3 bary : SV_DomainLocation, const OutputPatch<HullOut,3> tri)
{
	DomainOut dout;
	
	// Interpolate patch attributes to generated vertices.
	dout.PosW     = bary.x*tri[0].PosW     + bary.y*tri[1].PosW     + bary.z*tri[2].PosW;
	dout.NormalW  = bary.x*tri[0].NormalW  + bary.y*tri[1].NormalW  + bary.z*tri[2].NormalW;
	dout.TangentW = bary.x*tri[0].TangentW + bary.y*tri[1].TangentW + bary.z*tri[2].TangentW;
	dout.Tex      = bary.x*tri[0].Tex      + bary.y*tri[1].Tex      + bary.z*tri[2].Tex;

	dout.WaveDispTexHigh   = bary.x*tri[0].WaveDispTexHigh   + bary.y*tri[1].WaveDispTexHigh   + bary.z*tri[2].WaveDispTexHigh;
	dout.WaveDispTexLow    = bary.x*tri[0].WaveDispTexLow    + bary.y*tri[1].WaveDispTexLow    + bary.z*tri[2].WaveDispTexLow;
	dout.WaveNormalTexHigh = bary.x*tri[0].WaveNormalTexHigh + bary.y*tri[1].WaveNormalTexHigh + bary.z*tri[2].WaveNormalTexHigh;
	dout.WaveNormalTexLow  = bary.x*tri[0].WaveNormalTexLow  + bary.y*tri[1].WaveNormalTexLow  + bary.z*tri[2].WaveNormalTexLow;
	
	// Interpolating normal can unnormalize it, so normalize it.
	dout.NormalW = normalize(dout.NormalW);
	
	//
	// Displacement mapping.
	//
	
	// Choose the mipmap level based on distance to the eye; specifically, choose
	// the next miplevel every MipInterval units, and clamp the miplevel in [0,6].
	const float MipInterval = 20.0f;
	float mipLevel = clamp( (distance(dout.PosW, gEyePosW) - MipInterval) / MipInterval, 0.0f, 6.0f);
	
	// Sample height map (stored in alpha channel).
	float H = gNormalMapHigh.SampleLevel(samLinear, dout.WaveDispTexHigh, mipLevel).a;
	float h = gNormalMapLow.SampleLevel(samLinear, dout.WaveDispTexLow, mipLevel).a;

	// Offset vertex along normal.
	dout.PosW.y += ((gHeightScaleHigh * H) + (gHeightScaleLow * h));
	
	// Project to homogeneous clip space.
	dout.PosH = mul(float4(dout.PosW, 1.0f), gViewProj);
	
	return dout;
}
 //-------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------Pixel Shader-------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
float4 PS(DomainOut pin, uniform int gLightCount, uniform bool gUseTexure, uniform bool gAlphaClip, uniform bool gFogEnabled, uniform bool gReflectionEnabled) : SV_Target
{
	if ((gOceanMode >= 1.0f) && (distance (float3(0.0f,0.0f,0.0f), pin.PosW) < 300))
	{
		return float4(0.0f,0.0f,0.0f,0.0f);
	}

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
		texColor = gDiffuseMap.Sample( samLinear, pin.Tex );

		if(gAlphaClip)
		{
			// Discard pixel if texture alpha < 0.1.  Note that we do this
			// test as soon as possible so that we can potentially exit the shader 
			// early, thereby skipping the rest of the shader code.
			clip(texColor.a - 0.1f);
		}
	}

	//
	// Normal mapping
	//

	//float3 normalMapSample = ((gNormalMapHigh.Sample(samLinear, pin.Tex).rgb) + (gNormalMapLow.Sample(samLinear, pin.Tex).rgb))/2;
	//float3 bumpedNormalW = NormalSampleToWorldSpace(normalMapSample, pin.NormalW, pin.TangentW);

	float3 normalMapSampleHigh = gNormalMapHigh.Sample(samLinear, pin.WaveNormalTexHigh).rgb;
	float3 bumpedNormalWHigh = NormalSampleToWorldSpace(normalMapSampleHigh, pin.NormalW, pin.TangentW);

	float3 normalMapSampleLow = gNormalMapLow.Sample(samLinear, pin.WaveNormalTexLow).rgb;
	float3 bumpedNormalWLow = NormalSampleToWorldSpace(normalMapSampleLow, pin.NormalW, pin.TangentW);

	float3 bumpedNormalW = normalize(bumpedNormalWHigh + bumpedNormalWLow);
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
			ComputeDirectionalLight(gMaterial, gDirLights[i], bumpedNormalW, toEye, 
				A, D, S);

			ambient += A;
			diffuse += D;
			spec    += S;
		}

		litColor = texColor*(ambient + diffuse) + spec;

		if( gReflectionEnabled )
		{
			float3 incident = -toEye;
			float3 reflectionVector = reflect(incident, bumpedNormalW/2);
			float4 reflectionColor  = gCubeMap.Sample(samLinear, reflectionVector);

			float3 refractionVector = refract(incident, bumpedNormalW/2, 0.9);
			float4 refractionColor  = gCubeMap.Sample(samLinear, refractionVector);

			litColor += gMaterial.Reflect*reflectionColor * refractionColor;
		}
	}

	//Submarine View
	if(gEyePosW.y < 0.5f)
	{
		float4 subColor = float4(1.0f, 0.0f, 0.0f, 1.0f);
		ComputeSubMarineView(litColor, subColor);

		litColor += subColor;

		gFogEnabled = true;
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

	//litColor.a = 0.1;

    return litColor;
}

technique11 OceanTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetHullShader( CompileShader( hs_5_0, HS() ) );
        SetDomainShader( CompileShader( ds_5_0, DS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(3, true, true, false, true) ) ); 
    }
}