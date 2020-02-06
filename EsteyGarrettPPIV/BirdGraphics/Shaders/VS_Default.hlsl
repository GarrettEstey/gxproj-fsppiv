cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
}

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

PS_INPUT main( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT) 0;
    output.pos = mul(input.pos, World);
    output.pos = mul(output.pos, View);
    output.pos = mul(output.pos, Projection);
    output.norm = mul(float4(input.norm, 1), World).xyz;
    output.tex = input.tex;
    return output;
}