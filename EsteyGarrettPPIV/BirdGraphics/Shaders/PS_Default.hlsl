#include "ShaderHeader.fx"

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 finalColor = input.color;
    // Lighting Calculations
    
    // DIRECTIONAL
    // First, the main, ambient light of the scene
    float lightRatio = saturate(dot((float3) dirLights[0].dir, input.norm));
    lightRatio = saturate(lightRatio + 0.4f);
    finalColor += lightRatio * dirLights[0].col;
    // Then all the other lights
    //finalColor += saturate(dot((float3) dirLights[1].dir, input.norm) * dirLights[1].col);
    
    // POINT
    for (int i = 0; i < POINTLIGHTCOUNT; i++)
    {
        float3 lightDir = normalize(pointLights[i].pos - input.wPos);
        lightRatio = saturate(dot(lightDir, input.norm));
        float rAttenuation = 1.0f - saturate(length(pointLights[i].pos - input.wPos) / pointLights[i].rad.r);
        finalColor += lightRatio * rAttenuation * pointLights[i].col;
    }
    
    // Texture Calculation
    finalColor *= txDiffuse.Sample(samLinear, input.tex);
    // Set alpha to 1
    finalColor.a = 1.0f;
    
    return finalColor;
}