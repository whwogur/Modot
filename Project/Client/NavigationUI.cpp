#include "pch.h"
#include "NavigationUI.h"
#include "CGameObject.h"
#include "CEditorMgr.h"

#include <Engine/CNavigation.h>
#include <Engine/CLandscape.h>
#include <Engine/CKeyMgr.h>
NavigationUI::NavigationUI()
	: ComponentUI(COMPONENT_TYPE::NAVIGATION)
{
}

void NavigationUI::Update()
{
	Title();
	if (!Collapsed())
	{
		CLandscape* pLandscape = GetTargetObject()->Landscape();
		if (nullptr != pLandscape)
		{
			if (pLandscape->GetMode() == LANDSCAPE_MODE::PINPOINT)
			{
				ImGui::Text(u8"셀 관리 활성화됨");
				if (ImGui::Button("TurnOFF##CellManipulation"))
				{
					pLandscape->SetMode(LANDSCAPE_MODE::NONE);
					pLandscape->SetEdit(false);
					pLandscape->SetBrushScale(0.2f, 0.2f);
					pLandscape->SetBrushIdx(0);
				}
				CheckIfClicked(pLandscape);
			}
			else
			{
				if (ImGui::Button("TurnON##CellManipulation"))
				{
					pLandscape->SetMode(LANDSCAPE_MODE::PINPOINT);
					pLandscape->SetEdit(true);
					pLandscape->SetBrushScale(0.01f, 0.01f);
					pLandscape->SetBrushIdx(2);
				}
			}
			
		}
		else
		{
			ImGui::TextColored({ 1, 0, 0, 1 }, u8"랜드스케이프 컴포넌트 없음");
		}
	}
}

void NavigationUI::CheckIfClicked(CLandscape* _Landscape)
{
	if (KEY_TAP(KEY::LBTN))
	{
		// Temp
		const tRaycastOut& rayInfo = _Landscape->GetRaycastInfo();
		string strInfo("X: " + std::to_string(static_cast<int>(rayInfo.Location.x)));
		strInfo += "Z: " + std::to_string(static_cast<int>(rayInfo.Location.z));
		EDITOR_TRACE(strInfo);
	}
}
