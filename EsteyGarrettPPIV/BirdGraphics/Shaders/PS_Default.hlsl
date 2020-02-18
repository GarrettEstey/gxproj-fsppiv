#include "ShaderHeader.fx"

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 finalColor = input.color;
    // Lighting Calculations
    
    // DIRECTIONAL LIGHTS
    {
        // First, the main, ambient light of the scene
		float lightRatio = saturate(dot((float3) dirLights[0].dir, input.norm));
		lightRatio = saturate(lightRatio + 0.4f);
		finalColor += lightRatio * dirLights[0].col;
        // Then all the other lights
        //finalColor += saturate(dot((float3) dirLights[1].dir, input.norm) * dirLights[1].col);
	}
    
    // POINT LIGHTS
	for (int i = 0; i < POINTLIGHTCOUNT; i++)
    {
		float mag = length(pointLights[i].pos - input.wPos);
        float4 lightDir = normalize(pointLights[i].pos - input.wPos);
        float lightRatio = saturate(dot(lightDir, (float4)(input.norm, 1.0f)));
        float rAttenuation = 1.0f - saturate(mag / pointLights[i].rad.x);
		finalColor += lightRatio * pointLights[i].col * rAttenuation;
	}
    
    // Texture Calculation
    finalColor *= txDiffuse.Sample(samLinear, input.tex);
    // Set alpha to 1
    finalColor.a = 1.0f;
    
    return finalColor;
}