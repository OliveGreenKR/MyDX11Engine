cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION; // 정점 위치 (클립 공간)
    float2 tex : TEXCOORD0; // 텍스처 좌표
    float3 normal : NORMAL; // 법선 벡터
    float4 worldPosition : TEXCOORD1; // 정점 위치 (월드 공간)
};

PixelInputType LightVertexShader(VertexInputType input)
{
    PixelInputType output;
    float4 worldPosition;
    int i;
    
    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;
    worldPosition = mul(input.position, worldMatrix);
    
    output.worldPosition = worldPosition;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(worldPosition, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

    // Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3)worldMatrix);
    output.normal = normalize(output.normal);
    
    return output;
}
