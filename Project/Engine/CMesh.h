#pragma once
#include "CAsset.h"
#include "CFBXLoader.h"
class CStructuredBuffer;

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
    friend class CAssetMgr;
    CMesh();
    ~CMesh();
public:
    static CMesh* CreateFromContainer(CFBXLoader& _loader);
    int Create(Vtx* _VtxSysMem, UINT _VtxCount, UINT* _IdxSysMem, UINT _IdxCount);

    void Bind(UINT _Subset);
    void BindInstance(UINT _iSubset);

    void Render(UINT _Subset);
    void Render_Particle(UINT _Count);
    void Render_Instancing(UINT _iSubset);

    UINT GetVertexCount() { return m_VtxCount; }
    UINT GetSubsetCount() { return (UINT)m_vecIdxInfo.size(); }

    size_t GetClipCount() { return m_vecAnimClip.size(); }
    void* GetVtxSysMem() { return m_VtxSysMem; }

    const std::vector<tMTBone>* GetBones() { return &m_vecBones; }
    UINT GetBoneCount() { return (UINT)m_vecBones.size(); }
    const std::vector<tMTAnimClip>* GetAnimClip() { return &m_vecAnimClip; }
    bool IsAnimMesh() { return !m_vecAnimClip.empty(); }

    CStructuredBuffer* GetBoneFrameDataBuffer() { return m_pBoneFrameData; }    // ��ü �� ������ ����
    CStructuredBuffer* GetBoneInverseBuffer() { return  m_pBoneInverse; }	    // �� Bone �� Inverse ���
    CStructuredBuffer* GetBoneFrameDataBufferByIndex(int _Idx) { return m_pVecBoneFrameData[_Idx]; }

    virtual int Load(const wstring& _RelativePath) override;
    virtual int Save(const wstring& _RelativePath) override;

private:
    WRL::ComPtr<ID3D11Buffer>       m_VB;
    D3D11_BUFFER_DESC               m_VBDesc;
    UINT                            m_VtxCount;
    Vtx*                            m_VtxSysMem;

    // �ϳ��� ���ؽ����ۿ� �������� �ε������۰� ����
    std::vector<tIndexInfo>		    m_vecIdxInfo;

    // Animation3D ����
    std::vector<tMTAnimClip>		m_vecAnimClip;
    std::vector<tMTBone>			m_vecBones;

    CStructuredBuffer*              m_pBoneFrameData;    // ��ü �� ������ ����(ũ��, �̵�, ȸ��) (������ ������ŭ)
    CStructuredBuffer*              m_pBoneInverse;	     // �� ���� Inverse ���(�� ���� ��ġ�� �ǵ����� ���) (1�� ¥��)

    std::vector<CStructuredBuffer*> m_pVecBoneFrameData;
};

