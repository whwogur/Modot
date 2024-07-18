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
	Ptr<CSprite> pSprite = (CSprite*)GetAsset().Get();
	Vec2 LT = pSprite->GetLeftTopUV();
	Vec2 RB = pSprite->GetSliceUV();
	const Vec2& OffsetUV = pSprite->GetOffsetUVRef();

	ImGui::Image(pSprite->GetAtlasTexture()->GetSRV().Get(), { 150, 150 }, { LT.x, LT.y }, { LT.x + RB.x, LT.y + RB.y }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 });

	ImGui::SetNextItemWidth(70.0f);
	ImGui::InputFloat("##LTx", &LT.x, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(70.0f);
	ImGui::InputFloat("##LTy", &LT.y, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_ReadOnly);
	ImGui::SetNextItemWidth(70.0f);
	ImGui::InputFloat("##RBx", &RB.x, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(70.0f);
	ImGui::InputFloat("##RBy", &RB.y, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_ReadOnly);
}