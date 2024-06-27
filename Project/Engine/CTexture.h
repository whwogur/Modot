#pragma once
#include "CAsset.h"

class CTexture :
    public CAsset
{
public:
    CTexture();
    ~CTexture();
public:
    virtual int Load(const wstring& _FilePath) { return S_OK; };
    virtual int Save(const wstring& _FilePath) { return S_OK; };

private:

    WRL::ComPtr<ID3D11Texture2D>			m_Tex2D;
    D3D11_TEXTURE2D_DESC                    m_Desc;
};
