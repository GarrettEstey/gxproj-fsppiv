#include "ShaderHeader.fx"

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 finalColor = input.color;
    // Lighting Calculations
    
    // DIRECTIONAL LIGHTS
    {
        // First, the main, ambient light of the scene
        if(useDirLights.x != 0)
        {
            float lightRatio = saturate(dot(dirLights[0].dir.xyz, input.norm));
            lightRatio = saturate(lightRatio + 0.4f);
            finalColor += lightRatio * dirLights[0].col;
        }
        for (int i = (int) useDirLights.x; i < (int) useDirLights.y; i++)
		{
            // Then all the other lights
            finalColor += saturate(dot(dirLights[i].dir.xyz, input.norm) * dirLights[i].col);
		}
	}
    
    // POINT LIGHTS
    {
        for (int i = (int) usePointLights.x; i < (int) usePointLights.y; i++)
        {
            float mag = length(pointLights[i].pos.xyz - input.wPos.xyz);
            float3 lightDir = normalize(pointLights[i].pos.xyz - input.wPos.xyz);
            float lightRatio = saturate(dot(lightDir, input.norm));
            float rAttenuation = 1.0f - saturate(mag / pointLights[i].rad.x);
            finalColor += lightRatio * pointLights[i].col * rAttenuation;
        }
    }
    
    // SPOT LIGHTS
    {
        for (int i = (int) useSpotLights.x; i < (int) useSpotLights.y; i++)
        {
            float3 lightDir = normalize(spotLights[i].pos.xyz - input.wPos.xyz);
        }
    }
    
    // Texture Calculation
    finalColor *= txDiffuse.Sample(samLinear, input.tex);
    // Set alpha to 1
    finalColor.a = 1.0f;
    
    return finalColor;
}