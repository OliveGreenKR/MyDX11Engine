cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer CameraBuffer
{
    float3 cameraPosition;
    float padding;
};

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    //float3 tangent : TANGENT; // ÅºÁ¨Æ® º¤ÅÍ
    //float3 binormal : BINORMAL; // ¹ÙÀÌ³ë¸Ö º¤ÅÍ
    //float4 color : COLOR; // Á¤Á¡ »ö»ó
};

struct PixelInputType
{
    float4 position : SV_POSITION; // Á¤Á¡ À§Ä¡ (Å¬¸³ °ø°£)
    float2 tex : TEXCOORD0; // ÅØ½ºÃ³ ÁÂÇ¥
    float3 normal : NORMAL; // ¹ý¼± º¤ÅÍ
    float3 view : TEXCOORD1; // ºä º¤ÅÍ
    //float4 color : COLOR; // Á¤Á¡ »ö»ó
};

float3 GetViewVector(matrix viewMatrix)
{
    // View Vector(z-axis)
    float3 viewVector = float3(viewMatrix[2][0], viewMatrix[2][1], viewMatrix[2][2]);
    
    viewVector = normalize(viewVector);
  
    return viewVector;
}

PixelInputType LightVertexShader(VertexInputType input)
{
    PixelInputType output;
    float4 worldPosition;
    
    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

    // Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3)worldMatrix);
    output.normal = normalize(output.normal);
    
    // Calculate the position of the vertex in the world.
    worldPosition = mul(input.position, worldMatrix);

    // Determine the viewing direction based on the position of the camera and the position of the vertex in the world.
    output.view = cameraPosition.xyz - worldPosition.xyz;
	
    // Normalize the viewing direction vector.
    output.view = normalize(output.view);
    
    
    return output;
}
