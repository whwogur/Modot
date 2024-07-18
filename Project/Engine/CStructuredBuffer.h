#pragma once
#include "CEntity.h"

class CStructuredBuffer :
    public CEntity
{

public:
    CStructuredBuffer();
    ~CStructuredBuffer();
    CLONE(CStructuredBuffer);
public:
    UINT GetElementCount() { return m_ElementCount; }
    UINT GetElementSize() { return m_ElementSize; }
    UINT GetBufferSize() { return m_ElementCount * m_ElementSize; }

    void SetData(void* _pData, UINT _DataSize = 0);
    void Bind(UINT _RegisterNum);

public:
    int Create(UINT _ElementSize, UINT _ElementCount, void* _InitData = nullptr);

private:
    WRL::ComPtr<ID3D11Buffer>                   m_SB;
    D3D11_BUFFER_DESC                           m_Desc;
    WRL::ComPtr<ID3D11ShaderResourceView>       m_SRV;

    UINT                                m_ElementCount;
    UINT                                m_ElementSize;
};

