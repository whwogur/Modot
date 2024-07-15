#include "pch.h"
#include "AnimationUI.h"
#include "CAnimation.h"
#include "CEditorMgr.h"
#include "AnimationEditor.h"
#include "CAssetMgr.h"
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

	CAnimation* anim = static_cast<CAnimation*>(GetAsset().Get());
	MD_ENGINE_ASSERT(anim, L"애니메이션 없이 AnimationUI 활성화됨");

	ImGui::SameLine(ImGui::GetContentRegionAvail().x - 100);
	if (ImGui::Button(ICON_FA_PENCIL, { 40, 25 }))
	{
		AnimationEditor* editor = static_cast<AnimationEditor*>(CEditorMgr::GetInst()->FindEditorUI("AnimationEditor"));
		editor->SetAnimation(anim);
		editor->Refresh();
		editor->SetActive(true);
	}

	const vector<Ptr<CSprite>>& vecSprites = anim->GetSpritesCRef();

	string animName = string(anim->GetKey().begin(), anim->GetKey().end());
	string animFrameCount = std::to_string(anim->GetMaxFrameCount());

	const wstring& animWRelativePath = anim->GetRelativePath();
	string animRelativePath = animWRelativePath.length() < 1 ? "Engine Generated" : string(anim->GetRelativePath().begin(), anim->GetRelativePath().end());

	const auto& sprite = vecSprites[1];
	ImVec2 LeftTopUV = ImVec2(sprite->GetLeftTopUV().x, sprite->GetLeftTopUV().y);
	ImVec2 RightBottomUV = ImVec2(sprite->GetSliceUV().x + sprite->GetLeftTopUV().x, sprite->GetSliceUV().y + sprite->GetLeftTopUV().y);

	ImGui::Image(sprite->GetAtlasTexture().Get()->GetSRV().Get(), { 144, 144 }, LeftTopUV, RightBottomUV, { 1,1,1,1 }, { 0.1, 0.8, 0.2, 1.0 });

	ImGui::Text(u8"이름");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(150.f);
	ImGui::InputText("##AnimKey", (char*)animName.c_str(), ImGuiInputTextFlags_ReadOnly);

	ImGui::Text(u8"프레임수");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(150.f);
	ImGui::InputText("##AnimFrameCount", (char*)animFrameCount.c_str(), ImGuiInputTextFlags_ReadOnly);

	ImGui::Text(u8"경로");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(150.f);
	ImGui::InputText("##AnimRelativePath", (char*)animRelativePath.c_str(), ImGuiInputTextFlags_ReadOnly);

	ImGui::NewLine();
	if (ImGui::BeginCombo("##SpriteListCombo", u8"스프라이트 목록"))
	{
		vector<Ptr<CSprite>>& spriteRef = anim->GetSpritesRef();
		for (int i = 0; i < spriteRef.size(); ++i)
		{
			string key = string(spriteRef[i]->GetKey().begin(), spriteRef[i]->GetKey().end());
			static bool selected = false;
			ImGui::Selectable(key.c_str(), &selected, ImGuiSelectableFlags_DontClosePopups);

			if (selected)
			{
				if (m_SelectedIdx == -1)
					m_SelectedIdx = i;
				else
				{
					std::iter_swap(spriteRef.begin() + m_SelectedIdx, spriteRef.begin() + i);
					m_SelectedIdx = -1;
				}
				selected = false;
			}
		}
		ImGui::EndCombo();
	}
}
