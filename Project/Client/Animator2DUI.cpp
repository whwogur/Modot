#include "pch.h"
#include "Animator2DUI.h"
#include "CGameObject.h"
#include "CAnimator2D.h"
#include "CEditorMgr.h"
#include "Inspector.h"
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
		const vector<Ptr<CAnimation>>& vecAnim = GetTargetObject()->Animator2D()->GetAnimationsCRef();
		if (!vecAnim.empty())
		{// �ִϸ��̼� ���� ������ ��� ����
			for (size_t i = 0; i < vecAnim.size(); ++i)
			{
				if (vecAnim[i].Get() == nullptr)
				{
					if (ImGui::Selectable(u8"- Empty Slot - [Ŭ���ؼ� �߰�]"))
					{
						// �ִϸ��̼� �߰�
					}
				}
				else
				{
					const wstring& animName = vecAnim[i].Get()->GetKey();
					string sName = "- " + string(animName.begin(), animName.end());
					if (ImGui::Selectable((char*)sName.c_str()))
					{
						// �ִϸ��̼� ����
						Inspector* inspector = static_cast<Inspector*>(CEditorMgr::GetInst()->FindEditorUI("Inspector"));
						inspector->SetTargetAsset(vecAnim[i].Get());
					}
				}
			}
		}
	}
}
