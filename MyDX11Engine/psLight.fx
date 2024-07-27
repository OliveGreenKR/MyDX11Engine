Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);


cbuffer LightBuffer
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightPosition;
    float specularPower;
    float4 specularColor;
};

cbuffer CameraBuffer
{
    float3 cameraPosition;
    float padding;
};


struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 worldPosition : TEXCOORD1; // 정점 위치 (월드 공간)
};

float4 LightPixelShader(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
    float3 lightDir;
    float3 viewDir;
    float lightIntensity;
    float4 color;
    float3 reflection;
    float4 specular;
    
    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(SampleType, input.tex);
    
    // Set the default output color to the ambient light value for all pixels.
    color = ambientColor;
    
    // Initialize the specular color.
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    lightDir = normalize(lightPosition - input.worldPosition);
    
    //viewDir = normalize(float3(0, 0, -15.0) - input.worldPosition);
    viewDir = normalize(cameraPosition - input.worldPosition);
    //viewDir = normalize(float3(0,0,-1));
    
    // Calculate the amount of light on this pixel.
    lightIntensity = saturate(dot(input.normal, lightDir));
    
    if (lightIntensity > 0.0f)
    {
        // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
        color += (diffuseColor * lightIntensity);
        
        //Saturate final color
        color = saturate(color);

        // Calculate the reflection vector based on the light intensity, normal vector, and light direction.
        reflection = normalize(2.0f * lightIntensity * input.normal - lightDir);
        
        // Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
        specular = pow(saturate(dot(reflection, viewDir)), specularPower);
    }
    
    // Multiply the texture pixel and the final diffuse color to get the final pixel color result.
    color = color * textureColor;
    
    // Add the specular component last to the output color.
    color = saturate(color + specular);

    return color;
}