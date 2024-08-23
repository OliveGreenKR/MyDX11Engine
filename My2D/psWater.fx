SamplerState SampleType : register(s0);
Texture2D reflectionTexture : register(t0);
Texture2D refractionTexture : register(t1);
Texture2D normalTexture : register(t2);


cbuffer WaterBuffer
{
    float waterTranslation;
    float reflectRefractScale;
    float2 padding;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float4 reflectionPosition : TEXCOORD1;
    float4 refractionPosition : TEXCOORD2;
};

float4 main(PixelInputType input) : SV_TARGET
{
    float2 reflectTexCoord;
    float2 refractTexCoord;
    float4 normalMap;
    float3 normal;
    float4 reflectionColor;
    float4 refractionColor;
    float4 color;
    
    //moving water.
    input.tex.y += waterTranslation;
    
     // Calculate texture coordinates. NDC[-1:1] -> UV[0:1]
    reflectTexCoord.x = input.reflectionPosition.x / input.reflectionPosition.w / 2.0f + 0.5f;
    reflectTexCoord.y = -input.reflectionPosition.y / input.reflectionPosition.w / 2.0f + 0.5f;
	
    refractTexCoord.x = input.refractionPosition.x / input.refractionPosition.w / 2.0f + 0.5f;
    refractTexCoord.y = -input.refractionPosition.y / input.refractionPosition.w / 2.0f + 0.5f;
    

    // Normal. UV[0:1] -> [-1:1]
    normalMap = normalTexture.Sample(SampleType, input.tex);
    normal = (normalMap.xyz * 2.0f) - 1.0f;
    
    //Distort Reflection,Refraction coord by Ripple 
    reflectTexCoord = reflectTexCoord + (normal.xy * reflectRefractScale);
    refractTexCoord = refractTexCoord + (normal.xy * reflectRefractScale);
    
    //Sample color from the texture
    reflectionColor = reflectionTexture.Sample(SampleType, reflectTexCoord);
    refractionColor = refractionTexture.Sample(SampleType, refractTexCoord);
    
    // Combine the reflection and refraction results for the final color.
    color = lerp(reflectionColor, refractionColor, 0.6f);
	
    return color;
}