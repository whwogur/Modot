#pragma once
#include "EditorUI.h"
#include "TexturePreview.h"

class MenuUI :
    public EditorUI
{
public:
    MenuUI() = default;
    ~MenuUI() = default;

public:
    virtual void Tick() override;
    virtual void Update() override;

private:
    void File();
    void Tools();
    void Assets();
    void External();
    void OutputInfo();

    void LoadLevel();
    void SaveLevelAs();
    void AddScript();
};