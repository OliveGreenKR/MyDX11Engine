Texture2D shaderTextures[2] : register(t0);
SamplerState SampleType : register(s0);


cbuffer ConstantBuffer : register(b0)
{
    int textureCount;
};

cbuffer LightBuffer
{
    float4 diffuseColor;
    float3 lightDirection;
    float padding;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
};

float4 MultiTexturePixelShader(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
    float4 bumpMap;
    float3 bumpNormal;
    float3 lightDir;
    float lightIntensity;
    float4 color;

	
    // Sample the pixel color from the color texture at this location.
    textureColor = shaderTextures[0].Sample(SampleType, input.tex);

    // Sample the pixel from the normal map.
    bumpMap = shaderTextures[1].Sample(SampleType, input.tex);

    // Expand the range of the normal value from (0, +1) to (-1, +1).
    bumpMap = (bumpMap * 2.0f) - 1.0f;

    // Calculate the normal from the data in the normal map.
    bumpNormal = (bumpMap.x * input.tangent) + (bumpMap.y * input.binormal) + (bumpMap.z * input.normal);

    // Normalize the resulting bump normal.
    bumpNormal = normalize(bumpNormal);

    // Invert the light direction for calculations.
    lightDir = -lightDirection;

    // Calculate the amount of light on this pixel based on the normal map value.
    lightIntensity = saturate(dot(bumpNormal, lightDir));

    // Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
    color = saturate(diffuseColor * lightIntensity);

    // Combine the final light color with the texture color.
    color = color * textureColor;

    return color;
}
