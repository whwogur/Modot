#include "pch.h"
#include "TilemapEditor.h"
#include "CAssetMgr.h"
#include "TreeUI.h"
TilemapEditor::TilemapEditor()
	: m_Tilemap(nullptr)
{
}

void TilemapEditor::Update()// 정리 필요..;
{
	if (m_Tilemap != nullptr)
	{
		Ptr<CTexture> atlasTex = m_Tilemap->GetAtlasTexture();
		Vec2 sliceUV = m_Tilemap->GetTileSliceUV();
		const wstring& atlasName = atlasTex->GetKey();
		string strKey(atlasName.begin(), atlasName.end());
		int row = m_Tilemap->GetRowCol().x;
		int col = m_Tilemap->GetRowCol().y;
        int maxAtlasRow = m_Tilemap->GetMaxAtlasRow();
        int maxAtlasCol = m_Tilemap->GetMaxAtlasCol();
        static int selTileIndex = 0;

		
        static bool bOpen = IsActive();
        static bool enableGrid = false;
        ImGui::Begin(ICON_FA_HAND_POINTER_O "##TileAtlasSelector", &bOpen, ImGuiWindowFlags_DockNodeHost);
        ImGui::NewLine();
        ImGui::TextColored(HEADER_1, "Atlas :");
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
        ImGui::Checkbox("Enable Grid", &enableGrid);
        if (!m_EditHistory.empty())
        {
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_REPLY, { 30, 30 }))
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
        int ButtonSize = 30;
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
                    MD_ENGINE_TRACE("{0}", selTileIndex);
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
        ImGui::End();


        int previewIdx = 0;
        int previewTileSize = 30;
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
                        m_Tilemap = pTilemap;
                    }
                }
            }

            ImGui::EndDragDropTarget();
        }
    }
}

void TilemapEditor::SetTilemap(CTileMap* _Tilemap)
{
    m_Tilemap = _Tilemap;
    const vector<tTileInfo>& vecRef = m_Tilemap->GetTileInfoRef();
    m_TilemapToBeEdited.assign(vecRef.begin(), vecRef.end());
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
