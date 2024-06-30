#pragma once
#include "CAsset.h"

class CShader :
    public CAsset
{
public:
    CShader(ASSET_TYPE _Type);
    virtual ~CShader() = default;

public:
    virtual int Load(const wstring& _FilePath) { return S_OK; }
    virtual int Save(const wstring& _FilePath) { return S_OK; }

protected:
    WRL::ComPtr<ID3DBlob> m_ErrBlob;
};

