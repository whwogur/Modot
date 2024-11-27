#pragma once
#include "CComponent.h"

class CStructuredBuffer;
class CAnimator3D :
    public CComponent
{
public:
    CLONE(CAnimator3D);
    CAnimator3D();
    CAnimator3D(const CAnimator3D& _Other);
    ~CAnimator3D();

public:
    void SetBones(const std::vector<tMTBone>* _vecBones) { m_pVecBones = _vecBones; m_vecFinalBoneMat.resize(m_pVecBones->size()); }
    void SetAnimClip(const std::vector<tMTAnimClip>* _vecAnimClip);
    void SetClipTime(int _iClipIdx, float _fTime) { m_vecClipUpdateTime[_iClipIdx] = _fTime; }
    CStructuredBuffer* GetFinalBoneMat() { return m_pBoneFinalMatBuffer; }
    UINT GetBoneCount() { return (UINT)m_pVecBones->size(); }
    void ClearData();
    void Bind();

public:
    virtual void FinalTick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void check_mesh(Ptr<CMesh> _pMesh);

private:
    const std::vector<tMTBone>*     m_pVecBones = nullptr;
    const std::vector<tMTAnimClip>* m_pVecClip = nullptr;
    std::vector<float>				m_vecClipUpdateTime;
    std::vector<Matrix>				m_vecFinalBoneMat;              // 텍스쳐에 전달할 최종 행렬정보
    int							    m_iFrameCount = 30;
    double						    m_dCurTime = 0.;
    int							    m_iCurClip = 0;                 // 클립 인덱스	
    int							    m_iFrameIdx = 0;                // 클립의 현재 프레임
    int							    m_iNextFrameIdx = 0;            // 클립의 다음 프레임
    float						    m_fRatio = 0.f;	                // 프레임 사이 비율
    CStructuredBuffer*              m_pBoneFinalMatBuffer;          // 특정 프레임의 최종 행렬
    bool						    m_bFinalMatUpdate = false;      // 최종행렬 연산 수행여부
};