#pragma once
#include "EditorUI.h"

class MenuUI :
    public EditorUI
{
public:
    MenuUI();
    ~MenuUI();

public:
    virtual void Tick() override;
    virtual void Update() override;

private:
    void File();
    void Level();
    void AddScripts();
    void Assets();

    void LoadLevel();
    void SaveLevelAs();
private:
    void AddScript();


};