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

void Animator2DUI::Update()
{
	Title();
	if (!Collapsed())
	{
		CGameObject* targetObj = GetTargetObject();
		if (targetObj != nullptr)
		{
			const std::vector<Ptr<CAnimation>>& vecAnim = targetObj->Animator2D()->GetAnimationsRef();
			if (!vecAnim.empty())
			{
				const string combo_preview_value = "(" + std::to_string(vecAnim.size()) + ") Slots Available";

				ImGui::Text("Animation List");
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
				ImGui::SetItemTooltip(u8"애니메이션 에디터에서 보기");

				ImGui::Text("Current Animation");

				if (ImGui::BeginCombo("##CurAnimCombo", "Select Animation"))
				{
					for (const auto& anim : vecAnim)
					{
						const wstring& wstrKey = anim->GetKey();
						const string strKey(wstrKey.begin(), wstrKey.end());
						if (ImGui::Selectable(strKey.c_str()))
						{
							targetObj->Animator2D()->Play(wstrKey, 8.0f, true);
						}
					}
					ImGui::EndCombo();
				}
				ImGui::SetItemTooltip(u8"애니메이션 재생");


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

				ImGui::Text("Repeat Animation");
				ImGui::SameLine(100);
				bool& repeatRef = targetObj->Animator2D()->GetRepeatRef();
				ToggleButton("Repeat", &repeatRef);
			}
			else
			{
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
	}
	
}