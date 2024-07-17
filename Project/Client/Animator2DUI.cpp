#include "pch.h"
#include "Animator2DUI.h"
#include "CGameObject.h"
#include "CAnimator2D.h"
#include "CEditorMgr.h"
#include "Inspector.h"
#include "TreeUI.h"
#include "CTaskMgr.h"
#include "CAssetMgr.h"
Animator2DUI::Animator2DUI()
	: ComponentUI(COMPONENT_TYPE::ANIMATOR2D)
{
}

Animator2DUI::~Animator2DUI()
{
}

void Animator2DUI::Update()
{
	Title();
	if (GetTargetObject() != nullptr)
	{
		ImFont* iconFont = CEditorMgr::GetInst()->GetIconFont();
		ImGui::PushFont(iconFont);
		const vector<Ptr<CAnimation>>& vecAnim = GetTargetObject()->Animator2D()->GetAnimationsRef();
		if (!vecAnim.empty())
		{
			const string combo_preview_value = "(" + std::to_string(vecAnim.size()) + ") Slots Available";

			if (ImGui::BeginCombo("##AnimationListCombo", combo_preview_value.c_str()))
			{
				for (size_t i = 0; i < vecAnim.size(); ++i)
				{
					if (vecAnim[i].Get() == nullptr)
					{
						ImGui::Selectable("< Empty >", false, ImGuiSelectableFlags_Disabled);
					}
					else
					{
						const wstring& animName = vecAnim[i].Get()->GetKey();
						string sName = ICON_FA_VIDEO_CAMERA " " + string(animName.begin(), animName.end());
						if (ImGui::Selectable((char*)sName.c_str()))
						{
							// 애니메이션 정보
							Inspector* inspector = static_cast<Inspector*>(CEditorMgr::GetInst()->FindEditorUI("Inspector"));
							inspector->SetTargetAsset(vecAnim[i].Get());
						}
					}

				}
				ImGui::EndCombo();
			}
		}
		ImGui::PopFont();

		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
			if (payload)
			{
				TreeNode** ppNode = (TreeNode**)payload->Data;
				TreeNode* pNode = *ppNode;

				Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
				if (ASSET_TYPE::ANIMATION == pAsset->GetAssetType())
				{
					GetTargetObject()->Animator2D()->PushBackAnimation((CAnimation*)pAsset.Get());
				}
			}

			ImGui::EndDragDropTarget();
		}
	}
}
