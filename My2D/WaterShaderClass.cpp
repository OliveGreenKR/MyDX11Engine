#include "WaterShaderClass.h"
#include "Define.h"
WaterShaderClass::WaterShaderClass() 
    : ShaderClass() , m_matrixBuffer(nullptr), m_reflectionBuffer(nullptr), m_waterBuffer(nullptr)
{
}

WaterShaderClass::~WaterShaderClass()
{
    Shutdown();
}

bool WaterShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
    bool result;
    wchar_t vsFilename[128];
    wchar_t psFilename[128];
    int error;

    // Set the filename of the vertex shader.
    error = wcscpy_s(vsFilename, 128, VS_WATER_PATH);
    if (error != 0)
    {
        return false;
    }

    // Set the filename of the pixel shader.
    error = wcscpy_s(psFilename, 128, PS_WATER_PATH);
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

void WaterShaderClass::Shutdown()
{
    ShaderClass::ShutdownShader();
    ShutdownShader();
}

bool WaterShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
    HRESULT result;
    ID3D10Blob* vertexShaderBuffer;
    ID3D10Blob* pixelShaderBuffer;
    D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
    unsigned int numElements;
    D3D11_BUFFER_DESC matrixBufferDesc, reflectionBufferDesc, waterBufferDesc;
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

    // Setup the description of the reflection constant buffer that is in the vertex shader.
    reflectionBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    reflectionBufferDesc.ByteWidth = sizeof(ReflectionBufferType);
    reflectionBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    reflectionBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    reflectionBufferDesc.MiscFlags = 0;
    reflectionBufferDesc.StructureByteStride = 0;

    // Create the reflection constant buffer pointer so we can access the vertex shader constant buffer from within this class.
    result = device->CreateBuffer(&reflectionBufferDesc, nullptr, &m_reflectionBuffer);
    if (FAILED(result))
    {
        return false;
    }

    // Setup the description of the water constant buffer that is in the pixel shader.
    waterBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    waterBufferDesc.ByteWidth = sizeof(WaterBufferType);
    waterBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    waterBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    waterBufferDesc.MiscFlags = 0;
    waterBufferDesc.StructureByteStride = 0;

    // Create the water constant buffer pointer so we can access the pixel shader constant buffer from within this class.
    result = device->CreateBuffer(&waterBufferDesc, nullptr, &m_waterBuffer);
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

void WaterShaderClass::ShutdownShader()
{
    // Release the water constant buffer.
    if (m_waterBuffer)
    {
        m_waterBuffer->Release();
        m_waterBuffer = nullptr;
    }

    // Release the reflection constant buffer.
    if (m_reflectionBuffer)
    {
        m_reflectionBuffer->Release();
        m_reflectionBuffer = nullptr;
    }

    // Release the matrix constant buffer.
    if (m_matrixBuffer)
    {
        m_matrixBuffer->Release();
        m_matrixBuffer = nullptr;
    }
}

bool WaterShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, WaterShaderParameters& parameters)
{
    HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    MatrixBufferType* dataPtr;
    ReflectionBufferType* reflectionPtr;
    WaterBufferType* waterPtr;

//MatrixBuffer
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

//ReflectionBuffer
    result = deviceContext->Map(m_reflectionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result))
    {
        return false;
    }

    reflectionPtr = (ReflectionBufferType*)mappedResource.pData;

    reflectionPtr->reflection = XMMatrixTranspose(parameters.reflection);

    deviceContext->Unmap(m_reflectionBuffer, 0);

//WaterBuffer
    result = deviceContext->Map(m_waterBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result))
    {
        return false;
    }

    waterPtr = (WaterBufferType*)mappedResource.pData;

    waterPtr->waterTranslation = parameters.waterTranslation;
    waterPtr->reflectRefractScale = parameters.reflectRefractScale;
    waterPtr->padding = XMFLOAT2(0.0f, 0.0f);

    deviceContext->Unmap(m_waterBuffer, 0);

//Set Constant Buffers
    deviceContext->VSSetConstantBuffers(0, 1, &m_matrixBuffer);
    deviceContext->VSSetConstantBuffers(1, 1, &m_reflectionBuffer);

    deviceContext->PSSetConstantBuffers(0, 1, &m_waterBuffer);

// Set the shader texture resources
    deviceContext->PSSetShaderResources(0, 1, &parameters.reflectionTexture);
    deviceContext->PSSetShaderResources(1, 1, &parameters.refractionTexture);
    deviceContext->PSSetShaderResources(2, 1, &parameters.normalTexture);

    return true;
}