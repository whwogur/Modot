#include "pch.h"
#include "NavigationUI.h"
#include "CGameObject.h"
#include "CNavigation.h"

NavigationUI::NavigationUI()
	: ComponentUI(COMPONENT_TYPE::NAVIGATION)
{
}

void NavigationUI::Update()
{
	Title();
	if (!Collapsed())
	{
		CNavigation* pRB = GetTargetObject()->Navigation();
		if (pRB != nullptr)
		{
			float& mass = pRB->GetMassRef();
			float& friction = pRB->GetFrictionRef();
			float& fictionScale = pRB->GetFrictionScaleRef();
			float& velocityLimit = pRB->GetVelocityLimitRef();
			float& maxGravityAccel = pRB->GetMaxGravityAccelRef();
			bool isGround = pRB->IsGround();
			ImGui::InputFloat("Mass", &mass, 0.1f, 0.5f, "%.1f");
			ImGui::InputFloat("Friction", &friction, 100.0f, 200.0f, "%.1f");
			ImGui::InputFloat("Friction Scale", &fictionScale, 0.5f, 2.f, "%.1f");
			ImGui::InputFloat("Veloctiy Limit", &velocityLimit, 10.0f, 50.0f, "%.1f");
			ImGui::InputFloat("Max Gravity Accel", &maxGravityAccel, 10.0f, 50.0f, "%.1f");
		}
	}
}
