#include "pch.h"
#include "AnimationUI.h"
#include "CAnimation.h"
#include "CEditorMgr.h"
#include "AnimationEditor.h"
#include "CAssetMgr.h"
#include "TreeUI.h"
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
	string animName = string(anim->GetKey().begin(), anim->GetKey().end());
	strcpy_s(m_AnimationName, sizeof(m_AnimationName), animName.c_str());

	ImGui::SameLine(ImGui::GetContentRegionAvail().x - 100);
	if (ImGui::Button(ICON_FA_PENCIL, { 40, 25 }))
	{
		AnimationEditor* editor = static_cast<AnimationEditor*>(CEditorMgr::GetInst()->FindEditorUI("AnimationEditor"));
		editor->SetAnimation(anim);
		editor->Refresh();
		editor->SetActive(true);
	}
	ImGui::SetItemTooltip(u8"애니메이션 에디터를 엽니다");

	const std::vector<Ptr<CSprite>>& vecSprites = anim->GetSpritesCRef();

	string animFrameCount = std::to_string(anim->GetMaxFrameCount());

	const wstring& animWRelativePath = anim->GetRelativePath();
	string animRelativePath = animWRelativePath.length() < 1 ? "Engine Generated" : string(anim->GetRelativePath().begin(), anim->GetRelativePath().end());

	const auto& sprite = vecSprites[0];
	ImVec2 LeftTopUV = ImVec2(sprite->GetLeftTopUV().x, sprite->GetLeftTopUV().y);
	ImVec2 RightBottomUV = ImVec2(sprite->GetSliceUV().x + sprite->GetLeftTopUV().x, sprite->GetSliceUV().y + sprite->GetLeftTopUV().y);
	ImVec2 availRegion = ImGui::GetContentRegionAvail();
	ImGui::NewLine();
	ImGui::SameLine(availRegion.x / 2 - 72);
	ImGui::Image(sprite->GetAtlasTexture().Get()->GetSRV().Get(), { 144, 144 }, LeftTopUV, RightBottomUV, { 1,1,1,1 }, { 0.1f, 0.8f, 0.2f, 1.0f });

	ImGui::NewLine();
	ImGui::NewLine();
	ImGui::SameLine(60);
	ImGui::TextColored({0.2f, 0.56f, 0.77f, 1.0f}, u8"이름");
	ImGui::SameLine(125);
	ImGui::SetNextItemWidth(150.f);
	if (ImGui::InputText("##AnimKey", m_AnimationName, sizeof(m_AnimationName), ImGuiInputTextFlags_EnterReturnsTrue))
	{
		string strKey = m_AnimationName;
		wstring wstrKey(strKey.begin(), strKey.end());
		CAssetMgr::GetInst()->AddAsset<CAnimation>(wstrKey, anim);
		wstrKey += L".anim";
		anim->Save(L"animation\\" + wstrKey);
	}

	ImGui::NewLine();
	ImGui::SameLine(60);
	ImGui::TextColored({ 0.2f, 0.56f, 0.77f, 1.0f }, u8"프레임수");
	ImGui::SameLine(125);
	ImGui::SetNextItemWidth(150.f);
	ImGui::InputText("##AnimFrameCount", (char*)animFrameCount.c_str(), ImGuiInputTextFlags_ReadOnly);

	ImGui::NewLine();
	ImGui::SameLine(60);
	ImGui::TextColored({ 0.2f, 0.56f, 0.77f, 1.0f }, u8"경로");
	ImGui::SameLine(125);
	ImGui::SetNextItemWidth(150.f);
	ImGui::InputText("##AnimRelativePath", (char*)animRelativePath.c_str(), ImGuiInputTextFlags_ReadOnly);

	ImGui::NewLine();
	if (ImGui::BeginCombo("##SpriteListCombo", u8"스프라이트 목록"))
	{
		std::vector<Ptr<CSprite>>& spriteRef = anim->GetSpritesRef();
		for (int i = 0; i < spriteRef.size(); ++i)
		{
			ImGui::SetItemTooltip(u8"클릭해서 순서 변경");
			string key = string(spriteRef[i]->GetKey().begin(), spriteRef[i]->GetKey().end());
			static bool selected = false;
			ImGui::Selectable(key.c_str(), &selected, ImGuiSelectableFlags_DontClosePopups);

			if (selected)
			{
				if (m_SelectedIdx == -1)
				{
					m_SelectedIdx = i;
				}
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

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
		if (payload)
		{
			TreeNode** ppNode = (TreeNode**)payload->Data;
			TreeNode* pNode = *ppNode;

			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (ASSET_TYPE::SPRITE == pAsset->GetAssetType())
			{
				anim->AddSprite((CSprite*)pAsset.Get());
			}
		}

		ImGui::EndDragDropTarget();
	}
}
