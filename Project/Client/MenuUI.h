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
    virtual void Init() override;
private:
    void File();
    void Tools();
    void Assets();
    void External();
    void GizmoStat();

    void LoadLevel();
    void SaveLevelAs();
    void AddScript();
private:
    Ptr<CTexture>       m_LogoTex;
};