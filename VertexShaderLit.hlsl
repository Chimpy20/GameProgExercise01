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
	float4 vTint;
}

struct VS_INPUT
{
    float4 position     : POSITION;
	float3 normal       : NORMAL;
    float4 color        : COLOR0;
};

struct VS_OUTPUT
{
    float4 position     : SV_POSITION;
	float3 normal       : TEXCOORD0;
    float4 color        : COLOR0;
};

VS_OUTPUT main( VS_INPUT input )
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.color = input.color * vTint;
	input.position.w = 1.0f;
	output.position = mul( input.position, mWorld );
	output.position = mul( output.position, mViewProjection );
	output.normal = mul( input.normal, (float3x3)mWorld );
	output.normal = normalize( output.normal );

	return output;
}