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
    void ClearData();
    void Bind();

public:
    UINT GetBoneCount() { return (UINT)m_pVecBones->size(); }
    const std::vector<tMTAnimClip>* GetClips() { return m_pVecClip; }
    const std::vector<tMTBone>* GetBones() { return m_pVecBones; }
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
    std::vector<Matrix>				m_vecFinalBoneMat;              // �ؽ��Ŀ� ������ ���� �������
    int							    m_iFrameCount = 30;
    double						    m_dCurTime = 0.;
    int							    m_iCurClip = 0;                 // Ŭ�� �ε���	
    int							    m_iFrameIdx = 0;                // Ŭ���� ���� ������
    int							    m_iNextFrameIdx = 0;            // Ŭ���� ���� ������
    float						    m_fRatio = 0.f;	                // ������ ���� ����
    CStructuredBuffer*              m_pBoneFinalMatBuffer = nullptr;          // Ư�� �������� ���� ���
    bool						    m_bFinalMatUpdate = false;      // ������� ���� ���࿩��
};