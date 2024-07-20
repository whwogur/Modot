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
		int row = m_Tilemap->GetRowCol().x;
		int col = m_Tilemap->GetRowCol().y;

		int Idx = 0;
		int ButtonSize = 20;
		m_ImagePos = ImGui::GetCursorPos();
		for (int i = 0; i < row; ++i)
		{
			ImGui::SameLine(ButtonSize);
			for (int j = 0; j < col; ++j)
			{
				/*ImGui::SameLine(j * (ButtonSize + 10) + 10);
				if (ImGui::ImageButton("##TilemapEditor", atlasTex->GetSRV().Get(),
					ImVec2(ButtonSize, ButtonSize), ImVec2(vecTile[Idx].ImgIdx * sliceUV.x, (vecTile[Idx].ImgIdx / row) * sliceUV.y),
					ImVec2((vecTile[Idx].ImgIdx + 1) * sliceUV.x, (vecTile[Idx].ImgIdx / row + 1) * sliceUV.y), { 0, 0, 0, 0 }, { 1, 1, 1, 1 }))
				{
					++vecTile[Idx].ImgIdx;
				}*/
				ImGui::SameLine(j * ButtonSize + 20);
				ImGui::Image(atlasTex->GetSRV().Get(),
					ImVec2(ButtonSize, ButtonSize), ImVec2(vecTile[Idx].ImgIdx * sliceUV.x, (vecTile[Idx].ImgIdx / row) * sliceUV.y),
					ImVec2((vecTile[Idx].ImgIdx + 1) * sliceUV.x, (vecTile[Idx].ImgIdx / row + 1) * sliceUV.y));
				//ImGui::SetItemTooltip(test.c_str());
				Idx += 1;
				//test = std::to_string(Idx);
			}
			ImGui::NewLine();
		}

		auto [mX, mY] = ImGui::GetMousePos();
		auto [wX, wY] = ImGui::GetWindowPos();
		auto [wSX, wSY] = ImGui::GetWindowSize();

		if ((wX < mX && (mX < wX + wSX)) &&
			(wY < mY && (mY < mY + wSY)) &&
			ImGui::IsWindowFocused())
		{
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			{
				ImVec2 pos = { mX - wX, mY - wY };

				int iRow = floor((pos.x - m_ImagePos.x - ButtonSize) / ButtonSize);
				int iCol = floor((pos.y - m_ImagePos.y) / ButtonSize);
				//MD_ENGINE_TRACE("Mouse RelativePos {0} {1}", pos.x, pos.y);
				//MD_ENGINE_TRACE("ImagePos {0} {1}", m_ImagePos.x, m_ImagePos.y);
				MD_ENGINE_TRACE("row col {0} {1}", iRow, iCol);

				vecTile[static_cast<std::vector<tTileInfo, std::allocator<tTileInfo>>::size_type>(iCol) * col + iRow].ImgIdx += 1;
			}
		}
	}
}
