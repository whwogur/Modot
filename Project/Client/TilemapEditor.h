#pragma once
#include "EditorUI.h"
class CTileMap;

class TilemapEditor :
    public EditorUI
{
public:
    TilemapEditor();
    ~TilemapEditor() = default;

public:
    virtual void Update() override;

public:
    void SetTilemap(CTileMap* _Tilemap) { m_Tilemap = _Tilemap; }

private:
    CTileMap*   m_Tilemap;
    ImVec2      m_ImagePos = {};
};