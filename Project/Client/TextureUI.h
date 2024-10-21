#pragma once
#include "AssetUI.h"
class TextureUI :
    public AssetUI
{
public:
    TextureUI();
    ~TextureUI() = default;
public:
    virtual void Update() override;

};