cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer FogBuffer
{
    float4 fogColor;
    float fogStart;
    float fogEnd;
    float fogDensity;
    int fogType;
};

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float fogFactor : FOG;
};

PixelInputType main(VertexInputType input)
{
    PixelInputType output;
    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    float viewDistance = output.position.z; 
    output.position = mul(output.position, projectionMatrix);
    
    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;
    
    
    switch(fogType)
    {
        case 0 :
            // Calculate linear fog.    
            output.fogFactor = saturate((fogEnd - viewDistance) / (fogEnd - fogStart));
            break;
        case 1 :
            // Calculate exponential fog.
            output.fogFactor = saturate(exp(-fogDensity * viewDistance));
            break;
        case 2 :
            // Calculate exponential squared fog.
            output.fogFactor = saturate(exp(-fogDensity * fogDensity * viewDistance * viewDistance));
            break;
        default:
            output.fogFactor = saturate((fogEnd - viewDistance) / (fogEnd - fogStart));
            break;
    }
    
    return output;
}



