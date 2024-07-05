#include "pch.h"
#include "CEditorMgr.h"

#include "Engine/CKeyMgr.h"
#include "Engine/CRenderMgr.h"

#include "CGameObjectEx.h"
#include "Engine/components.h"
#include "CEditorCameraScript.h"


#include "Engine/CEngine.h"
#include "Engine/CDevice.h"
#include "Engine/CKeyMgr.h"
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
			pUI->SetActive(false);
        else
			pUI->SetActive(true);
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
	style.WindowMinSize.x = 350.0f;
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	style.WindowMinSize.x = minWinSizeX;

	if (ImGui::BeginMenuBar())
	{
		Ptr<CTexture> LogoTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"Logo");
		ImGui::Image(LogoTex->GetSRV().Get(), {28 ,28});

		if (ImGui::BeginMenu(u8"파일"))
		{
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
				PostQuitMessage(0);
			}

			ImGui::EndMenu();
		}
		 
		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

		UINT FPS = CTimeMgr::GetInst()->GetFPSRecord();
		char buffer[255];
		sprintf_s(buffer, " FPS : %d", FPS);
		ImGui::SameLine(contentRegionAvailable.x);
		ImGui::Button(buffer);

		ImGui::EndMenuBar();
	}

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	
	ImGui::Begin("Viewport");
	// Resize
	ImVec2 temp = ImGui::GetContentRegionAvail();
	ID3D11ShaderResourceView* ViewportTexSRV = CRenderMgr::GetInst()->GetViewportSRV();
	
	ImGui::Image((void*)(ViewportTexSRV), temp);

	ImGui::End();
	ImGui::PopStyleVar();

	ImGui::End();

    for (const auto& pair : m_mapUI)
    {
        pair.second->Tick();
    }
}

void CEditorMgr::SetDarkThemeColors()
{
	ImGuiStyle* style = &ImGui::GetStyle();

	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 5.0f;
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 4.0f;
	style->ItemSpacing = ImVec2(12, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;

	// Tabs
	style->Colors[ImGuiCol_Tab] = ImVec4{ 0.0f, 0.0f, 0.0f, 1.0f };
	style->Colors[ImGuiCol_TabHovered] = ImVec4{ 0.098f, 0.102f, 0.111f, 1.0f };
	style->Colors[ImGuiCol_TabActive] = ImVec4{ 0.18f, 0.1805f, 0.581f, 1.0f };
	style->Colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.0f, 0.0f, 0.0f, 1.0f };
	style->Colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.0f, 0.0f, 0.0f, 1.0f };

	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.14f, 0.13f, 0.19f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
}
