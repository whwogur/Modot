#pragma once
#include "EditorUI.h"
#include "CTileMap.h"

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
    void SetTilemap(CTileMap* _Tilemap);
private:
    void Edit(UINT vecTileIdx, UINT oldImgIdx, UINT newImgIdx);
    void Undo();
    void Revert();
    void Commit();
private:
    CTileMap*                           m_Tilemap;
    ImVec2                              m_ImagePos = {};
    ImVec2                              m_ImageButtonPos = {};
    vector<std::pair<UINT, UINT>>       m_EditHistory;
    std::vector<tTileInfo>              m_TilemapToBeEdited;
    bool                                m_Altered;
    std::pair<int, int>                 m_RowCol;
};