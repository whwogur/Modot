#include "pch.h"
#include "MDViewport.h"
#include <Engine/CRenderMgr.h>
#include <Engine/CAssetMgr.h>
#include <ImGui/imgui.h>
#include <Engine/CKeyMgr.h>
#include "ImGui/ImGuizmo.h"
#include "Gizmo.h"
MDViewport::MDViewport()
{
}

void MDViewport::Init()
{
	m_RenderTarget = CAssetMgr::GetInst()->FindAsset<CTexture>(L"ViewportRT");
	MD_ENGINE_ASSERT(m_RenderTarget.Get(), L"뷰포트 초기화 실패");
	m_Gizmo = std::make_shared<Gizmo>();
	m_Gizmo->Init(CRenderMgr::GetInst()->GetEditorCamera());
}

void MDViewport::Tick()
{
	ImGui::Begin("Viewport");
	ShortCut();
	// Resize
	ImVec2 temp = ImGui::GetContentRegionAvail();

	const Vec2& ViewportTexSize = CRenderMgr::GetInst()->GetViewportSizeRef();

	if (temp.x > 0.0f && temp.y > 0.0f &&
		(ViewportTexSize.x != temp.x || ViewportTexSize.y != temp.y))
	{
		CRenderMgr::GetInst()->ResizeViewportTex(Vec2(temp.x, temp.y));
	}

	ImGui::Image(m_RenderTarget.Get()->GetSRV().Get(), { temp.x, temp.y });
	m_Gizmo->Update();

	ImGui::End();
}

void MDViewport::ShortCut()
{
	if (KEY_TAP(KEY::G))
	{
		m_Gizmo->ToggleActive();
	}
	if (KEY_TAP(KEY::Z))
	{
		m_Gizmo->SetGizmoType(ImGuizmo::OPERATION::TRANSLATE);
	}
	if (KEY_TAP(KEY::X))
	{
		m_Gizmo->SetGizmoType(ImGuizmo::OPERATION::ROTATE);
	}
	if (KEY_TAP(KEY::C))
	{
		m_Gizmo->SetGizmoType(ImGuizmo::OPERATION::SCALE);
	}
}

void MDViewport::SetTargetObject(CGameObject* Obj)
{
	m_Gizmo->SetTargetObject(Obj); //TODO
	m_Gizmo->SetGizmoType(ImGuizmo::OPERATION::TRANSLATE);
}

void MDViewport::SetGizmoType(int Type)
{
	m_Gizmo->SetGizmoType(Type);
}
