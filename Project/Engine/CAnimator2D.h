#pragma once
#include "CComponent.h"
#include "CAnimation.h"

class CAnimator2D :
    public CComponent
{
public:
    CAnimator2D();
    ~CAnimator2D() = default;
    CAnimator2D(const CAnimator2D& _Other);
    CLONE(CAnimator2D);
public:
    virtual void FinalTick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    void AddAnimation(int _Idx, Ptr<CAnimation> _Animation);
    void PushBackAnimation(Ptr<CAnimation> _Animation);
    Ptr<CAnimation> FindAnimation(const wstring& _Key);
    void Play(int _AnimationIdx, float _FPS, bool _Repeat);
    void Play(const wstring& _AnimName, float _FPS, bool _Repeat);
    Ptr<CSprite> GetCurSprite() { return m_CurFrmSprite; }
    vector<Ptr<CAnimation>>& GetAnimationsRef() { return m_vecAnimation; }
    float& GetFPSRef() { return m_FPS; }
    bool& GetRepeatRef() { return m_Repeat; }

    void Reset();
    void Bind();
    static void Clear();

private:
    vector<Ptr<CAnimation>>     m_vecAnimation;  // �ִϸ����� ������Ʈ�� ������ ��� �ִϸ��̼� ���
    Ptr<CAnimation>             m_CurAnimation;  // ���� ������� Animation
    Ptr<CSprite>                m_CurFrmSprite; // ���� ������� Animation ���� ���� ������ �ε����� �ش��ϴ� ��������Ʈ
    int                         m_CurFrmIdx;    // ���� ������� Animation ���� ���° Sprite �� ��������� �ε��� ���
    float                       m_FPS;          // ���� ������� Animation �� �ʴ� ������ ���� ��
    float                       m_AccTime;      // ���� �ð��� üũ
    bool                        m_Repeat;
    bool                        m_Finish;       // Animation ����� ���� �����ߴ��� ����
};