#include "ShaderHeader.fx"

float4 main(PS_INPUT input) : SV_TARGET
{
    return float4(saturate(abs(sin(input.wPos.x))), saturate(abs(cos(input.wPos.y))), saturate(abs(tan(input.wPos.z))), 1.0f);
}