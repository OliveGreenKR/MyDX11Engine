#include "RefractionShaderClass.h"
#include "Define.h"

RefractionShaderClass::RefractionShaderClass()
    : ShaderClass(), m_matrixBuffer(nullptr), m_lightBuffer(nullptr), m_clipPlaneBuffer(nullptr)
{
}

RefractionShaderClass::~RefractionShaderClass()
{
    Shutdown();
}

bool RefractionShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
    bool result;
    wchar_t vsFilename[128];
    wchar_t psFilename[128];
    int error;

    // Set the filename of the vertex shader.
    error = wcscpy_s(vsFilename, 128, VS_REFRAC_PATH);
    if (error != 0)
    {
        return false;
    }

    // Set the filename of the pixel shader.
    error = wcscpy_s(psFilename, 128, PS_REFRAC_PATH);
    if (error != 0)
    {
        return false;
    }

    // Initialize the vertex and pixel shaders.
    result = InitializeShader(device, hwnd, vsFilename, psFilename);
    if (!result)
    {
        return false;
    }

    return true;
}

void RefractionShaderClass::Shutdown()
{
    ShaderClass::ShutdownShader();
    ShutdownShader();
}

bool RefractionShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
    HRESULT result;
    ID3D10Blob* vertexShaderBuffer;
    ID3D10Blob* pixelShaderBuffer;
    D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
    unsigned int numElements;
    D3D11_BUFFER_DESC matrixBufferDesc, lightBufferDesc, ClipBufferDesc;
    D3D11_SAMPLER_DESC samplerDesc;

    // Initialize the pointers this function will use to null.
    vertexShaderBuffer = 0;
    pixelShaderBuffer = 0;

    // Compile the vertex shader code.
    result = CompileShaderFromFile(vsFilename, "main", "vs_5_0", &vertexShaderBuffer);
    if (FAILED(result))
    {
        return false;
    }
    result = CompileShaderFromFile(psFilename, "main", "ps_5_0", &pixelShaderBuffer);
    if (FAILED(result))
    {
        return false;
    }

    // Create the vertex shader from the buffer.
    result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
    if (FAILED(result))
    {
        return false;
    }

    // Create the pixel shader from the buffer.
    result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
    if (FAILED(result))
    {
        return false;
    }

    // Create the vertex input layout description.
    polygonLayout[0].SemanticName = "POSITION";
    polygonLayout[0].SemanticIndex = 0;
    polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[0].InputSlot = 0;
    polygonLayout[0].AlignedByteOffset = 0;
    polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[0].InstanceDataStepRate = 0;

    polygonLayout[1].SemanticName = "TEXCOORD";
    polygonLayout[1].SemanticIndex = 0;
    polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
    polygonLayout[1].InputSlot = 0;
    polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[1].InstanceDataStepRate = 0;

    polygonLayout[2].SemanticName = "NORMAL";
    polygonLayout[2].SemanticIndex = 0;
    polygonLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
    polygonLayout[2].InputSlot = 0;
    polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[2].InstanceDataStepRate = 0;

    // Get a count of the elements in the layout.
    numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

    // Create the vertex input layout.
    result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);
    if (FAILED(result))
    {
        return false;
    }

    // Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
    vertexShaderBuffer->Release();
    vertexShaderBuffer = 0;

    pixelShaderBuffer->Release();
    pixelShaderBuffer = 0;

    // Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBufferDesc.MiscFlags = 0;
    matrixBufferDesc.StructureByteStride = 0;

    // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
    result = device->CreateBuffer(&matrixBufferDesc, nullptr, &m_matrixBuffer);
    if (FAILED(result))
    {
        return false;
    }

    // Setup the description of the light constant buffer that is in the pixel shader.
    lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    lightBufferDesc.ByteWidth = sizeof(LightBufferType);
    lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    lightBufferDesc.MiscFlags = 0;
    lightBufferDesc.StructureByteStride = 0;

    // Create the light constant buffer pointer so we can access the pixel shader constant buffer from within this class.
    result = device->CreateBuffer(&lightBufferDesc, nullptr, &m_lightBuffer);
    if (FAILED(result))
    {
        return false;
    }


    ClipBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    ClipBufferDesc.ByteWidth = sizeof(ClipPlaneBufferType);
    ClipBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    ClipBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    ClipBufferDesc.MiscFlags = 0;
    ClipBufferDesc.StructureByteStride = 0;

    // Create the light constant buffer pointer so we can access the pixel shader constant buffer from within this class.
    result = device->CreateBuffer(&ClipBufferDesc, nullptr, &m_clipPlaneBuffer);
    if (FAILED(result))
    {
        return false;
    }

    // Create a texture sampler state description.
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    // Create the texture sampler state.
    result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
    if (FAILED(result))
    {
        return false;
    }

    return true;
}

void RefractionShaderClass::ShutdownShader()
{
    // Release the clip plane constant buffer.
    if(m_clipPlaneBuffer)
	{
		m_clipPlaneBuffer->Release();
		m_clipPlaneBuffer = nullptr;
	}

    // Release the light constant buffer.
    if (m_lightBuffer)
    {
        m_lightBuffer->Release();
        m_lightBuffer = nullptr;
    }

    // Release the matrix constant buffer.
    if (m_matrixBuffer)
    {
        m_matrixBuffer->Release();
        m_matrixBuffer = nullptr;
    }
}

bool RefractionShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, RefractionShaderParameters& parameters)
{
    HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    MatrixBufferType* dataPtr;
    LightBufferType* lightPtr;
    ClipPlaneBufferType* clipPtr;

    // MatrixBuffer
    result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result))
    {
        return false;
    }

    dataPtr = (MatrixBufferType*)mappedResource.pData;

    dataPtr->world = XMMatrixTranspose(parameters.world);
    dataPtr->view = XMMatrixTranspose(parameters.view);
    dataPtr->projection = XMMatrixTranspose(parameters.projection);

    deviceContext->Unmap(m_matrixBuffer, 0);

    // LightBuffer
    result = deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result))
    {
        return false;
    }

    lightPtr = (LightBufferType*)mappedResource.pData;

    lightPtr->ambientColor = parameters.ambientColor;
    lightPtr->diffuseColor = parameters.diffuseColor;
    lightPtr->lightDirection = parameters.lightDirection;
    lightPtr->padding = 0.0f;

    deviceContext->Unmap(m_lightBuffer, 0);

    // ClipPlaneBuffer
    result = deviceContext->Map(m_clipPlaneBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result))
	{
		return false;
	}

    clipPtr = (ClipPlaneBufferType*)mappedResource.pData;

    clipPtr->clipPlane = parameters.clipPlane;

    deviceContext->Unmap(m_clipPlaneBuffer, 0);

    // Set Constant Buffers
    deviceContext->VSSetConstantBuffers(0, 1, &m_matrixBuffer);
    deviceContext->VSSetConstantBuffers(1, 1, &m_clipPlaneBuffer);

    deviceContext->PSSetConstantBuffers(0, 1, &m_lightBuffer);

    // Set the shader texture resource
    deviceContext->PSSetShaderResources(0, 1, &parameters.texture);

    return true;
}