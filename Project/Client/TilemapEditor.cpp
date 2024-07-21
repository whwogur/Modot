#include "pch.h"
#include "TilemapEditor.h"
#include "CTileMap.h"

TilemapEditor::TilemapEditor()
	: m_Tilemap(nullptr)
{
}

void TilemapEditor::Update()
{
	if (m_Tilemap != nullptr)
	{
		vector<tTileInfo>& vecTile = m_Tilemap->GetTileInfoRef();
		Ptr<CTexture> atlasTex = m_Tilemap->GetAtlasTexture();
		Vec2 sliceUV = m_Tilemap->GetTileSliceUV();
		const wstring& atlasName = atlasTex->GetKey();
		string strKey(atlasName.begin(), atlasName.end());
		int row = m_Tilemap->GetRowCol().x;
		int col = m_Tilemap->GetRowCol().y;
        int maxAtlasRowCol = m_Tilemap->GetMaxAtlasRowCol();
        static int selTileIndex = 0;

		ImGui::NewLine();
		ImGui::TextColored(HEADER_1, u8"현재 타일맵 아틀라스 : ");
		ImGui::SameLine();
		ImGui::Text(strKey.c_str());
		ImGui::NewLine();
		ImGui::TextColored(HEADER_1, u8"타일 인덱스 선택 : ");
		ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputInt("##SelTileIndex", &selTileIndex, 1, 1, ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);
		ImGui::Separator();
        ImGui::NewLine();
        ImGui::NewLine();
        




        int Idx = 0;
        int ButtonSize = 30;
        float Padding = 1.0f;
        m_ImagePos = ImGui::GetCursorPos();

        /*for (int i = 0; i < row; ++i)
        {
            for (int j = 0; j < col; ++j)
            {
                if (j > 0) ImGui::SameLine();

                ImGui::SetCursorPosX(m_ImagePos.x + j * (ButtonSize + Padding));
                ImGui::SetCursorPosY(m_ImagePos.y + i * (ButtonSize + Padding));

                if (ImGui::ImageButton("##TilemapEditor", atlasTex->GetSRV().Get(),
                    ImVec2(ButtonSize, ButtonSize),
                    ImVec2(vecTile[Idx].ImgIdx * sliceUV.x, (vecTile[Idx].ImgIdx / row) * sliceUV.y),
                    ImVec2((vecTile[Idx].ImgIdx + 1) * sliceUV.x, (vecTile[Idx].ImgIdx / row + 1) * sliceUV.y),
                    { 0, 0, 0, 0 }, { 1, 1, 1, 1 }))
                {
                    ++vecTile[Idx].ImgIdx;
                }

                Idx += 1;
            }
        }*/

        for (int i = 0; i < row; ++i)
        {
            for (int j = 0; j < col; ++j)
            {
                if (j > 0) ImGui::SameLine();

                ImGui::SetCursorPosX(m_ImagePos.x + j * (ButtonSize + Padding));
                ImGui::SetCursorPosY(m_ImagePos.y + i * (ButtonSize + Padding));

                ImGui::Image(atlasTex->GetSRV().Get(),
                    ImVec2(ButtonSize, ButtonSize),
                    ImVec2(vecTile[Idx].ImgIdx * sliceUV.x, (vecTile[Idx].ImgIdx / row) * sliceUV.y),
                    ImVec2((vecTile[Idx].ImgIdx + 1) * sliceUV.x, (vecTile[Idx].ImgIdx / row + 1) * sliceUV.y),
                    ImVec4(1, 1, 1, 1), ImVec4(0, 0, 0, 0));

                Idx += 1;
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

                int iCol = static_cast<int>((pos.x - m_ImagePos.x + scrollX) / (ButtonSize + Padding));
                int iRow = static_cast<int>((pos.y - m_ImagePos.y + scrollY) / (ButtonSize + Padding));

                //MD_ENGINE_TRACE("Mouse RelativePos {0} {1}", pos.x, pos.y);
                //MD_ENGINE_TRACE("ImagePos {0} {1}", m_ImagePos.x, m_ImagePos.y);
                //MD_ENGINE_TRACE("row col {0} {1}", iRow, iCol);

                if (iRow >= 0 && iRow < row && iCol >= 0 && iCol < col && (maxAtlasRowCol >= selTileIndex))
                {
                    vecTile[iRow * col + iCol].ImgIdx = selTileIndex;
                }
            }
        }
	}
}
