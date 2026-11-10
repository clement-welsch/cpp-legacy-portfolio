//=============================================================================
// Basic.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Basic effect that currently supports transformations, lighting, and texturing.
//=============================================================================

#include "LightHelper.fx"
 #define MAX_SUBDIVISION_LEVEL 3

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
	row_major float4x4 World  : WORLD;
	float4 Color    : COLOR;
	uint InstanceId : SV_InstanceID;
};

struct VertexOut
{
    float3 PosW    : POSITION;
    float3 NormalW : NORMAL;
	float4 Color   : COLOR;
	int SubIndice  : INTEGER;
	row_major float4x4 World  : WORLD;
};
struct GeoOut
{
	float4 PosH    : SV_POSITION;
    float3 PosW    : POSITION;
    float3 NormalW : NORMAL;
	float4 Color   : COLOR;
};
//-------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------Vertex Shader------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to world space space.
	vout.PosW    = vin.PosL;
	vout.NormalW = vin.NormalL;
	vout.World = vin.World;

	float3 worldPoint = mul(float4(vout.PosW,1.0f), vout.World).xyz;

	float d = distance(worldPoint, gEyePosW);
	vout.Color = float4(1.0f, 1.0f, 0.0f, 1.0f);
	vout.SubIndice = 0;
	if (d <20)
	{
		vout.Color = float4(1.0f, 1.0f, 0.0f, 1.0f);
		vout.SubIndice = 3;
	}
	else if (d < 40)
	{
		vout.Color = float4(0.0f, 1.0f, 1.0f, 1.0f);
		vout.SubIndice = 2;
	}
	else if(d < 50)
	{
		vout.Color = float4(1.0f, 0.0f, 1.0f, 1.0f);
		vout.SubIndice = 1;
	}
	else
	{
		vout.Color = float4(1.0f, 0.0f, 0.0f, 1.0f);
		vout.SubIndice = 0;
	}

	return vout;
}
//-------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------Geometry Shader----------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
[maxvertexcount(73)]
void GS(triangle VertexOut gin[3], inout TriangleStream<GeoOut> streamOut)
{
	int nbSubdivision = gin[0].SubIndice;
	//nbSubdivision = 0;

    if (nbSubdivision != 0 && nbSubdivision <= MAX_SUBDIVISION_LEVEL)
    {
        uint level_count = (uint)pow(2, nbSubdivision);
		//Define the size of the triangle subdived
        float3 u = (gin[1].PosW - gin[0].PosW) / level_count,
               w = (gin[2].PosW - gin[0].PosW) / level_count,
               v = gin[0].PosW;

        for (int level = level_count; level > 0; level--)
        {            
            GeoOut gout;
            gout.PosH     = mul(float4(normalize(v), 1.0f), mul(gin[0].World, gViewProj));
			gout.PosW     = mul(float4(normalize(v), 1.0f), gin[0].World);
			gout.NormalW  = mul(gin[0].NormalW, (float3x3)gWorld);
			gout.Color    = gin[0].Color;
            streamOut.Append(gout);

            float3 p = v;
            uint   vertex_count = level;
			
			//Creation of the subdived triangles by row
			//it start by the base and reach the top
			//			1
			//			*
			//		   / \      ^
			//		  /   \     |
			//	     *-----*    |
			//	    / \	  /	\   |
			//     /   \ /   \  |
			//    *-----*-----*
			//   0             2
			//   -------------->
            for (uint vertex = 0; vertex < vertex_count; vertex++)
            {
                gout.PosH = mul(float4(normalize(p + u), 1.0f), mul(gin[1].World, gViewProj));
                streamOut.Append(gout);

                gout.PosH = mul(float4(normalize(p += w), 1.0f), mul(gin[2].World, gViewProj));
                streamOut.Append(gout);
            }

            v += u;
            streamOut.RestartStrip();
        }
    }
    else
    {
		//Else the original triangle are drawn
        [unroll]
        for (uint i = 0; i < 3; ++i)
        {
			GeoOut gout;
			// Transform to world space space.
			gout.PosW     = mul(float4(gin[i].PosW, 1.0f), gin[i].World).xyz;
			gout.NormalW  = mul(gin[i].NormalW, (float3x3)gin[i].World);
			// Transform to homogeneous clip space.
			gout.PosH     = mul(float4(gout.PosW, 1.0f), gViewProj);
			gout.Color    = gin[i].Color;
            streamOut.Append(gout);
        }
    }
}
//-------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------Pixel Shader-------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
float4 PS(GeoOut pin, uniform int gLightCount) : SV_Target
{
	// Interpolating normal can unnormalize it, so normalize it.
    pin.NormalW = normalize(pin.NormalW);

	// The toEye vector is used in lighting.
	float3 toEye = gEyePosW - pin.PosW; 
	 
	// Cache the distance to the eye from this surface point.
	float distToEye = length(toEye);

	// Normalize.
	toEye /= distToEye;

	//
	// Lighting.
	//
	float4 texColor = float4(1, 1, 1, 1);
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
			ComputeDirectionalLight(gMaterial, gDirLights[i], pin.NormalW, toEye, A, D, S);

			ambient += A;
			diffuse += D;
			spec    += S;
		}

		// Modulate with late add.
		litColor = texColor * ((ambient/2) + (diffuse * 2)) + spec;
	}

	// Common to take alpha from diffuse material and texture.
	litColor.a *= texColor.a;

    return litColor * pin.Color;
}

technique11 LODInstancing
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( CompileShader( gs_5_0, GS() ) );
        SetPixelShader( CompileShader( ps_5_0, PS(2) ) );
    }
}