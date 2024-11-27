#include "pch.h"
#include "Animator3DUI.h"
#include <Engine/CGameObject.h>
#include <Engine/CAnimator3D.h>

Animator3DUI::Animator3DUI()
	: ComponentUI(COMPONENT_TYPE::ANIMATOR3D)
{
}

void Animator3DUI::Update()
{
	Title();
	if (!Collapsed())
	{
		CGameObject* targetObj = GetTargetObject();
		if (targetObj != nullptr)
		{
			UINT boneCnt = targetObj->Animator3D()->GetBoneCount();
			ImGui::Text(std::to_string(boneCnt).c_str());
		}
	}
}
