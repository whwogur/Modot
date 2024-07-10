#include "pch.h"
#include "AnimationUI.h"
#include "CAnimation.h"

AnimationUI::AnimationUI()
	: AssetUI(ASSET_TYPE::ANIMATION)
{
}

AnimationUI::~AnimationUI()
{
}

void AnimationUI::Update()
{
	Title();
	ImGui::SameLine(ImGui::GetContentRegionAvail().x - 100);
	ImGui::Button(u8"편집", { 80, 25 });

	CAnimation* anim = static_cast<CAnimation*>(GetAsset().Get());
	MD_ENGINE_ASSERT(anim, L"애니메이션 없이 AnimationUI 활성화됨");
	const vector<Ptr<CSprite>>& vecSprites = anim->GetSpritesCRef();

	for (const auto& sprite : vecSprites)
	{
		ImVec2 LeftTopUV = ImVec2(sprite->GetLeftTopUV().x, sprite->GetLeftTopUV().y);
		ImVec2 RightBottomUV = ImVec2(sprite->GetSliceUV().x + sprite->GetLeftTopUV().x, sprite->GetSliceUV().y + sprite->GetLeftTopUV().y);
		ImGui::Image(sprite->GetAtlasTexture().Get()->GetSRV().Get(), { 80, 80 }, LeftTopUV, RightBottomUV, { 1,1,1,1 }, { 0.1, 0.8, 0.2, 1.0 });
		ImGui::SameLine();
		string name = "name : " + string(sprite->GetKey().begin(), sprite->GetKey().end());
		
		ImGui::Text(name.c_str());
		if (sprite->GetOffsetUV() != Vec2())
		{
			ImGui::Text("OffsetUV : %.2f", sprite->GetOffsetUV().x, sprite->GetOffsetUV().y);
		}
	}
}
