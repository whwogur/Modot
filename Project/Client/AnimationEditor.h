#pragma once
#include "EditorUI.h"

class AnimationEditor :
    public EditorUI
{
public:
    AnimationEditor();
    ~AnimationEditor() = default;

public:
    virtual void Update() override;

public:
    void SetAnimation(Ptr<CAnimation> _Anim) { m_Animation = _Anim; }
    void Refresh();
private:
    Ptr<CAnimation>     m_Animation;
    int m_CurrentFrame = 0, m_StartFrame = 0, m_EndFrame = 0;
    float m_FPS = 1.0f, m_Acc = 0.0f;
    bool m_Play;
};

