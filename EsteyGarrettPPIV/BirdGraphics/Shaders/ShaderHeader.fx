// https://docs.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl-intrinsic-functions

#include "../lightDefines.h"

Texture2D txDiffuse : register(t0);
TextureCube cubeDiffuse : register(t1);
SamplerState samLinear : register(s0);

struct DirectionalLight
{
    float4 col;
    float4 dir;
};

struct PointLight
{
    float4 col;
    float4 pos;
    float4 rad;
};

struct SpotLight
{
    float4 col;
    float4 pos;
    float4 dir;
    float4 rat;
};

cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
    DirectionalLight dirLights[DIRLIGHTCOUNT];
    PointLight pointLights[POINTLIGHTCOUNT];
    SpotLight spotLights[SPOTLIGHTCOUNT];
    float4 solidColor;
    float2 time;
    float2 useDirLights;
    float2 usePointLights;
    float2 useSpotLights;
};

struct VS_INPUT
{
    float4 pos : POSITION;
    float3 norm : NORMAL;
    float2 tex : TEXCOORD;
    float4 instData : INSTANCEDATA;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 lPos : POSITION;
    float4 wPos : POSITION1;
    float4 color : COLOR;
    float3 norm : NORMAL;
    float2 tex : TEXCOORD;
};