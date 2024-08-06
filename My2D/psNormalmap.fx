Texture2D shaderTextures[3] : register(t0);
SamplerState SampleType : register(s0);


cbuffer ConstantBuffer : register(b0)
{
    int textureCount;
    float3 CameraPosition;
};

cbuffer LightBuffer : register(b1)
{
    float4 diffuseColor;
    float4 specularColor;
    float specularPower;
    float3 lightDirection;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 worldPosition : TEXCOORD1;
};

float4 MultiTexturePixelShader(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
    float4 bumpMap;
    float3 bumpNormal;
    float3 lightDir;
    float lightIntensity;
    float4 color;
    float4 specularIntensity;
    float3 reflection;
    float4 specular;
    
    float3 viewDirection;
    
    viewDirection =  normalize(CameraPosition - input.worldPosition);

	
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
    
    if (lightIntensity > 0.0f)
    {
        specularIntensity = shaderTextures[2].Sample(SampleType, input.tex);
        reflection = normalize(2 * lightIntensity * bumpNormal - lightDir);
        specular = pow(saturate(dot(reflection, viewDirection)), specularPower);
        
        specular = specular * specularIntensity;
        color = saturate(color + specular);
    }
    return color;
}
