#include "pch.h"
#include "SpriteUI.h"
#include "CSprite.h"

SpriteUI::SpriteUI()
	: AssetUI(ASSET_TYPE::SPRITE)
{
}

SpriteUI::~SpriteUI()
{
}

void SpriteUI::Update()
{
	Title();

	ImGui::NewLine();
	ImGui::SeparatorText(u8"Á¤º¸");
	ImGui::NewLine();

	Ptr<CSprite> pSprite = (CSprite*)GetAsset().Get();
	Vec2 LT = pSprite->GetLeftTopUV();
	Vec2 RB = pSprite->GetSliceUV();
	const Vec2& OffsetUV = pSprite->GetOffsetUVRef();
	ImGui::NewLine();
	ImGui::SameLine(95);
	ImGui::Image((ImTextureID)pSprite->GetAtlasTexture()->GetSRV().Get(), { 150, 150 }, { LT.x, LT.y }, { LT.x + RB.x, LT.y + RB.y }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 });
	ImGui::NewLine();
	ImGui::NewLine();
	
	ImGui::SameLine(60);
	ImGui::TextColored(HEADER_1, "LT");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(70.0f);
	ImGui::InputFloat("##LTx", &LT.x, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine(200);
	ImGui::SetNextItemWidth(70.0f);
	ImGui::InputFloat("##LTy", &LT.y, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_ReadOnly);

	ImGui::NewLine(); ImGui::SameLine(60);
	ImGui::TextColored(HEADER_1, "RB");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(70.0f);
	ImGui::InputFloat("##RBx", &RB.x, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine(200);
	ImGui::SetNextItemWidth(70.0f);
	ImGui::InputFloat("##RBy", &RB.y, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_ReadOnly);
}