#include "pch.h"
#include "AnimationUI.h"
#include "CAnimation.h"
#include "CEditorMgr.h"
#include "Animation2DEditor.h"
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
	string animName = string(anim->GetKey().begin(), anim->GetKey().end());
	strcpy_s(m_AnimationName, sizeof(m_AnimationName), animName.c_str());

	ImGui::SameLine(ImGui::GetContentRegionAvail().x - 100);
	if (ImGui::Button(ICON_FA_PENCIL, { 40, 25 }))
	{
		Animation2DEditor* editor = static_cast<Animation2DEditor*>(CEditorMgr::GetInst()->FindEditorUI("Animation2DEditor"));
		editor->SetAnimation(anim);
		editor->Refresh();
		editor->SetActive(true);
	}
	ImGui::SetItemTooltip(u8"�ִϸ��̼� �����͸� ���ϴ�");

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
	ImGui::Image((ImTextureID)sprite->GetAtlasTexture().Get()->GetSRV().Get(), { 144, 144 }, LeftTopUV, RightBottomUV, { 1,1,1,1 }, { 0.1f, 0.8f, 0.2f, 1.0f });

	ImGui::NewLine();
	ImGui::NewLine();
	ImGui::SameLine(60);
	ImGui::TextColored({0.2f, 0.56f, 0.77f, 1.0f}, u8"�̸�");
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
		std::vector<Ptr<CSprite>>& spriteRef = anim->GetSpritesRef();
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
