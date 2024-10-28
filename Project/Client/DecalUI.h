#pragma once
#include "ComponentUI.h"
class DecalUI :
    public ComponentUI
{
public:
    DecalUI();
    ~DecalUI() = default;

public:
    void SelectDecalTexture(DWORD_PTR _ListUI);
    void SelectEmissiveTexture(DWORD_PTR _ListUI);
public:
    virtual void Update() override;
};

