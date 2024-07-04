#include "pch.h"
#include "TransformUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>

TransformUI::TransformUI()
	: ComponentUI(COMPONENT_TYPE::TRANSFORM)
{
}

TransformUI::~TransformUI()
{
}

void TransformUI::Update()
{
	Title();

	CTransform* pTrans = GetTargetObject()->Transform();

	Vec3& vPos = pTrans->GetRelativePos();
	Vec3& vScale = pTrans->GetRelativeScale();
	Vec3& vRot = pTrans->GetRelativeRoatation();
	vRot = (vRot / XM_PI) * 180.f;

	DrawVec3Control("Position", vPos);
	DrawVec3Control("Scale", vScale);
	DrawVec3Control("Rotation", vRot);

	vRot = (vRot / 180.f) * XM_PI;

	ImGui::NewLine();
	// Independent Scale
	bool IS = pTrans->IsIndependentScale();

	ImGui::Text("Ignore Parent");
	ImGui::SameLine(100);
	if (ImGui::Checkbox("##TransIS", &IS))
	{
		pTrans->SetIndependentScale(IS);
	}
}