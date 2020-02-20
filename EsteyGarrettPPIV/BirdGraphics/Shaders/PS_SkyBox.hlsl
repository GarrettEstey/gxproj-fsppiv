#include "ShaderHeader.fx"

float4 main(PS_INPUT input) : SV_TARGET
{
    return cubeDiffuse.Sample(samLinear, input.lPos.xyz);
}