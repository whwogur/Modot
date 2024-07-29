#include "pch.h"
#include "TileMapUI.h"
#include "CGameObject.h"
#include "CTileMap.h"

#include "CEditorMgr.h"
#include "TilemapEditor.h"
#include "TreeUI.h"
TileMapUI::TileMapUI()
	: ComponentUI(COMPONENT_TYPE::TILEMAP)
{
}

TileMapUI::~TileMapUI()
{
}

void TileMapUI::Update()
{
	Title();
	CTileMap* pTileMap = GetTargetObject()->TileMap();
	ImVec2 temp = ImGui::GetContentRegionAvail();
	ImFont* iconFont = CEditorMgr::GetInst()->GetIconFont();
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
		if (payload)
		{
			TreeNode** ppNode = (TreeNode**)payload->Data;
			TreeNode* pNode = *ppNode;

			Ptr<CTexture> pTexture = (CTexture*)pNode->GetData();
			if (pTexture != nullptr)
			{
				pTileMap->SetAtlasTexture(pTexture);
			}
		}

		ImGui::EndDragDropTarget();
	}
	
	ImGui::PushFont(iconFont);
	ImGui::SameLine(temp.x - 30);
	if (ImGui::Button(ICON_FA_PENCIL, { 28, 28 }))
	{
		EditorUI* pUI = CEditorMgr::GetInst()->FindEditorUI("TilemapEditor");

		((TilemapEditor*)pUI)->SetTilemap(pTileMap);

		if (pUI->IsActive())
			pUI->SetActive(false);
		else
			pUI->SetActive(true);
	}
	ImGui::PopFont();
	
	Ptr<CTexture> pTexture = pTileMap->GetAtlasTexture();
	if (pTexture != nullptr)
	{
		// 이미지	
		ImVec2 uv_min = ImVec2(0.0f, 0.0f);
		ImVec2 uv_max = ImVec2(1.0f, 1.0f);

		ImVec4 border_col = ImVec4(0.4f, 0.8f, 0.45f, 1.0f);
		ImGui::NewLine();
		ImGui::SameLine(95);
		ImGui::Image(pTexture->GetSRV().Get(), ImVec2(200, 200), uv_min, uv_max, { 1, 1, 1, 1 }, border_col);

		// 텍스쳐 이름
		string Name = string(pTexture->GetKey().begin(), pTexture->GetKey().end());

		ImGui::TextColored(HEADER_1, u8"아틀라스 정보");

		ImGui::Text(u8"이름");
		ImGui::SameLine(100);
		ImGui::InputText("##AssetName", (char*)Name.c_str(), Name.length(), ImGuiInputTextFlags_ReadOnly);

		// 해상도
		UINT width = pTexture->Width();
		UINT height = pTexture->Height();

		char buff[50] = {};
		sprintf_s(buff, "%d", width);

		ImGui::Text(u8"너비");
		ImGui::SameLine(100);
		ImGui::InputText("##TextureWidth", buff, 50, ImGuiInputTextFlags_ReadOnly);

		sprintf_s(buff, "%d", height);
		ImGui::Text(u8"높이");
		ImGui::SameLine(100);
		ImGui::InputText("##TextureHeight", buff, 50, ImGuiInputTextFlags_ReadOnly);

		auto& [AtlasTileSizeX, AtlasTileSizeY] = pTileMap->GetAtlasTileSizeRef();
		auto& [TileSizeX, TileSizeY] = pTileMap->GetTileSizeRef();
		int& Row = pTileMap->GetRowRef();
		int& Col = pTileMap->GetColRef();

		ImGui::TextColored(HEADER_1, u8"타일맵 정보");

		ImGui::Text(u8"타일 크기(A)");
		ImGui::SameLine(100);
		ImGui::SetNextItemWidth(120);
		if (ImGui::InputFloat("##AtTilesizeX", &AtlasTileSizeX, 0, 0, "%.1f", ImGuiInputTextFlags_EnterReturnsTrue))
		{
			pTileMap->SetAtlasTileSize(Vec2(AtlasTileSizeX, AtlasTileSizeY));
		}
		ImGui::SameLine();
		ImGui::SetNextItemWidth(120);
		if (ImGui::InputFloat("##AtTilesizeY", &AtlasTileSizeY, 0, 0, "%.1f", ImGuiInputTextFlags_EnterReturnsTrue))
		{
			pTileMap->SetAtlasTileSize(Vec2(AtlasTileSizeX, AtlasTileSizeY));
		}
		ImGui::SetItemTooltip(u8"아틀라스 텍스처에서 타일 크기");

		ImGui::Text(u8"타일 크기");
		ImGui::SameLine(100);
		ImGui::SetNextItemWidth(120);
		if (ImGui::InputFloat("##TileSizeX", &TileSizeX, 0, 0, "%.1f", ImGuiInputTextFlags_EnterReturnsTrue))
		{
			pTileMap->SetTileSize(Vec2(TileSizeX, TileSizeY));
		}
		ImGui::SameLine();
		ImGui::SetNextItemWidth(120);
		if (ImGui::InputFloat("##TilemapSizeX", &TileSizeY, 0, 0, "%.1f", ImGuiInputTextFlags_EnterReturnsTrue))
		{
			pTileMap->SetTileSize(Vec2(TileSizeX, TileSizeY));
		}
		ImGui::SetItemTooltip(u8"게임 화면 타일 크기");

		ImGui::Text(u8"타일맵 행/열");
		ImGui::SameLine(100);
		ImGui::SetNextItemWidth(120);
		if (ImGui::InputInt("##TilemapRow", &Row, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			pTileMap->SetRowCol(Row, Col);
		}
		ImGui::SameLine();
		ImGui::SetNextItemWidth(120);
		if (ImGui::InputInt("##TilemapCol", &Col, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			pTileMap->SetRowCol(Row, Col);
		}
	}
	
}