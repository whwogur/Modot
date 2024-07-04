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
};

