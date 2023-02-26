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
	float4x4 mView;
	float4x4 mProjection;
}

cbuffer Constants : register( b1 )
{
	float4x4 mWorld;
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
	output.color = input.color;
	float4 inputPos = float4(input.position, 1.0f);
	output.position = mul( inputPos, mWorld );
	output.position = mul( output.position, mView );
	output.position = mul( output.position, mProjection );

	return output;
}