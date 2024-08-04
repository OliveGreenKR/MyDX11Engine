Texture2D shaderTextures[4] : register(t0); 
SamplerState SampleType : register(s0);


cbuffer ConstantBuffer : register(b0)
{
    int textureCount;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};


float4 MultiTexturePixelShader(PixelInputType input) : SV_TARGET
{

    float4 blendColor = float4(1,1,1,1);
    
    // Loop through each texture and sample the color
    for (int i = 0; i < textureCount; ++i)
    {
        blendColor *= shaderTextures[i].Sample(SampleType, input.tex);
    }

    // Normalize the color by the number of textures
    blendColor = pow(blendColor, 1.0 / textureCount);

    // Saturate the final color
    blendColor = saturate(blendColor);

    return blendColor;
}