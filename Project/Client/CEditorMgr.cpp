#include "pch.h"
#include "CEditorMgr.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CEngine.h>
#include <Engine/CDevice.h>
#include <Engine/CPathMgr.h>

#include "CGameObjectEx.h"
#include "Engine/components.h"
#include "CEditorCameraScript.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "EditorUI.h"

#include "Inspector.h"
#include "EditorLogger.h"
#include "LevelEditor.h"
#include "ModelEditor.h"
CEditorMgr::CEditorMgr()
{
}

CEditorMgr::~CEditorMgr()
{
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

	// Content 폴더를 감시하는 커널 오브젝트 생성
	const wstring& ContentPath = CPathMgr::GetInst()->GetContentPath();
	m_Sentinel = FindFirstChangeNotification(ContentPath.c_str(), true
		, FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME
		| FILE_ACTION_ADDED | FILE_ACTION_REMOVED);
}

void CEditorMgr::Tick()
{
    ImGuiRun();
	ObserveContents();
}

void CEditorMgr::CreateEditorObject()
{
	m_Logger = std::make_unique<EditorLogger>();
	m_arrViewport[(UINT)VIEWPORT_TYPE::LEVEL] = std::make_shared<LevelEditor>();
	m_arrViewport[(UINT)VIEWPORT_TYPE::MODEL] = std::make_shared<ModelEditor>();

	for (UINT i = 0; i < (UINT)VIEWPORT_TYPE::END; ++i)
	{
		m_arrViewport[i]->Init();
	}

	m_Logger->LogAdapterInfo();
}

