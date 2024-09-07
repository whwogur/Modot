#include "pch.h"
#include "Gizmo.h"

#include "CDevice.h"
#include "ImGui/imgui.h"
#include "ImGui/ImGuizmo.h"
#include "CCamera.h"
#include <Engine/CTransform.h>
#include <Engine/CRenderMgr.h>
#include "CKeyMgr.h"

Gizmo::Gizmo()
{
}


void Gizmo::Init(CCamera* _Cam)
{
	m_EditorCamera = _Cam;
}

void Gizmo::Update()
{
	if (!m_Active)
		return;

	ImGui::Begin("##GizmoControl", &m_Active, ImGuiWindowFlags_NoTitleBar);

	const Vec2& viewportSizeRef = CRenderMgr::GetInst()->GetViewportSizeRef();
	m_Resolution = viewportSizeRef;
	if (ImGui::Button(ICON_FA_ARROWS))
	{
		m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
	}
	ImGui::SameLine();
	if (ImGui::Button(ICON_FA_EXPAND))
	{
		m_GizmoType = ImGuizmo::OPERATION::SCALE;
	}
	ImGui::SameLine();
	if (ImGui::Button(ICON_FA_REFRESH))
	{
		m_GizmoType = ImGuizmo::OPERATION::ROTATE;
	}
	ImGui::End();

	if (m_TargetObject && m_GizmoType != -1)
	{
		MD_ENGINE_ASSERT(m_EditorCamera != nullptr, L"기즈모 업데이트 호출 시점 카메라 없음");

		ImGuizmo::SetOrthographic(true);

		ImGuizmo::BeginFrame();
		ImGuizmo::SetDrawlist(ImGui::GetWindowDrawList());
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowHeight(), ImGui::GetWindowWidth());

		// Editor Camera
		DirectX::XMFLOAT4X4 cameraProjection = m_EditorCamera->GetcamProjRef();
		DirectX::XMFLOAT4X4 cameraView = m_EditorCamera->GetcamViewRef();

		CTransform* tc = m_TargetObject->Transform();

		DirectX::XMFLOAT4X4 transform{};
		ImGuizmo::RecomposeMatrixFromComponents(&tc->GetRelativePosRef().x, &tc->GetRelativeRoatationRef().x, &tc->GetRelativeScaleRef().x, *transform.m);

		// Snapping
		bool snap = KEY_PRESSED(KEY::CTRL);

		float snapValue = 0.5f;
		if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
			snapValue = 45.0f;

		float snapValues[3] = { snapValue, snapValue, snapValue };

		ImGuizmo::Manipulate(*cameraView.m, *cameraProjection.m, (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, *transform.m, nullptr, snap ? snapValues : nullptr);

		if (ImGuizmo::IsUsing())
		{
			ImGuizmo::DecomposeMatrixToComponents(*transform.m, &tc->GetRelativePosRef().x, &tc->GetRelativeRoatationRef().x, &tc->GetRelativeScaleRef().x);
		}
	}
}
