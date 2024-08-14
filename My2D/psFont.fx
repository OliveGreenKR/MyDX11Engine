Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer PixelBuffer
{
    float4 pixelColor;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

float4 FontPixelShader(PixelInputType input) : SV_TARGET
{
    float4 color;
    // Sample the texture pixel at this location.
    color = shaderTexture.Sample(SampleType, input.tex);
	
    if (length(color.xyz) < 0.001f)
    {
        color.a = 0.0f;
    }    
    else
    {
        color.a = 1.0f;
        color = color * pixelColor;
    }
    
    return color;
}