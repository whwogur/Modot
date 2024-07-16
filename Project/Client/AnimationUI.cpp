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
	MD_ENGINE_ASSERT(anim, L"�ִϸ��̼� ���� AnimationUI Ȱ��ȭ��");

	ImGui::SameLine(ImGui::GetContentRegionAvail().x - 100);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.341f, 0.0f, 1.0f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.44f, 0.2f, 1.0f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.0f, 0.8f, 1.0f });
	if (ImGui::Button(ICON_FA_PENCIL, { 40, 25 }))
	{
		AnimationEditor* editor = static_cast<AnimationEditor*>(CEditorMgr::GetInst()->FindEditorUI("AnimationEditor"));
		editor->SetAnimation(anim);
		editor->Refresh();
		editor->SetActive(true);
	}
	ImGui::PopStyleColor(3);
	const vector<Ptr<CSprite>>& vecSprites = anim->GetSpritesCRef();

	string animName = string(anim->GetKey().begin(), anim->GetKey().end());
	string animFrameCount = std::to_string(anim->GetMaxFrameCount());

	const wstring& animWRelativePath = anim->GetRelativePath();
	string animRelativePath = animWRelativePath.length() < 1 ? "Engine Generated" : string(anim->GetRelativePath().begin(), anim->GetRelativePath().end());

	const auto& sprite = vecSprites[1];
	ImVec2 LeftTopUV = ImVec2(sprite->GetLeftTopUV().x, sprite->GetLeftTopUV().y);
	ImVec2 RightBottomUV = ImVec2(sprite->GetSliceUV().x + sprite->GetLeftTopUV().x, sprite->GetSliceUV().y + sprite->GetLeftTopUV().y);
	ImVec2 availRegion = ImGui::GetContentRegionAvail();
	ImGui::NewLine();
	ImGui::SameLine(availRegion.x / 2 - 72);
	ImGui::Image(sprite->GetAtlasTexture().Get()->GetSRV().Get(), { 144, 144 }, LeftTopUV, RightBottomUV, { 1,1,1,1 }, { 0.1, 0.8, 0.2, 1.0 });

	ImGui::NewLine();
	ImGui::NewLine();
	ImGui::SameLine(60);
	ImGui::TextColored({0.2f, 0.56f, 0.77f, 1.0f}, u8"�̸�");
	ImGui::SameLine(125);
	ImGui::SetNextItemWidth(150.f);
	ImGui::InputText("##AnimKey", (char*)animName.c_str(), ImGuiInputTextFlags_ReadOnly);

	ImGui::NewLine();
	ImGui::SameLine(60);
	ImGui::TextColored({ 0.2f, 0.56f, 0.77f, 1.0f }, u8"�����Ӽ�");
	ImGui::SameLine(125);
	ImGui::SetNextItemWidth(150.f);
	ImGui::InputText("##AnimFrameCount", (char*)animFrameCount.c_str(), ImGuiInputTextFlags_ReadOnly);

	ImGui::NewLine();
	ImGui::SameLine(60);
	ImGui::TextColored({ 0.2f, 0.56f, 0.77f, 1.0f }, u8"���");
	ImGui::SameLine(125);
	ImGui::SetNextItemWidth(150.f);
	ImGui::InputText("##AnimRelativePath", (char*)animRelativePath.c_str(), ImGuiInputTextFlags_ReadOnly);

	ImGui::NewLine();
	if (ImGui::BeginCombo("##SpriteListCombo", u8"��������Ʈ ���"))
	{
		vector<Ptr<CSprite>>& spriteRef = anim->GetSpritesRef();
		for (int i = 0; i < spriteRef.size(); ++i)
		{
			ImGui::SetItemTooltip(u8"Ŭ���ؼ� ���� ����");
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
