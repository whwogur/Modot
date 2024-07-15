#include "pch.h"
#include "Animator2DUI.h"
#include "CGameObject.h"
#include "CAnimator2D.h"
#include "CEditorMgr.h"
#include "Inspector.h"
#include "TreeUI.h"

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
		const vector<Ptr<CAnimation>>& vecAnim = GetTargetObject()->Animator2D()->GetAnimationsCRef();
		if (!vecAnim.empty())
		{
			const string combo_preview_value = "(" + std::to_string(vecAnim.capacity()) + ")";

			if (ImGui::BeginCombo("##AnimationListCombo", combo_preview_value.c_str()))
			{
				for (size_t i = 0; i < vecAnim.size(); ++i)
				{
					if (vecAnim[i].Get() == nullptr)
					{
						if (ImGui::Selectable("< Empty >"))
						{

						}
					}
					else
					{
						const wstring& animName = vecAnim[i].Get()->GetKey();
						string sName = ICON_FA_VIDEO_CAMERA + string(animName.begin(), animName.end());
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
	}
}
