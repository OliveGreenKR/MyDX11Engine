Texture2D shaderTextures[2] : register(t0);
SamplerState SampleType : register(s0);

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

float4 MultiTexturePixelShader(PixelInputType input) : SV_TARGET
{

    float4 Color;
    
    float4 base = shaderTextures[0].Sample(SampleType, input.tex);
    float4 light = shaderTextures[1].Sample(SampleType, input.tex);
  
    Color = saturate(base * light);
    
    return Color;
}
