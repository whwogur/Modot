#include "pch.h"
#include "TilemapEditor.h"
#include "CAssetMgr.h"
#include "TreeUI.h"
TilemapEditor::TilemapEditor()
	: m_Tilemap(nullptr)
    , m_Altered(false)
    , m_RowCol{1, 1}
{
}

void TilemapEditor::Init()
{
    UseHorizontalScroll(true);
}

void TilemapEditor::Update()// 정리 필요..;
{
	if (m_Tilemap != nullptr)
	{
        if (m_Altered)
        {
            m_Tilemap->SetRowCol(m_RowCol.first, m_RowCol.second);
            m_TilemapToBeEdited.resize(m_RowCol.first * m_RowCol.second);
            m_Altered = false;
        }

		Ptr<CTexture> atlasTex = m_Tilemap->GetAtlasTexture();
		Vec2 sliceUV = m_Tilemap->GetTileSliceUV();
		const wstring& atlasName = atlasTex->GetKey();
		string strKey(atlasName.begin(), atlasName.end());
		int row = m_Tilemap->GetRowCol().first;
		int col = m_Tilemap->GetRowCol().second;
        int maxAtlasRow = m_Tilemap->GetMaxAtlasRow();
        int maxAtlasCol = m_Tilemap->GetMaxAtlasCol();
        static int selTileIndex = 0;

		// ============
        // Selector
        // ============
        static bool bOpen = IsActive();
        static bool enableGrid = false;
        bool& bTransparent = IsTransparentRef();
        ImGui::Begin(ICON_FA_HAND_POINTER_O "##TileAtlasSelector", &bOpen, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse);
        ImGui::TextColored(HEADER_2, u8"투명");
        ImGui::SameLine();
        ToggleButton("##TilemapTransparent", &bTransparent);
        ImGui::TextColored(HEADER_1, u8"아틀라스 :");
        ImGui::SameLine();
        const map<wstring, Ptr<CAsset>>& pTextures = CAssetMgr::GetInst()->GetAssets(ASSET_TYPE::TEXTURE);
        ImGui::SetNextItemWidth(200);
        if (ImGui::BeginCombo(ICON_FA_PICTURE_O "##TexturePrevDropbox", strKey.c_str()))
        {
            for (const auto& texture : pTextures)
            {
                string strTexName(texture.first.begin(), texture.first.end());
                if (ImGui::MenuItem(strTexName.c_str()))
                {
                    m_Tilemap->SetAtlasTexture((CTexture*)texture.second.Get());
                }
            }
            ImGui::EndCombo();
        }
        ImGui::SetItemTooltip(u8"타일맵 아틀라스를\n선택해주세요");
        ImGui::SameLine();
        ImGui::Checkbox(u8"그리드", &enableGrid);
        if (!m_EditHistory.empty())
        {
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_UNDO, { 30, 30 }))
            {
                Undo();
            }
            //ImGui::SameLine();
            //std::pair<UINT, UINT> latest = m_EditHistory.back();
            //string temp = std::to_string(latest.first) + u8"번째 칸 수정";
            //ImGui::Text(temp.c_str());
        }
        ImGui::NewLine();
        
        m_ImageButtonPos = ImGui::GetCursorPos();
        float ButtonSize = 30.0f;
        int atlasWidth = atlasTex->GetDesc().Width;
        int atlasHeight = atlasTex->GetDesc().Height;
        Vec2 atlasTileSize = m_Tilemap->GetAtlasTileSize();
        float uvX = atlasTileSize.x / atlasWidth;
        float uvY = atlasTileSize.y / atlasHeight;
        int sIdx = 0;
        static string strIdx;
        for (int i = 0; i < maxAtlasRow; ++i)
        {
            for (int j = 0; j < maxAtlasCol; ++j)
            {
                if (j > 0) ImGui::SameLine();

                ImGui::SetCursorPosX(m_ImageButtonPos.x + j * (ButtonSize + 7));
                ImGui::SetCursorPosY(m_ImageButtonPos.y + i * (ButtonSize + 7));
                strIdx = "##AtlasSelectButton" + std::to_string(sIdx);
                if (ImGui::ImageButton(strIdx.c_str(), atlasTex->GetSRV().Get(),
                    ImVec2(ButtonSize, ButtonSize),
                    ImVec2(uvX * j, uvY * i),
                    ImVec2(uvX * (j + 1), uvY * (i + 1))))
                {
                    selTileIndex = sIdx;
                }
                ++sIdx;
            }
        }
        ImGui::NewLine();
        if (ImGui::Button("Commit", { 70, 30 }))
        {
            Commit();
        }
        ImGui::SameLine();
        if (ImGui::Button("Revert", { 70, 30 }))
        {
            Revert();
        }

        ImGui::Text(u8"타일맵 행/열");
        ImGui::SameLine(100);
        ImGui::SetNextItemWidth(50);
        if (ImGui::InputInt("##TilemapRow", &m_RowCol.first, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            m_Altered = true;
        }
        ImGui::SameLine();
        ImGui::SetNextItemWidth(50);
        if (ImGui::InputInt("##TilemapCol", &m_RowCol.second, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            m_Altered = true;
        }

        auto& [AtlasTileSizeX, AtlasTileSizeY] = m_Tilemap->GetAtlasTileSizeRef();
        auto& [TileSizeX, TileSizeY] = m_Tilemap->GetTileSizeRef();
        ImGui::Text(u8"타일 크기(A)");
        ImGui::SameLine(100);
        ImGui::SetNextItemWidth(120);
        if (ImGui::InputFloat("##AtTilesizeX", &AtlasTileSizeX, 0, 0, "%.1f", ImGuiInputTextFlags_EnterReturnsTrue))
        {
            m_Tilemap->SetAtlasTileSize(Vec2(AtlasTileSizeX, AtlasTileSizeY));
        }
        ImGui::SameLine();
        ImGui::SetNextItemWidth(120);
        if (ImGui::InputFloat("##AtTilesizeY", &AtlasTileSizeY, 0, 0, "%.1f", ImGuiInputTextFlags_EnterReturnsTrue))
        {
            m_Tilemap->SetAtlasTileSize(Vec2(AtlasTileSizeX, AtlasTileSizeY));
        }
        ImGui::SetItemTooltip(u8"아틀라스 텍스처에서 타일 크기");

        ImGui::Text(u8"타일 크기");
        ImGui::SameLine(100);
        ImGui::SetNextItemWidth(120);
        if (ImGui::InputFloat("##TileSizeX", &TileSizeX, 0, 0, "%.1f", ImGuiInputTextFlags_EnterReturnsTrue))
        {
            m_Tilemap->SetTileSize(Vec2(TileSizeX, TileSizeY));
        }
        ImGui::SameLine();
        ImGui::SetNextItemWidth(120);
        if (ImGui::InputFloat("##TilemapSizeX", &TileSizeY, 0, 0, "%.1f", ImGuiInputTextFlags_EnterReturnsTrue))
        {
            m_Tilemap->SetTileSize(Vec2(TileSizeX, TileSizeY));
        }
        ImGui::SetItemTooltip(u8"게임 화면 타일 크기");
        ImGui::End();

        // ============
        // Tilemap
        // ============

        int previewIdx = 0;
        float previewTileSize = 30;
        float previewPadding = 1.0f;
        m_ImagePos = ImGui::GetCursorPos();

        for (int i = 0; i < row; ++i)
        {
            for (int j = 0; j < col; ++j)
            {
                if (j > 0) ImGui::SameLine();

                ImGui::SetCursorPosX(m_ImagePos.x + j * (previewTileSize + previewPadding));
                ImGui::SetCursorPosY(m_ImagePos.y + i * (previewTileSize + previewPadding));

                int tileIdx = m_TilemapToBeEdited[previewIdx].ImgIdx;
                int tileX = tileIdx % maxAtlasCol;
                int tileY = tileIdx / maxAtlasCol;

                ImGui::Image(atlasTex->GetSRV().Get(),
                    ImVec2(previewTileSize, previewTileSize),
                    ImVec2(tileX * sliceUV.x, tileY * sliceUV.y),
                    ImVec2((tileX + 1) * sliceUV.x, (tileY + 1) * sliceUV.y),
                    ImVec4(1, 1, 1, 1), enableGrid ? ImVec4(0.22f, 0.22f, 0.22f, 0.77f) : ImVec4(0, 0, 0, 0));

                previewIdx += 1;
            }
        }

        auto [mX, mY] = ImGui::GetMousePos();
        auto [wX, wY] = ImGui::GetWindowPos();
        auto [wSX, wSY] = ImGui::GetWindowSize();

        if ((wX < mX && mX < wX + wSX) &&
            (wY < mY && mY < wY + wSY) &&
            ImGui::IsWindowFocused())
        {
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
            {
                ImVec2 pos = { mX - wX, mY - wY };
                float scrollX = ImGui::GetScrollX();
                float scrollY = ImGui::GetScrollY();

                int iCol = static_cast<int>((pos.x - m_ImagePos.x + scrollX) / (previewTileSize + previewPadding));
                int iRow = static_cast<int>((pos.y - m_ImagePos.y + scrollY) / (previewTileSize + previewPadding));

                //MD_ENGINE_TRACE("Mouse RelativePos {0} {1}", pos.x, pos.y);
                //MD_ENGINE_TRACE("ImagePos {0} {1}", m_ImagePos.x, m_ImagePos.y);
                //MD_ENGINE_TRACE("row col {0} {1}", iRow, iCol);

                if (iRow >= 0 && iRow < row && iCol >= 0 && iCol < col && ((maxAtlasRow * maxAtlasCol) >= selTileIndex))
                {
                    Edit(iRow * col + iCol, m_TilemapToBeEdited[iRow * col + iCol].ImgIdx, selTileIndex);
                }
            }
        }
	}
    else
    {
        ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, u8"선택된 타일맵 없음");

        if (ImGui::BeginDragDropTarget())
        {
            const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HierarchyViewTree");
            if (payload)
            {
                TreeNode** ppNode = (TreeNode**)payload->Data;
                TreeNode* pNode = *ppNode;

                CGameObject* pGameObject = (CGameObject*)pNode->GetData();
                if (pGameObject != nullptr)
                {
                    CTileMap* pTilemap = pGameObject->TileMap();
                    if (pTilemap != nullptr)
                    {
                        SetTilemap(pTilemap);
                    }
                }
            }

            ImGui::EndDragDropTarget();
        }
    }
}

