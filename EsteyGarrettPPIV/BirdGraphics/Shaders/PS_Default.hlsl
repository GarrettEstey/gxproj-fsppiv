#include "ShaderHeader.fx"

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 finalColor = 0;
    // Lighting Calculations
    // First, the main, ambient light of the scene
    float4 lightRatio = saturate(dot((float3) dirLightDir[0], input.norm));
    lightRatio = saturate(lightRatio + 0.4f);
    finalColor += lightRatio * dirLightCol[0];
    // Then all the other lights
    finalColor += saturate(dot((float3) dirLightDir[1], input.norm) * dirLightCol[1]);
    // Texture Calculation
    finalColor *= txDiffuse.Sample(samLinear, input.tex);
    // Set alpha to 1
    finalColor.a = 1;
    return finalColor;
}