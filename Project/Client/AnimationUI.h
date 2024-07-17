#pragma once
#include "AssetUI.h"
class AnimationUI :
    public AssetUI
{
public:
    AnimationUI();
    ~AnimationUI();
public:
    virtual void Update() override;

private:
    int         m_SelectedIdx = -1;
    char        m_AnimationName[64] = {};
};

