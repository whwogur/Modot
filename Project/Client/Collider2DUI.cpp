#include "pch.h"
#include "Collider2DUI.h"

#include "ModotHelpers.h"
#include <Engine/CCollider2D.h>
Collider2DUI::Collider2DUI()
	: ComponentUI(COMPONENT_TYPE::COLLIDER2D)
{
}

Collider2DUI::~Collider2DUI()
{
}

void Collider2DUI::Update()
{
	Title();
	if (!Collapsed())
	{
		CCollider2D* pCollider = GetTargetObject()->Collider2D();
		if (pCollider != nullptr)
		{
			Vec3& vOffset = pCollider->GetOffsetRef();
			Vec3& vScale = pCollider->GetScaleRef();
			bool& bIndependent = pCollider->GetIndependetScaleRef();

			Modot::ModotHelpers::DrawVec3Control("Offset", vOffset, 0.0f, 80.0f);
			Modot::ModotHelpers::DrawVec3Control("Scale", vScale, 0.0f, 80.0f);
			ImGui::Checkbox("Independent Scale", &bIndependent);
		}
	}
}