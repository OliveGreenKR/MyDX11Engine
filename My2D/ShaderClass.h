#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <fstream>
#include <directxmath.h>

using namespace DirectX;
using namespace std;

template <class ShaderParameters>
class ShaderClass
{
public:

	ShaderClass() : m_vertexShader(nullptr), m_pixelShader(nullptr), m_layout(nullptr), m_sampleState(nullptr) {};
	ShaderClass(const ShaderClass&) = delete; //복사 방지
	~ShaderClass() { ShutdownShader(); };

	virtual bool Initialize(ID3D11Device*, HWND) = 0;
	virtual void Shutdown() = 0;

	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, ShaderParameters& parameters);

private:
	virtual bool InitializeShader(ID3D11Device*, HWND, WCHAR* vsFilename, WCHAR* psFilename) = 0;
	virtual bool SetShaderParameters(ID3D11DeviceContext*, ShaderParameters& parameters) = 0;

	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);
protected:
	void ShutdownShader();
	static void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
	{
		char* compileErrors;
		unsigned long long bufferSize, i;
		ofstream fout;


		// Get a pointer to the error message text buffer.
		compileErrors = (char*)(errorMessage->GetBufferPointer());

		// Get the length of the message.
		bufferSize = errorMessage->GetBufferSize();

		// Open a file to write the error message to.
		fout.open("shader-error.txt");

		// Write out the error message.
		for (i = 0; i < bufferSize; i++)
		{
			fout << compileErrors[i];
		}

		// Close the file.
		fout.close();

		// Release the error message.
		errorMessage->Release();
		errorMessage = 0;

		// Pop a message up on the screen to notify the user to check the text file for compile errors.
		MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

		return;
	}
	

protected:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_sampleState;
};

template<class ShaderParameters>
bool ShaderClass<ShaderParameters>::Render(ID3D11DeviceContext* deviceContext, int indexCount, ShaderParameters& parameters)
{
	bool result;
	result = SetShaderParameters(deviceContext, parameters);
	if (!result)
		return false;
	RenderShader(deviceContext, indexCount);
	return true;
}

template <class ShaderParameters>
void ShaderClass<ShaderParameters>::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(m_layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	// Render the triangle.
	deviceContext->DrawIndexed(indexCount, 0, 0);
	return;
}

template<class ShaderParameters>
inline void ShaderClass<ShaderParameters>::ShutdownShader()
{
	// Release the sampler state.
	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}

	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}
}
