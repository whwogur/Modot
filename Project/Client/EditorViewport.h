#pragma once
#include "ImGui/imgui.h"
#include "ImGui/ImGuizmo.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_internal.h"

class EditorViewport
{
public:
	EditorViewport() = default;
	virtual ~EditorViewport() = default;

public:
	virtual void Update() = 0;
	virtual void LateUpdate() = 0;
	virtual void Init() {}
public:
	void SetTargetObject(CGameObject* _Target) { m_TargetObject = _Target; }

protected:
	CGameObject* GetTargetObject() { return m_TargetObject; }
	const Vec2& GetSize() { return m_Size; }
	void SetSize(const Vec2& _Size) { m_Size = _Size; }
private:
	CGameObject*	m_TargetObject = nullptr;
	Vec2			m_Size = {};
};

