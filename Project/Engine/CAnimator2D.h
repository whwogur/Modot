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
    vector<Ptr<CAnimation>>     m_vecAnimation;  // 애니메이터 컴포넌트가 보유한 모든 애니메이션 목록
    Ptr<CAnimation>             m_CurAnimation;  // 현재 재생중인 Animation
    Ptr<CSprite>                m_CurFrmSprite; // 현재 재생중인 Animation 에서 현재 프레임 인덱스에 해당하는 스프라이트
    int                         m_CurFrmIdx;    // 현재 재생중인 Animation 에서 몇번째 Sprite 가 재생중인지 인덱스 기록
    int                         m_MaxFrm;       // 현재 재생중인 Animation 의 최대 프레임
    float                       m_FPS;          // 현재 재생중인 Animation 의 초당 프레임 진행 수
    float                       m_AccTime;      // 누적 시간값 체크
    bool                        m_Repeat;
    bool                        m_Finish;       // Animation 재생이 끝에 도달했는지 여부
};