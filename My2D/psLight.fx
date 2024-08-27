
Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

cbuffer LightBuffer 
{
    float4 diffuseColor;
    float4 ambientColor;
    float3 lightDirection;
    float padding;
};

float4 main(PixelInputType input) : SV_Target
{
    float4 color;
    float4 textureColor;
    float lightIntensity;
    float3 lightDir;
    
    color = ambientColor;
    textureColor = shaderTexture.Sample(SampleType, input.tex);
    
    lightDir = -lightDirection;
    lightIntensity = saturate(dot(input.normal, lightDir));
    
    if(lightIntensity > 0.0f)
    {
        color += (diffuseColor * lightIntensity);
    }
    
    color = saturate(color);
    
    color *= textureColor;
    
    return color;
}