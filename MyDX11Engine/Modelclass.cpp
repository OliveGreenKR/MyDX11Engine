#include "Modelclass.h"
#include "objLoader.h"

ModelClass::ModelClass()
{
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
	m_texture = nullptr;
    m_verteces = vector<VertexInfo>();
}

ModelClass::ModelClass(const ModelClass& other)
{
}

ModelClass::~ModelClass()
{
}

bool ModelClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* modelFilename, char* textureFilename )
{
	bool result;

    // Load in the model data.
    result = LoadModel(modelFilename);
    if (!result)
    {
        return false;
    }

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}
	// Load the texture for this model.
	result = LoadTexture(device, deviceContext, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

    // Release the model
    ReleaseModel();

	return;
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}

int ModelClass::GetIndexCount()
{
	return m_indices.size();
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_texture->GetTexture();
}

bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
    D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
    HRESULT result;
    int i;

    // Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_verteces.size();
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    // Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = m_verteces.data();
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    // Now create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
    if (FAILED(result))
    {
        return false;
    }

    // Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indices.size();
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    // Give the subresource structure a pointer to the index data.
    indexData.pSysMem = m_indices.data();
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    // Create the index buffer.
    result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
    if (FAILED(result))
    {
        return false;
    }

    return true;
}

void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool ModelClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;


	// Create and initialize the texture object.
	m_texture = new TextureClass;

	result = m_texture->Initialize(device, deviceContext, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::ReleaseTexture()
{
	// Release the texture object.
	if (m_texture)
	{
		m_texture->Shutdown();
		delete m_texture;
		m_texture = 0;
	}

	return;
}


bool ModelClass::LoadModel(char* filename)
{
    char* extend;

    extend = strrchr(filename, '.');

    if (strcmp(extend, ".txt") == 0)
	{
		return LoadTxtModel(filename);
	}
	else if (strcmp(extend, ".obj") == 0)
	{
        ObjLoader objLoader;
        return objLoader.Load(filename, m_verteces, m_indices);
	}
	else
	{
		return false;
	}

}

bool ModelClass::LoadTxtModel(char* filename) {

    unsigned long vertexCount, indexCount;
    ifstream fin;
    char input;
    int i;

    // Open the model file.
    fin.open(filename);

    // If it could not open the file then exit.
    if (fin.fail())
    {
        return false;
    }

    // Read up to the value of vertex count.
    fin.get(input);
    while (input != ':')
    {
        fin.get(input);
    }

    // Read in the vertex count.
    fin >> vertexCount;

    // Set the number of indices to be the same as the vertex count.
    indexCount = vertexCount;

    // Create the model using the vertex count that was read in.
    m_verteces.resize(vertexCount);
    m_indices.resize(indexCount);

    // Read up to the beginning of the data.
    fin.get(input);
    while (input != ':')
    {
        fin.get(input);
    }
    fin.get(input);
    fin.get(input);

    // Read in the vertex data.
    for (i = 0; i < vertexCount; i++)
    {
        fin >> m_verteces[i].x >> m_verteces[i].y >> m_verteces[i].z;
        fin >> m_verteces[i].tu >> m_verteces[i].tv;
        fin >> m_verteces[i].nx >> m_verteces[i].ny >> m_verteces[i].nz;
        m_indices[i] = i;
    }

    // Close the model file.
    fin.close();

}

void ModelClass::ReleaseModel()
{
    if (!m_verteces.empty())
    {
        m_verteces.clear();
        m_verteces.shrink_to_fit();
    }

    return;
}



