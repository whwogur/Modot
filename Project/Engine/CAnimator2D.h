#pragma once
#include "CComponent.h"
#include "CAnimation.h"

class CAnimator2D :
    public CComponent
{
public:
    CAnimator2D();
    ~CAnimator2D() = default;

public:
    virtual void FinalTick() override;

public:
    void AddAnimation(int _Idx, Ptr<CAnimation> _Animation);
    void PushBackAnimation(Ptr<CAnimation> _Animation);
    Ptr<CAnimation> FindAnimation(const wstring& _Key);
    void Play(int _AnimationIdx, float _FPS, bool _Repeat);
    Ptr<CSprite> GetCurSprite() { return m_CurFrmSprite; }
    vector<Ptr<CAnimation>>& GetAnimationsRef() { return m_vecAnimation; }

    void Reset();
    void Bind();
    static void Clear();

private:
    vector<Ptr<CAnimation>>     m_vecAnimation;  // �ִϸ����� ������Ʈ�� ������ ��� �ִϸ��̼� ���
    Ptr<CAnimation>             m_CurAnimation;  // ���� ������� Animation
    Ptr<CSprite>                m_CurFrmSprite; // ���� ������� Animation ���� ���� ������ �ε����� �ش��ϴ� ��������Ʈ
    int                         m_CurFrmIdx;    // ���� ������� Animation ���� ���° Sprite �� ��������� �ε��� ���
    int                         m_MaxFrm;       // ���� ������� Animation �� �ִ� ������
    float                       m_FPS;          // ���� ������� Animation �� �ʴ� ������ ���� ��
    float                       m_AccTime;      // ���� �ð��� üũ
    bool                        m_Repeat;
    bool                        m_Finish;       // Animation ����� ���� �����ߴ��� ����
};