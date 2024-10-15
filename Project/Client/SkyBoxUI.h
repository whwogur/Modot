#pragma once
#include "ComponentUI.h"
class SkyBoxUI :
    public ComponentUI
{
public:
    SkyBoxUI();
    ~SkyBoxUI() = default;

public:
    virtual void Update() override;

private:
    Ptr<CTexture>       m_CubemapTex;
    Ptr<CTexture>       m_NullTexture;
};

