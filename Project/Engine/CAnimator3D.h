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
    void SetClipTime(int _iClipIdx, float _fTime) { m_vecClipUpdateTime[_iClipIdx] = _fTime; }
    CStructuredBuffer* GetFinalBoneMat() { return m_pBoneFinalMatBuffer; }
    CStructuredBuffer* GetPrevFinalBoneMat() const { return m_pPrevBoneFinalMatBuffer; }
    void ClearData();
    void Bind();
    UINT GetBoneCount() const;

    int GetFrameCount() const { return m_iFrameCount; }
    void SetFrameCount(int _Cnt) { m_iFrameCount = _Cnt; }

    int GetFrameIdx() const { return m_iFrameIdx; }
    void SetFrameIdx(int _FrameIdx);

    int GetCurClipIdx() const { return m_iCurClipIdx; }
    void SetCurClipIdx(int _Idx) { m_iCurClipIdx = _Idx; }

    void PauseAnimation() { m_bPlay = false;}
    void ResumeAnimation() { m_bPlay = true;}
    bool IsPlayingAnim() const { return m_bPlay; }
    bool IsOnRepeat() const { return m_bRepeat; }
    void SetRepeat(bool _Repeat) { m_bRepeat = _Repeat; }
    float GetPlaySpeed() const { return m_fPlaySpeed; }
    void SetPlaySpeed(float _Speed) { m_fPlaySpeed = _Speed; }

    Ptr<CMesh> GetSkeletalMesh() { return m_SkeletalMesh; }
    bool IsValid() const { return nullptr != m_SkeletalMesh && m_SkeletalMesh->IsSkeletalMesh(); }

    UINT GetClipIdx(const wstring& _ClipName);
    void Play(const wstring& _ClipName, bool _Repeat, float _PlaySpeed, double _TransitionDuration);
    void Play(int _ClipIdx, bool _Repeat, float _PlaySpeed, double _TransitionDuration);
public:
    virtual void FinalTick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    void SetSkeletalMesh(Ptr<CMesh> _SkeletalMesh);
private:
    void CheckBoneMatBuffer();
private:
    DT_OPTIONS                              m_eTimeOption = DT_OPTIONS::NONESCALE;
    std::vector<float>				        m_vecClipUpdateTime;
    std::vector<Matrix>				        m_vecBoneTransformMat = {};              // �ؽ��Ŀ� ������ ���� �������
    std::unordered_map<std::wstring, UINT>  m_AnimationHash = {};
    Ptr<CMesh>                              m_SkeletalMesh = nullptr;

    CStructuredBuffer*                      m_pBoneFinalMatBuffer = nullptr;    // Ư�� �������� ���� ���
    CStructuredBuffer*                      m_pPrevBoneFinalMatBuffer = nullptr;// ���� ������ ���� ���
    CStructuredBuffer*                      m_pBoneTransformMatBuffer = nullptr;

    bool                                    m_bPlay = true;
    bool                                    m_bRepeat = true;
    int							            m_iFrameCount = 30;
    double						            m_dCurTime = 0.;
    int							            m_iCurClipIdx = 0;                     // Ŭ�� �ε���	
    int							            m_iFrameIdx = 0;                    // Ŭ���� ���� ������
    int							            m_iNextFrameIdx = 0;                // Ŭ���� ���� ������
    float						            m_fRatio = 0.f;	                    // ������ ���� ����
    bool						            m_bFinalMatUpdated = false;         // ������� ���� ���࿩��
    float                                   m_fPlaySpeed = 1.0f;

    // ����Ŭ��
    int                                     m_iNextClipIdx;
    bool                                    m_bTransition = false;
    double                                  m_dCurTransitionTime = 0.0;
    double                                  m_dTransitionDuration = 0.0;
    bool                                    m_bNextRepeat = false;
    float                                   m_fNextPlaySpeed = 1.0f;
};