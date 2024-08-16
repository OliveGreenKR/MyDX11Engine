
Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

float4 main(PixelInputType input) : SV_TARGET
{
	float4 textureColor;
    float4 color;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(SampleType, input.tex);

    textureColor = float4(1, 1, 1, 0.5f);
    //textureColor.a = 0.5f;
    return textureColor;
}
