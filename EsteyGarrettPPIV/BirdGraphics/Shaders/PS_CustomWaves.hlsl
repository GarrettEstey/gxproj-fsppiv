#include "ShaderHeader.fx"

float4 main(PS_INPUT input) : SV_TARGET
{
	float4 color = float4(saturate(abs(sin(input.wPos.x))), saturate(abs(cos(input.wPos.y))), saturate(abs(tan(input.wPos.z))), 1.0f);
    return color;
}