//--------------------------------------------------------------------------------------
// VertexShader.hlsl
//
// Simple vertex shader for rendering a triangle
//
// Advanced Technology Group (ATG)
// Copyright (C) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer Constants : register( b0 )
{
	float4x4 mViewProjection;
	float4   vAmbient;
	float4   vDirLight0;
	float4   vDirLight0Col;
}

cbuffer Constants : register( b1 )
{
	float4x4 mWorld;
	float vTint;
}

struct VS_INPUT
{
    float3 position     : POSITION;
    float4 color        : COLOR0;
};

struct VS_OUTPUT
{
    float4 position     : SV_POSITION;
    float4 color        : COLOR0;
};

VS_OUTPUT main( VS_INPUT input )
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.color = input.color * vTint;
	float4 inputPos = float4(input.position, 1.0f);
	output.position = mul( inputPos, mWorld );
	output.position = mul( output.position, mViewProjection );

	return output;
}