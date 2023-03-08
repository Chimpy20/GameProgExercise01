//------------------------------------------------------------------------------------
// PixelShader.hlsl
//
// Simple shader to render a triangle
//
// Advanced Technology Group (ATG)
// Copyright (C) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

cbuffer Constants : register( b0 )
{
	float4x4 mViewProjection;
	float4   vAmbient;
	float4   vDirLight0;
	float4   vDirLight0Col;
}

struct PS_INPUT
{
    float4 position : SV_Position;
	float3 normal   : TEXCOORD0;
    float4 color    : COLOR0;
};

float4 main( PS_INPUT input ) : SV_Target
{
	float4 output;
	//float3 testLightDir = float3( 0.0f, -1.0f, 0.0f );
	//float3 testNormal = float3( 0.0f, 1.0f, 0.0f );
	//float4 testLightCol = float4( 0.5f, 0.4f, 0.3f, 1.0f );
	float4 litColour = dot((float3)vDirLight0, -input.normal ) * vDirLight0Col;
	float4 maxLight = saturate( vAmbient + litColour );
	output = input.color * maxLight;
    return output;
}