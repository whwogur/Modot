#pragma once
#include "CEntity.h"

enum SB_TYPE
{
    SRV_ONLY,
    SRV_UAV,
};

class CStructuredBuffer :
    public CEntity
{

public:
    CStructuredBuffer();
    ~CStructuredBuffer() = default;
    CLONE(CStructuredBuffer);
public:
    UINT GetElementCount() { return m_ElementCount; }
    UINT GetElementSize() { return m_ElementSize; }
    UINT GetBufferSize() { return m_ElementCount * m_ElementSize; }

    void SetData(void* _pData, UINT _DataSize = 0);
    void GetData(void* _pData, UINT _DataSize = 0);

    void Bind(UINT _RegisterNum);
    void Clear(UINT _RegisterNum);

    // ComputeShader - t register
    void Bind_CS_SRV(UINT _RegisterNum);
    void Clear_CS_SRV();

    // ComputeShader - u register
    void Bind_CS_UAV(UINT _RegisterNum);
    void Clear_CS_UAV();

public:
    int Create(UINT _ElementSize, UINT _ElementCount, SB_TYPE _Type, bool _SysMemMove = false, void* _InitData = nullptr);

private:
    WRL::ComPtr<ID3D11Buffer>                        m_SB;
    WRL::ComPtr<ID3D11Buffer>                        m_SB_Write;
    WRL::ComPtr<ID3D11Buffer>                        m_SB_Read;
    D3D11_BUFFER_DESC                                m_Desc;
    WRL::ComPtr<ID3D11ShaderResourceView>            m_SRV;
    WRL::ComPtr<ID3D11UnorderedAccessView>           m_UAV;

    SB_TYPE                                          m_Type;
    bool                                             m_SysMemMove;
                                                     
    UINT                                             m_ElementCount;
    UINT                                             m_ElementSize;
    UINT                                             m_LastBoundRegNum;
};

