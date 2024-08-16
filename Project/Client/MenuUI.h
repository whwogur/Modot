#pragma once
#include "EditorUI.h"

class MenuUI :
    public EditorUI
{
public:
    MenuUI();
    ~MenuUI() = default;

public:
    virtual void Tick() override;
    virtual void Update() override;

private:
    void File();
    void Tools();
    void Assets();

    void LoadLevel();
    void SaveLevelAs();
    void AddScript();
private:
    bool        m_Mode = false; // false = dark true = light
};