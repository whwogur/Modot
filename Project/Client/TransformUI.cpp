#include "pch.h"
#include "TransformUI.h"

#include "ModotHelpers.h"
#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>
#include <Engine/CBoundingSphere.h>

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
	if (!Collapsed())
	{
		// Independent Scale
		CTransform* pTrans = GetTargetObject()->Transform();
		bool IS = pTrans->IsIndependentScale();

		ImGui::Text("Ignore Parent");
		ImGui::SameLine(100);
		if (ImGui::Checkbox("##TransIS", &IS))
		{
			pTrans->SetIndependentScale(IS);
		}

		Vec3& vPos = pTrans->GetRelativePosRef();
		Vec3& vScale = pTrans->GetRelativeScaleRef();
		Vec3& vRot = pTrans->GetRelativeRotationRef();
		vRot = (vRot / XM_PI) * 180.f;

		Modot::ModotHelpers::DrawVec3Control("Position", vPos, 0.0f, 80.0f);
		Modot::ModotHelpers::DrawVec3Control("Scale", vScale, 1.0f, 80.0f);
		Modot::ModotHelpers::DrawVec3Control("Rotation", vRot, 0.0f, 80.0f);

		vRot = (vRot / 180.f) * XM_PI;

		bool bFrustumCull = pTrans->GetFrustumCulling();

		if (ImGui::Checkbox("Frustum Culling", &bFrustumCull))
		{
			pTrans->SetFrustumCulling(bFrustumCull);
		}

		if (bFrustumCull)
		{
			std::shared_ptr<CBoundingSphere>& pBoundingSphere = pTrans->GetBoundingSphere();

			if (pBoundingSphere != nullptr)
			{
				float fRadius = pBoundingSphere->GetRadius();
				if (ImGui::DragFloat("Radius", &fRadius, 1.f, 0.f, 0.f, "%.1f"))
				{
					pBoundingSphere->SetRadius(fRadius);
				}
			}
		}
		
	}	
}