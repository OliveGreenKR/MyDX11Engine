#pragma once

#include <directxmath.h>
#include <fstream>
using namespace DirectX;
using namespace std;

#include "Textureclass.h"
#include "Define.h"

class FontClass
{
private:
    struct FontType
    {
        float left, right;
        int size;
    };

    struct VertexType
    {
        XMFLOAT3 position;
        XMFLOAT2 texture;
    };

public:
    FontClass();
    FontClass(const FontClass&);
    ~FontClass();

    bool Initialize(ID3D11Device*, ID3D11DeviceContext*, int);
    void Shutdown();

    ID3D11ShaderResourceView* GetTexture();

    void BuildVertexArray(void*, char*, float, float);
    int GetSentencePixelLength(char*);
    int GetFontHeight();

private:
    bool LoadFontData(char*);
    void ReleaseFontData();
    bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
    void ReleaseTexture();

    __forceinline int GetLetterIndex(const char c) { return (int)c - 32; }

private:
    const int m_fontCount = 95;

    FontType* m_Font;
    TextureClass* m_Texture;
    float m_fontHeight;
    int m_spaceSize;
};
