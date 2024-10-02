#pragma once
#include "ComponentUI.h"
class TextRenderUI :
    public ComponentUI
{
public:
    TextRenderUI();
    ~TextRenderUI() = default;

private:
    virtual void Update() override;
};