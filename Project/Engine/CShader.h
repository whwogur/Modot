#pragma once
#include "CAsset.h"

class CShader :
    public CAsset
{
public:
    CShader(ASSET_TYPE _Type);
    ~CShader();
protected:
    WRL::ComPtr<ID3DBlob> m_ErrBlob;
};

