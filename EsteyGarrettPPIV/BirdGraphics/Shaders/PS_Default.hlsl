#include "ShaderHeader.fx"

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 finalColor = 0;
    
    //do NdotL lighting for light
    finalColor += saturate(dot((float3) dirLightDir, input.norm) * dirLightCol);
    finalColor *= txDiffuse.Sample(samLinear, input.tex);
    finalColor.a = 1;
    return finalColor;
}