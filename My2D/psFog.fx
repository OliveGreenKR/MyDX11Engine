#include "vsFog.fx"

Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

float4 main(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
    float4 color;
    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(SampleType, input.tex);
    
    // Blend the fog color with the pixel color based on the fog factor.
    color = lerp(fogColor, textureColor, input.fogFactor);
    //color = float4(1 - input.fogFactor,  1- input.fogFactor, 1- input.fogFactor, 1.f);
    
    return color;
}
