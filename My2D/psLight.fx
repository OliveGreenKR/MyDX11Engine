Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

#define NUM_LIGHTS 4

struct Light
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightPosition;
    float specularPower;
    float4 specularColor;
    float3 attenuation;
    float range;
};

cbuffer LightBufferType
{
    Light lights[NUM_LIGHTS];
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
    float3 reflection;
    float4 specular;
    float4 color;
    float attenuation;
    float distance;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(SampleType, input.tex);
    //Initialize the specular color.
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    color = float4(0, 0, 0, 1.0f);
    
    
    for (int i = 0; i < NUM_LIGHTS; i++)
    {
        color = saturate(color);
        
        Light light = lights[i];
    
        // Set the default output color to the ambient light value for all pixels.
        color += light.ambientColor;
        
        float3 lightvector = light.lightPosition - input.worldPosition;
        distance = length(lightvector);
        lightDir = normalize(lightvector);
        viewDir = normalize(cameraPosition - input.worldPosition);
        
        if(distance > light.range)
        {
            continue;
        }
        
        //calculate the attenuation factor
        attenuation = min(1.0f, (1.0f / dot(light.attenuation, float3(1.0f, distance, distance * distance))) );
        //attenuation = 1.0f;
        // Calculate the amount of light on this pixel.
        lightIntensity = saturate(dot(input.normal, lightDir));
  
        if (lightIntensity > 0.0f)
        {
            // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
            color += light.diffuseColor * lightIntensity * attenuation ;
        
            // Calculate the reflection vector based on the light intensity, normal vector, and light direction.
            reflection = normalize(2.0f * lightIntensity * input.normal - lightDir);
        
            // Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
            specular += light.specularColor * pow(saturate(dot(reflection, viewDir)), light.specularPower) * attenuation;
        }
    }
    // Multiply the texture pixel and the final diffuse color to get the final pixel color result.
    color = saturate(color) * textureColor;
    
    // Add the specular component last to the output color.
    color = saturate(color + specular);

    return color;
}