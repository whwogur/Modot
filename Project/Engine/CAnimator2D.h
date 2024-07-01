#pragma once
#include "CComponent.h"

class CAnimation;

class CAnimator2D :
    public CComponent
{
public:
    virtual void FinalTick() override;

public:
    CLONE(CAnimator2D);
    CAnimator2D();
    ~CAnimator2D();

private:
    map<wstring, CAnimation*>       m_mapAnim;  // Animator2D 컴포넌트가 보유한 모든 애니메이션 목록
    CAnimation*                     m_CurAnim;  // 현재 재생중인 애니메이션
    int                             m_CurFrm;   // 현재 재생중인 애니메이션 에서 몇번째 Sprite 가 재생중인지 인덱스 기록
};