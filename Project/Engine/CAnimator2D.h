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
    map<wstring, CAnimation*>       m_mapAnim;  // Animator2D ������Ʈ�� ������ ��� �ִϸ��̼� ���
    CAnimation*                     m_CurAnim;  // ���� ������� �ִϸ��̼�
    int                             m_CurFrm;   // ���� ������� �ִϸ��̼� ���� ���° Sprite �� ��������� �ε��� ���
};