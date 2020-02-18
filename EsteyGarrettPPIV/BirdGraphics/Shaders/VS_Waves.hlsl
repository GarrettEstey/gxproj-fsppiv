#include "ShaderHeader.fx"

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    output.pos = mul(input.pos, World);
	output.pos = mul(output.pos, float4x4(float4(1.0f, 0.0f, 0.0f, 0.0f), float4(0.0f, 1.0f, 0.0f, 0.0f), float4(0.0f, 0.0f, 1.0f, 0.0f), float4(0.0f, (sin(time.x) * sin(input.pos.x) * 0.1), 0.0f, 1.0f)));
    output.wPos = output.pos;
    output.pos = mul(output.pos, View);
    output.pos = mul(output.pos, Projection);
    output.norm = mul(float4(input.norm, 1), World).xyz;
    output.tex = input.tex;
    return output;
}