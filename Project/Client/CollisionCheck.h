#pragma once
#include "EditorUI.h"

class CollisionCheck :
    public EditorUI
{
public:
    CollisionCheck();
    ~CollisionCheck() = default;

public:
    virtual void Update() override;
    virtual void Activate() override;
private:
    void Refresh();

private:
    UINT    m_CollisionCheck[EFFECTIVE_LAYER];
    string  m_LayerNames[EFFECTIVE_LAYER];
};

