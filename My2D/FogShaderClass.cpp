#include "FogShaderClass.h"
#include "Define.h"

bool FogShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
    bool result;
    wchar_t vsFilename[128];
    wchar_t psFilename[128];
    int error;

    // Set the filename of the vertex shader.
    error = wcscpy_s(vsFilename, 128, VS_FOG_PATH);
    if (error != 0)
    {
        return false;
    }

    // Set the filename of the pixel shader.
    error = wcscpy_s(psFilename, 128, PS_FOG_PATH);
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

void FogShaderClass::Shutdown()
{
	ShaderClass::ShutdownShader();
	ShutdownShader();
}

bool FogShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
    HRESULT result;
    ID3D10Blob* errorMessage;
    ID3D10Blob* vertexShaderBuffer;
    ID3D10Blob* pixelShaderBuffer;
    D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
    unsigned int numElements;
    D3D11_BUFFER_DESC matrixBufferDesc, fogBufferDesc;
    D3D11_SAMPLER_DESC samplerDesc;


    // Initialize the pointers this function will use to null.
    errorMessage = 0;
    vertexShaderBuffer = 0;
    pixelShaderBuffer = 0;
#pragma region vertex, pixel shaders
    // Compile the vertex shader code.
    result = D3DCompileFromFile(vsFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
                                &vertexShaderBuffer, &errorMessage);
    if (FAILED(result))
    {
        // If the shader failed to compile it should have writen something to the error message.
        if (errorMessage)
        {
            ShaderClass::OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
        }
        // If there was nothing in the error message then it simply could not find the shader file itself.
        else
        {
            MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
        }

        return false;
    }
    // Compile the pixel shader code.
    result = D3DCompileFromFile(psFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
                                &pixelShaderBuffer, &errorMessage);
    if (FAILED(result))
    {
        // If the shader failed to compile it should have writen something to the error message.
        if (errorMessage)
        {
            OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
        }
        // If there was nothing in the error message then it simply could not find the file itself.
        else
        {
            MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
        }

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
    result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
                                       vertexShaderBuffer->GetBufferSize(), &m_layout);
    if (FAILED(result))
    {
        return false;
    }

    // Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
    vertexShaderBuffer->Release();
    vertexShaderBuffer = 0;

    pixelShaderBuffer->Release();
    pixelShaderBuffer = 0;
#pragma endregion
#pragma region cBufferDescs
    // Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBufferDesc.MiscFlags = 0;
    matrixBufferDesc.StructureByteStride = 0;

    fogBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    fogBufferDesc.ByteWidth = sizeof(FogBufferType);
    fogBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    fogBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    fogBufferDesc.MiscFlags = 0;
    fogBufferDesc.StructureByteStride = 0;

    // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
    result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
    if (FAILED(result))
    {
        return false;
    }

    result = device->CreateBuffer(&fogBufferDesc, NULL, &m_fogBuffer);
    if (FAILED(result))
    {
        return false;
    }
#pragma endregion
#pragma region samplerDesc
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
#pragma endregion
    return true;
}

void FogShaderClass::ShutdownShader()
{
    if(m_fogBuffer)
	{
		m_fogBuffer->Release();
		m_fogBuffer = nullptr;
	}
    if(m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = nullptr;
	}
}

bool FogShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, FogShaderParameters& parameters)
{
    HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    MatrixBufferType* dataPtr;
    FogBufferType* dataPtr2;

    XMMATRIX worldMatrix, viewMatrix, projection;
    worldMatrix = XMMatrixTranspose(parameters.worldMatrix);
    viewMatrix = XMMatrixTranspose(parameters.viewMatrix);
    projection = XMMatrixTranspose(parameters.projectionMatrix);

#pragma region matrixBuffer
    result =  deviceContext->Map(m_matrixBuffer,0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if(FAILED(result))
	{
		return false;
	}

    dataPtr = (MatrixBufferType*)mappedResource.pData;
    dataPtr->world = worldMatrix;
    dataPtr->view = viewMatrix;
    dataPtr->projection = projection;

    deviceContext->Unmap(m_matrixBuffer, 0);
#pragma endregion
#pragma region fogBuffer
    result = deviceContext->Map(m_fogBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result))
	{
		return false;
	}
    
    dataPtr2 = (FogBufferType*)mappedResource.pData;
    dataPtr2->fogColor = parameters.fogColor;
    dataPtr2->fogStart = parameters.fogStart;
    dataPtr2->fogEnd = parameters.fogEnd;
    dataPtr2->fogDensity = parameters.fogDensity;
    dataPtr2->fogType = parameters.fogType;

    deviceContext->Unmap(m_fogBuffer, 0);
#pragma endregion

    deviceContext->VSSetConstantBuffers(0, 1, &m_matrixBuffer);

    deviceContext->VSSetConstantBuffers(1, 1, &m_fogBuffer);
    deviceContext->PSSetConstantBuffers(0, 1, &m_fogBuffer);

    deviceContext->PSSetShaderResources(0,1, &parameters.baseTexture);
    deviceContext->PSSetSamplers(0, 1, &m_sampleState);
    return true;
}

