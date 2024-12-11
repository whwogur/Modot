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
    ~CAnimator3D() = default;
public:
    void SetBones(const std::vector<tMTBone>* _vecBones) { m_pVecBones = _vecBones; m_vecFinalBoneMat.resize(m_pVecBones->size()); }
    void SetClipIdx(int _Idx) { m_iCurClip = _Idx; }
    void SetAnimClip(const std::vector<tMTAnimClip>* _vecAnimClip);
    void SetClipTime(int _iClipIdx, float _fTime) { m_vecClipUpdateTime[_iClipIdx] = _fTime; }
    int  GetCurClipIdx() const { return m_iCurClip; }

    std::weak_ptr<CStructuredBuffer> GetFinalBoneMat() { return m_pBoneFinalMatBuffer; }
    UINT GetBoneCount() { return (UINT)m_pVecBones->size(); }
    void ClearData();

    void Bind();

public:
    const std::vector<tMTAnimClip>* GetCurClip() { return m_pVecClip; }
    void SetFrameIdx(int _Idx) { m_iFrameIdx = _Idx; }
    int GetFrameIdx() const { return m_iFrameIdx; }

    int GetFrameCount() const { return m_iFrameCount; }

    int GetNextFrameIdx() const { return m_iNextFrameIdx; }
    float GetFrameRatio() const { return m_fRatio; }

    bool IsPlayingAnim() const { return m_bPlay; }
    void SetPlay(bool _Play) { m_bPlay = _Play; }

    void SetPlaySpeed(float _Speed) { m_fPlaySpeed = _Speed; }
    float GetPlaySpeed() const { return m_fPlaySpeed; }

    void SetRepeat(bool _Repeat) { m_bRepeat = _Repeat; }
    bool IsOnRepeat() const { return m_bRepeat; }
public:
    virtual void FinalTick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void check_mesh(Ptr<CMesh> _pMesh);
private:
    const std::vector<tMTBone>*                     m_pVecBones;
    const std::vector<tMTAnimClip>*                 m_pVecClip;

    std::vector<float>				                m_vecClipUpdateTime;
    std::vector<Matrix>				                m_vecFinalBoneMat;      // �ؽ��Ŀ� ������ ���� �������
    int							                    m_iFrameCount;          // 30
    double						                    m_dCurTime;
    int							                    m_iCurClip;             // Ŭ�� �ε���	

    int							                    m_iFrameIdx;            // Ŭ���� ���� ������
    int							                    m_iNextFrameIdx;        // Ŭ���� ���� ������
    float						                    m_fRatio;	            // ������ ���� ����

    std::shared_ptr<CStructuredBuffer>              m_pBoneFinalMatBuffer;  // Ư�� �������� ���� ���
    bool						                    m_bFinalMatUpdate;      // ������� ���� ���࿩��
    bool                                            m_bPlay;
    float                                           m_fPlaySpeed;
    bool                                            m_bRepeat;
};