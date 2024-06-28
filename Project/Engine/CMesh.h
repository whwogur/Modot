#pragma once
#include "CAsset.h"

class CMesh :
    public CAsset
{
public:
    CMesh();
    ~CMesh();
public:
    int Create(Vtx* _VtxSysMem, UINT _VtxCount, UINT* _IdxSysMem, UINT _IdxCount);
    void Bind();
    void render();

    virtual int Load(const wstring& _FilePath) { return S_OK; }
    virtual int Save(const wstring& _FilePath) { return S_OK; }

private:
    WRL::ComPtr<ID3D11Buffer>	m_VB;
    WRL::ComPtr<ID3D11Buffer>	m_IB;

    UINT                    m_VtxCount;
    UINT                    m_IdxCount;

    D3D11_BUFFER_DESC       m_VBDesc;
    D3D11_BUFFER_DESC       m_IBDesc;

    void* m_VtxSysMem;
    void* m_IdxSysMem;
};

