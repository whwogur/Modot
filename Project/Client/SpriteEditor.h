#pragma once
#include "EditorUI.h"
class SpriteEditor :
    public EditorUI
{
public:
    SpriteEditor();
    ~SpriteEditor();
public:
    void SetAtlas(Ptr<CTexture> _Atlas) { m_AtlasTex = _Atlas; }

public:
    virtual void Update() override;
    
private:
    Ptr<CTexture>               m_AtlasTex;
    ImVec2                      m_UVStart;
    ImVec2                      m_UVEnd;
    ImVec2                      m_ImagePos;
    ImVec2                      m_MouseStart;
    std::pair<ImVec2, ImVec2>   m_UVpair;
};
