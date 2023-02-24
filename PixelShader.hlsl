//------------------------------------------------------------------------------------
// PixelShader.hlsl
//
// Simple shader to render a triangle
//
// Advanced Technology Group (ATG)
// Copyright (C) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

struct PS_INPUT
{
    float4 position : SV_Position;
    float4 color    : COLOR0;
};

struct PS_OUTPUT
{
    float4 color    : SV_Target;
};

PS_OUTPUT main( PS_INPUT In )
{
	PS_OUTPUT Out;
    Out.color = In.color;
    return Out;
}