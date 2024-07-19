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
		for (int i = 0; i < row; ++i)
		{
			ImGui::SameLine(0);
			for (int j = 0; j < col; ++j)
			{
				ImGui::SameLine(j * (ButtonSize + 10) + 10);
				if (ImGui::ImageButton("##TilemapEditor", atlasTex->GetSRV().Get(),
					ImVec2(ButtonSize, ButtonSize), ImVec2(vecTile[Idx].ImgIdx * sliceUV.x, (vecTile[Idx].ImgIdx / row) * sliceUV.y),
					ImVec2((vecTile[Idx].ImgIdx + 1) * sliceUV.x, (vecTile[Idx].ImgIdx / row + 1) * sliceUV.y), { 0, 0, 0, 0 }, { 1, 1, 1, 1 }))
				{
					++vecTile[Idx].ImgIdx;
				}
				Idx += 1;
			}
			ImGui::NewLine();
		}
	}
}
