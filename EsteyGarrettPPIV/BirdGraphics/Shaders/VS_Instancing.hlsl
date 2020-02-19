#include "ShaderHeader.fx"

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    output.pos = mul(input.pos, float4x4(float4(0.1f, 0.0f, 0.0f, 0.0f), float4(0.0f, 0.1f, 0.0f, 0.0f), float4(0.0f, 0.0f, 0.1f, 0.0f), input.instData));
    output.wPos = output.pos;
    output.pos = mul(output.pos, View);
    output.pos = mul(output.pos, Projection);
    output.norm = mul(float4(input.norm, 0.0f), World).xyz;
    output.tex = input.tex;
    return output;
}