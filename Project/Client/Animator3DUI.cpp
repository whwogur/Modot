#include "pch.h"
#include "Animator3DUI.h"
#include <Engine/CGameObject.h>
#include <Engine/CAnimator3D.h>
#include "CEditorMgr.h"
Animator3DUI::Animator3DUI()
	: ComponentUI(COMPONENT_TYPE::ANIMATOR3D)
{
}

void Animator3DUI::Update()
{
	Title();
	if (!Collapsed())
	{
		
	}
}
