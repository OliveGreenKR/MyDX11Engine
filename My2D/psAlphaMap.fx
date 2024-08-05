Texture2D shaderTextures[3] : register(t0);
SamplerState SampleType : register(s0);


struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

float4 MultiTexturePixelShader(PixelInputType input) : SV_TARGET
{
    float4 color1;
    float4 color2;
    float4 alphaValue;
    float4 blendColor;
	 
    // Get the pixel color from the first texture.
    color1 = shaderTextures[0].Sample(SampleType, input.tex);

    // Get the pixel color from the second texture.
    color2 = shaderTextures[1].Sample(SampleType, input.tex);

    // Get the pixel color from the alpha texture.
    alphaValue = shaderTextures[2].Sample(SampleType, input.tex);

    // Combine the two textures based on the alpha value.
    blendColor = (alphaValue * color1) + ((1.0 - alphaValue) * color2);

    // Saturate the final color value.
    blendColor = saturate(blendColor);

    return blendColor;
}