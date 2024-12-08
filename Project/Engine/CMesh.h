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
    void RenderInstance(UINT _iSubset);

    UINT GetVertexCount() { return m_VtxCount; }
    UINT GetSubsetCount() { return (UINT)m_vecIdxInfo.size(); }

    size_t GetClipCount() { return m_vecAnimClip.size(); }
    void* GetVtxSysMem() { return m_VtxSysMem; }

    const std::vector<tMTBone>* GetBones() { return &m_vecBones; }
    UINT GetBoneCount() { return (UINT)m_vecBones.size(); }
    const std::vector<tMTAnimClip>* GetAnimClip() { return &m_vecAnimClip; }
    bool IsAnimMesh() { return !m_vecAnimClip.empty(); }
    bool IsSkeletalMesh() const { return !m_vecBones.empty(); }

    CStructuredBuffer* GetBoneFrameDataBuffer() { return m_pBoneFrameData; }    // 전체 본 프레임 정보
    CStructuredBuffer* GetBoneInverseBuffer() { return  m_pBoneInverse; }	    // 각 Bone 의 Inverse 행렬

    virtual int Load(const wstring& _RelativePath) override;
    virtual int Save(const wstring& _RelativePath) override;

private:
    WRL::ComPtr<ID3D11Buffer>       m_VB = {};
    D3D11_BUFFER_DESC               m_VBDesc = {};
    UINT                            m_VtxCount;
    Vtx*                            m_VtxSysMem;
    
    std::vector<tIndexInfo>		    m_vecIdxInfo;       // 하나의 버텍스버퍼에 여러개의 인덱스버퍼가 연결

    // Animation3D 정보
    std::vector<tMTAnimClip>		m_vecAnimClip;
    std::vector<tMTBone>			m_vecBones;
    CStructuredBuffer*              m_pBoneFrameData;   // 전체 본 프레임 정보(크기, 이동, 회전) (프레임 개수만큼)
    CStructuredBuffer*              m_pBoneInverse;	    // 각 뼈의 offset 행렬(각 뼈의 위치를 되돌리는 행렬) (1행 짜리)
};

