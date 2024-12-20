#include "pch.h"
#include "NavigationUI.h"
#include "CGameObject.h"
#include "CEditorMgr.h"

#include <Engine/CNavigation.h>
#include <Engine/CLandscape.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CTransform.h>
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
					pLandscape->SetBrushScale(0.005f, 0.005f);
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
		string strInfo("X: " + std::to_string(rayInfo.WorldPos.x));
		strInfo += " Z: " + std::to_string(rayInfo.WorldPos.z);
		EDITOR_TRACE(strInfo);

		CGameObject* testObj = CLevelMgr::GetInst()->FindObjectByName(L"TestObj");
		if (testObj != nullptr)
		{
			testObj->Transform()->SetRelativePos({ rayInfo.WorldPos.x, rayInfo.WorldPos.y, rayInfo.WorldPos.z });
		}
	}
}
