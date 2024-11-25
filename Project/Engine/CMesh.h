#pragma once
#include "CAsset.h"
#include "CFBXLoader.h"
struct tIndexInfo
{
    WRL::ComPtr<ID3D11Buffer>    pIB;
    D3D11_BUFFER_DESC            tIBDesc;
    UINT				         iIdxCount;
    void*                        pIdxSysMem;
};

class CMesh :
    public CAsset
{
public:
    CMesh();
    ~CMesh();
public:
    static CMesh* CreateFromContainer(CFBXLoader& _loader);
    int Create(Vtx* _VtxSysMem, UINT _VtxCount, UINT* _IdxSysMem, UINT _IdxCount);
    void Bind(UINT _Subset);
    void Render(UINT _Subset);
    void Render_Particle(UINT _Count);

    UINT GetVertexCount() { return m_VtxCount; }
    UINT GetSubsetCount() { return (UINT)m_vecIdxInfo.size(); }

    void* GetVtxSysMem() { return m_VtxSysMem; }

    virtual int Load(const wstring& _FilePath) { return S_OK; }
    virtual int Save(const wstring& _FilePath) { return S_OK; }

private:
    WRL::ComPtr<ID3D11Buffer>       m_VB;
    D3D11_BUFFER_DESC               m_VBDesc = {};
    UINT                            m_VtxCount;
    Vtx*                            m_VtxSysMem;
    
    std::vector<tIndexInfo>		    m_vecIdxInfo;// �ϳ��� ���ؽ����ۿ� �������� �ε������۰� ����
};