void TilemapEditor::Activate()
{
    if (m_Tilemap != nullptr)
        m_RowCol = m_Tilemap->GetRowCol();
    else
        SetTransparent(false);
}

void TilemapEditor::SetTilemap(CTileMap* _Tilemap)
{//TODO
    m_Tilemap = _Tilemap;
    if (m_Tilemap != nullptr)
    {
        const vector<tTileInfo>& vecRef = m_Tilemap->GetTileInfoRef();
        m_TilemapToBeEdited.assign(vecRef.begin(), vecRef.end());
    }

    m_EditHistory.clear();
}

void TilemapEditor::Edit(UINT vecTileIdx, UINT oldImgIdx, UINT newImgIdx)
{
    m_EditHistory.push_back(std::make_pair(vecTileIdx, oldImgIdx));

    m_TilemapToBeEdited[vecTileIdx].ImgIdx = newImgIdx;
}

void TilemapEditor::Undo()
{
    std::pair<UINT, UINT> temp = m_EditHistory.back();
    m_TilemapToBeEdited[temp.first].ImgIdx = temp.second;
    m_EditHistory.pop_back();
}

void TilemapEditor::Revert()
{
    SetTilemap(m_Tilemap);
}

void TilemapEditor::Commit()
{
    m_Tilemap->SetTileInfo(m_TilemapToBeEdited);
    SetTilemap(m_Tilemap);
}