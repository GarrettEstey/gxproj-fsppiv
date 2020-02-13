#include "ShaderHeader.fx"

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 finalColor = 0;
    // Lighting Calculations
    // First, the main, ambient light of the scene
    float4 lightRatio = saturate(dot((float3) dirLights[0].dir, input.norm));
    lightRatio = saturate(lightRatio + 0.4f);
    finalColor += lightRatio * dirLights[0].col;
    // Then all the other lights
    finalColor += saturate(dot((float3) dirLights[1].dir, input.norm) * dirLights[1].col);
    // Texture Calculation
    finalColor *= txDiffuse.Sample(samLinear, input.tex);
    // Set alpha to 1
    finalColor.a = 1;
    return finalColor;
}