void CEditorMgr::SetTheme()
{
	// https://coolors.co/palette/75dddd-508991-172a3a-004346-09bc8a
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha						= 1.0f;
	style.DisabledAlpha				= 0.6000000238418579f;
	style.WindowPadding				= ImVec2(8.0f, 8.0f);
	style.WindowRounding			= 0.0f;
	style.WindowBorderSize			= 1.0f;
	style.WindowMinSize				= ImVec2(32.0f, 32.0f);
	style.WindowTitleAlign			= ImVec2(0.5f, 0.5f);
	style.WindowMenuButtonPosition	= ImGuiDir_Left;
	style.ChildRounding				= 0.0f;
	style.ChildBorderSize			= 1.0f;
	style.PopupRounding				= 0.0f;
	style.PopupBorderSize			= 1.0f;
	style.FramePadding				= ImVec2(4.0f, 3.0f);
	style.FrameRounding				= 0.0f;
	style.FrameBorderSize			= 0.0f;
	style.ItemSpacing				= ImVec2(8.0f, 4.0f);
	style.ItemInnerSpacing			= ImVec2(4.0f, 4.0f);
	style.CellPadding				= ImVec2(4.0f, 2.0f);
	style.IndentSpacing				= 21.0f;
	style.ColumnsMinSpacing			= 6.0f;
	style.ScrollbarSize				= 14.0f;
	style.ScrollbarRounding			= 0.0f;
	style.GrabMinSize				= 10.0f;
	style.GrabRounding				= 0.0f;
	style.TabRounding				= 0.0f;
	style.TabBorderSize				= 0.0f;
	style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition		= ImGuiDir_Right;
	style.ButtonTextAlign			= ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign		= ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text]						= ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled]				= ImVec4(0.4980392158031464f, 0.4980392158031464f, 0.4980392158031464f, 1.0f);
	style.Colors[ImGuiCol_WindowBg]					= ImVec4(0.03921568766236305f, 0.03921568766236305f, 0.03921568766236305f, 1.0f);
	style.Colors[ImGuiCol_ChildBg]					= ImVec4(0.05490196123719215f, 0.05490196123719215f, 0.05490196123719215f, 1.0f);
	style.Colors[ImGuiCol_PopupBg]					= ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_Border]					= ImVec4(0.0f, 0.2627451f, 0.2745098f, 1.0f);
	style.Colors[ImGuiCol_BorderShadow]				= ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_FrameBg]					= ImVec4(0.1176470592617989f, 0.1176470592617989f, 0.1176470592617989f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered]			= ImVec4(0.0f, 0.2627451f, 0.2745098f, 0.5647059f);
	style.Colors[ImGuiCol_FrameBgActive]			= ImVec4(0.0f, 0.2627451f, 0.2745098f, 0.5647059f);
	style.Colors[ImGuiCol_TitleBg]					= ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive]			= ImVec4(0.03921568766236305f, 0.03921568766236305f, 0.03921568766236305f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed]			= ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_MenuBarBg]				= ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.9411764740943909f);
	style.Colors[ImGuiCol_ScrollbarBg]				= ImVec4(0.0f, 0.2627451f, 0.2745098f, 0.5647059f);
	style.Colors[ImGuiCol_ScrollbarGrab]			= ImVec4(0.0f, 0.2627451f, 0.2745098f, 0.5019608f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered]		= ImVec4(0.0f, 0.2627451f, 0.2745098f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive]		= ImVec4(0.0f, 0.2627451f, 0.2745098f, 1.0f);
	style.Colors[ImGuiCol_CheckMark]				= ImVec4(0.0f, 0.2627451f, 0.2745098f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab]				= ImVec4(0.0f, 0.2627451f, 0.2745098f, 0.8154507f);
	style.Colors[ImGuiCol_SliderGrabActive]			= ImVec4(0.0f, 0.2627451f, 0.2745098f, 0.8156863f);
	style.Colors[ImGuiCol_Button]					= ImVec4(0.0f, 0.2627451f, 0.2745098f, 0.5019608f);
	style.Colors[ImGuiCol_ButtonHovered]			= ImVec4(0.0f, 0.2627451f, 0.2745098f, 0.7450981f);
	style.Colors[ImGuiCol_ButtonActive]				= ImVec4(0.0f, 0.2627451f, 0.2745098f, 1.0f);
	style.Colors[ImGuiCol_Header]					= ImVec4(0.0f, 0.2627451f, 0.2745098f, 0.6566523f);
	style.Colors[ImGuiCol_HeaderHovered]			= ImVec4(0.0f, 0.2627451f, 0.2745098f, 0.8039216f);
	style.Colors[ImGuiCol_HeaderActive]				= ImVec4(0.0f, 0.2627451f, 0.2745098f, 1.0f);
	style.Colors[ImGuiCol_Separator]				= ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.501960813999176f);
	style.Colors[ImGuiCol_SeparatorHovered]			= ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.6695278882980347f);
	style.Colors[ImGuiCol_SeparatorActive]			= ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.9570815563201904f);
	style.Colors[ImGuiCol_ResizeGrip]				= ImVec4(0.1019607856869698f, 0.1137254908680916f, 0.1294117718935013f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ResizeGripHovered]		= ImVec4(0.2039215713739395f, 0.2078431397676468f, 0.2156862765550613f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ResizeGripActive]			= ImVec4(0.3019607961177826f, 0.3019607961177826f, 0.3019607961177826f, 0.2000000029802322f);
	style.Colors[ImGuiCol_Tab]						= ImVec4(0.0f, 0.2627451f, 0.2745098f, 0.4392157f);
	style.Colors[ImGuiCol_TabHovered]				= ImVec4(0.0f, 0.2627451f, 0.2745098f, 1.0f);
	style.Colors[ImGuiCol_TabActive]				= ImVec4(0.0f, 0.2627451f, 0.2745098f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused]				= ImVec4(0.06666667014360428f, 0.06666667014360428f, 0.06666667014360428f, 0.9725490212440491f);
	style.Colors[ImGuiCol_TabUnfocusedActive]		= ImVec4(0.06666667014360428f, 0.06666667014360428f, 0.06666667014360428f, 1.0f);
	style.Colors[ImGuiCol_PlotLines]				= ImVec4(0.6078431606292725f, 0.6078431606292725f, 0.6078431606292725f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered]			= ImVec4(0.9490196108818054f, 0.3450980484485626f, 0.3450980484485626f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram]			= ImVec4(0.9490196108818054f, 0.3450980484485626f, 0.3450980484485626f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered]		= ImVec4(0.4274509847164154f, 0.3607843220233917f, 0.3607843220233917f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg]			= ImVec4(0.0f, 0.2627451f, 0.2745098f, 0.7124463f);
	style.Colors[ImGuiCol_TableBorderStrong]		= ImVec4(0.0f, 0.2627451f, 0.2745098f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight]			= ImVec4(0.0f, 0.2627451f, 0.2745098f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg]				= ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TableRowBgAlt]			= ImVec4(0.196078434586525f, 0.196078434586525f, 0.196078434586525f, 0.6274510025978088f);
	style.Colors[ImGuiCol_TextSelectedBg]			= ImVec4(0.0f, 0.2627451f, 0.2745098f, 1.0f);
	style.Colors[ImGuiCol_DragDropTarget]			= ImVec4(0.2588235437870026f, 0.2705882489681244f, 0.3803921639919281f, 1.0f);
	style.Colors[ImGuiCol_NavHighlight]				= ImVec4(0.1803921610116959f, 0.2274509817361832f, 0.2784313857555389f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight]	= ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
	style.Colors[ImGuiCol_NavWindowingDimBg]		= ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ModalWindowDimBg]			= ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);
	
}
