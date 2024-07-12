#include "pch.h"
#include "TileMapUI.h"
#include "CGameObject.h"
#include "CTileMap.h"

#include "CEditorMgr.h"
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
	ImVec2 temp = ImGui::GetContentRegionAvail();
	ImGui::SameLine(temp.x - 30);
	ImFont* iconFont = CEditorMgr::GetInst()->GetIconFont();
	ImGui::PushFont(iconFont);

	if (ImGui::Button(ICON_FA_PLUS, { 25, 25 }))
	{
		EditorUI* pUI = CEditorMgr::GetInst()->FindEditorUI("List");

		if (pUI->IsActive())
			pUI->SetActive(false);
		else
			pUI->SetActive(true);
	}
	ImGui::PopFont();

	Ptr<CTexture> pTexture = GetTargetObject()->TileMap()->GetAtlasTexture();

	// 이미지	
	ImVec2 uv_min = ImVec2(0.0f, 0.0f);
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);

	ImVec4 border_col = ImVec4(0.4f, 0.8f, 0.45f, 1.0f);
	ImGui::Image(pTexture->GetSRV().Get(), ImVec2(200, 200), uv_min, uv_max, {1, 1, 1, 1}, border_col);

	// 텍스쳐 이름
	string Name = string(pTexture->GetKey().begin(), pTexture->GetKey().end());

	ImGui::Text(u8"아틀라스");
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
}