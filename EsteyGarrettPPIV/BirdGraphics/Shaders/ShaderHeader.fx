// https://docs.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl-intrinsic-functions

Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

struct DirectionalLight
{
    float4 col;
    float4 dir;
};

cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
    DirectionalLight dirLights[2];
    float4 solidColor;
};

struct VS_INPUT
{
    float4 pos : POSITION;
    float3 norm : NORMAL;
    float2 tex : TEXCOORD;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float3 norm : NORMAL;
    float2 tex : TEXCOORD;
};