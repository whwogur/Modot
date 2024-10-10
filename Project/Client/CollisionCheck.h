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
    void EditLayerName();
private:
    UINT    m_CollisionCheck[MAX_LAYER];
    string  m_LayerNames[MAX_LAYER];
    char    m_Buff[255] = {};
    bool    m_Edit = false;
    UINT    m_Selected = -1;
};

