#include "pch.h"
#include "CEditorMgr.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CRenderMgr.h>

#include "CGameObjectEx.h"
#include <Engine/components.h>
#include "CEditorCameraScript.h"


#include <Engine/CEngine.h>
#include <Engine/CDevice.h>
#include <Engine/CKeyMgr.h>
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "EditorUI.h"
CEditorMgr::CEditorMgr()
{
}

CEditorMgr::~CEditorMgr()
{
	Delete_Vec(m_vecEditorObject);
    Delete_Map(m_mapUI);
    // ImGui Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void CEditorMgr::Init()
{
	CreateEditorObject();
	InitImGui();
}

void CEditorMgr::Tick()
{
    ShortCut();

    EditorObjectUpdate();

    ImGuiRun();
}

void CEditorMgr::ShortCut()
{
    if (KEY_TAP(KEY::I))
    {
        EditorUI* pUI = FindEditorUI("Inspector");

        if (pUI->IsActive())
            pUI->Deactivate();
        else
            pUI->Activate();
    }
}

void CEditorMgr::EditorObjectUpdate()
{
    for (size_t i = 0; i < m_vecEditorObject.size(); ++i)
    {
        m_vecEditorObject[i]->Tick();
    }

    for (size_t i = 0; i < m_vecEditorObject.size(); ++i)
    {
        m_vecEditorObject[i]->FinalTick();
    }
}

void CEditorMgr::ImGuiTick()
{
	// Note: Switch this to true to enable dockspace
	static bool dockspaceOpen = true;
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
	float minWinSizeX = style.WindowMinSize.x;
	style.WindowMinSize.x = 370.0f;
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	style.WindowMinSize.x = minWinSizeX;

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows, 
			// which we can't undo at the moment without finer window depth/z control.
			//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);
			if (ImGui::MenuItem(u8"새 파일", "Ctrl + N"))
			{
			}

			if (ImGui::MenuItem(u8"열기", "Ctrl + O"))
			{
			}

			if (ImGui::MenuItem(u8"다른 이름으로 저장", "Ctrl + Shift + S"))
			{
			}

			if (ImGui::MenuItem(u8"닫기"))
			{

			}

			ImGui::EndMenu();
		}
		 
		ImGui::EndMenuBar();
	}

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	
	ImGui::Begin("Viewport");
	// Resize
	ImVec2 temp = ImGui::GetContentRegionAvail();
	
	Vec2 ViewportTexSize = CRenderMgr::GetInst()->GetViewportTexSize();

	if (temp.x > 0.0f && temp.y > 0.0f &&
		(ViewportTexSize.x != temp.x || ViewportTexSize.y != temp.y))
	{
		CRenderMgr::GetInst()->ResizeViewportTex(Vec2(temp.x, temp.y));
	}

	ID3D11ShaderResourceView* ViewportTexSRV = CRenderMgr::GetInst()->GetViewportSRV();
	
	ImGui::Image((void*)(ViewportTexSRV), CRenderMgr::GetInst()->GetViewportTexSize());

	ImGui::End();
	ImGui::PopStyleVar();

	ImGui::End();

    /*for (const auto& pair : m_mapUI)
    {
        pair.second->Tick();
    }*/
}

void CEditorMgr::SetDarkThemeColors()
{
    auto& colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

    // Headers
    colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Buttons
    colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Frame BG
    colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
    colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
    colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
}
