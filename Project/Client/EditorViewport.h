#pragma once
#include "ImGui/imgui.h"
#include "ImGui/ImGuizmo.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_internal.h"

enum class VIEWPORT_TYPE
{
	LEVEL, // Default
	MODEL,

	END
};
class CTexture;
class EditorViewport
{
public:
	EditorViewport() = default;
	virtual ~EditorViewport() = default;

public:
	virtual void Update() = 0;
	virtual void SetViewport(VIEWPORT_TYPE _Type) = 0;
	virtual void Init() {}
public:
	void SetTargetObject(CGameObject* _Target) { m_TargetObject = _Target; }
	CGameObject* GetTargetObject() { return m_TargetObject; }

protected:
	const Vec2& GetSize() { return m_Size; }
	void SetSize(const Vec2& _Size) { m_Size = _Size; }
	static void DrawLoadingAssetWindow();
	void DrawViewportTransitionButtons();
	void DrawRenderTarget();
private:
	CGameObject*			m_TargetObject = nullptr;
	Vec2					m_Size = {};
};

