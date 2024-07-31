#pragma once
#include "EditorUI.h"
#include "SpriteEditor.h"
#include <Engine/CAssetMgr.h>
class SE_Sub :
    public EditorUI
{
public:
    SE_Sub();
    virtual ~SE_Sub() = default;
public:
    SpriteEditor* GetOwner() { return m_Owner; }
    class SE_Detail* GetDetail() { return m_Owner->GetDetail(); }
    class SE_AtlasView* GetAtlasView() { return m_Owner->GetAtlasView(); }


private:
    SpriteEditor*   m_Owner;
    friend class    SpriteEditor;
};