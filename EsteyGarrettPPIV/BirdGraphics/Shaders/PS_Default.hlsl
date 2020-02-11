#include "ShaderHeader.fx"

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 finalColor = 0;
    // Lighting Calculation
    float4 lightRatio = saturate(dot((float3) dirLightDir, input.norm));
    lightRatio = saturate(lightRatio + 0.25f);
    finalColor += lightRatio * dirLightCol;
    // Texture Calculation
    finalColor *= txDiffuse.Sample(samLinear, input.tex);
    // Set alpha to 1
    finalColor.a = 1;
    return finalColor;
}