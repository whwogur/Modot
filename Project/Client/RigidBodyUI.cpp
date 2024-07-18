#include "pch.h"
#include "RigidBodyUI.h"
#include "CGameObject.h"
#include "CRigidBody.h"

RigidBodyUI::RigidBodyUI()
	: ComponentUI(COMPONENT_TYPE::RIGIDBODY)
{
}

void RigidBodyUI::Update()
{
	Title();

	CRigidBody* pRB = GetTargetObject()->RigidBody();
	if (pRB != nullptr)
	{
		float& mass = pRB->GetMassRef();
		float& friction = pRB->GetFrictionRef();
		float& fictionScale = pRB->GetFrictionScaleRef();
		float& velocityLimit = pRB->GetVelocityLimitRef();
		float& maxGravityAccel = pRB->GetMaxGravityAccelRef();

		ImGui::InputFloat("Mass", &mass, 0.1f, 0.5f, "%.1f");
		ImGui::InputFloat("Friction", &friction, 100.0f, 200.0f, "%.1f");
		ImGui::InputFloat("Friction Scale", &fictionScale, 0.5f, 2.f, "%.1f");
		ImGui::InputFloat("Veloctiy Limit", &velocityLimit, 10.0f, 50.0f, "%.1f");
		ImGui::InputFloat("Max Gravity Accel", &maxGravityAccel, 10.0f, 50.0f, "%.1f");
	}
}
