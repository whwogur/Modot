#include "pch.h"

#include "ImGui/IconsFontAwesome4.h"
#include "CEditorMgr.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include <Engine/CDevice.h>

#include "CAssetMgr.h"
#include "CPathMgr.h"

#include "Inspector.h"
#include "ParamUI.h"
#include "MenuUI.h"
#include "ListUI.h"
#include "Content.h"
#include "HierarchyView.h"
#include "FileBrowser.h"
#include "AnimationEditor.h"
#include "SE_AtlasView.h"
#include "SE_Detail.h"
#include "SpriteEditor.h"
#include "TilemapEditor.h"
#include "CollisionCheck.h"
void CEditorMgr::InitImGui()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    wstring wcontentPath = CPathMgr::GetInst()->GetContentPath();
    string iconContentPath = string(wcontentPath.begin(), wcontentPath.end()) + "font\\fontawesome-webfont.ttf";
    string defaultContentPath = string(wcontentPath.begin(), wcontentPath.end()) + "font\\Galmuri9.ttf";
    io.Fonts->AddFontFromFileTTF(defaultContentPath.c_str(), 14.0f, NULL, io.Fonts->GetGlyphRangesKorean());
    
    ImFontConfig config;
    config.MergeMode = true;
    static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    m_IconFont = io.Fonts->AddFontFromFileTTF(iconContentPath.c_str(), 16.0f, &config, icon_ranges);
    
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    //io.ConfigViewportsNoDefaultParent = true;
    //io.ConfigDockingAlwaysTabBar = true;
    //io.ConfigDockingTransparentPayload = true;
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

    SetThemeUnrealEngine();
    //SetThemeFutureDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplWin32_Init(CEngine::GetInst()->GetMainWnd());
    ImGui_ImplDX11_Init(DEVICE, CONTEXT);

    CreateEditorUI();
}




void CEditorMgr::CreateEditorUI()
{
    EditorUI* pUI = nullptr;

    // Content
    pUI = new Content;
    pUI->Init();
    pUI->SetName("Content");
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    // HierarchyView
    pUI = new HierarchyView;
    pUI->SetName("HierarchyView");
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    // ListUI
    pUI = new ListUI;
    pUI->SetName("List");
    pUI->SetActive(false);
    pUI->SetModal(true);
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    // Inspector
    pUI = new Inspector;
    pUI->Init();
    pUI->SetName("Inspector");
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    // Menu
    pUI = new MenuUI;
    pUI->Init();
    pUI->SetName("MainMenu");
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    // FileBrowser
    pUI = new FileBrowser;
    pUI->Init();
    pUI->SetName("FileBrowser");
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    // AnimationEditor
    pUI = new AnimationEditor;
    pUI->SetName("AnimationEditor");
    pUI->SetActive(false);
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    // SE_AtlasView
    pUI = new SE_AtlasView;
    pUI->Init();
    pUI->SetName(ICON_FA_OBJECT_GROUP" AtlasView");
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    // SE_Detail
    pUI = new SE_Detail;
    pUI->Init();
    pUI->SetName(ICON_FA_TH_LIST" Detail");
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    // SpriteEditor
    pUI = new SpriteEditor;
    pUI->SetName("SpriteEditor");
    pUI->Init();
    pUI->SetActive(false);
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    // TilemapEditor
    pUI = new TilemapEditor;
    pUI->SetName("TilemapEditor");
    pUI->Init();
    pUI->SetActive(false);
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    pUI = new CollisionCheck;
    pUI->SetName(ICON_FA_CHECK_SQUARE_O" CollisionCheck");
    pUI->SetActive(false);
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));
}

void CEditorMgr::ObserveContents()
{
    // 지정된 상황이 발생했는지 확인
    DWORD dwStatus = WaitForSingleObject(m_Sentinel, 0);

    if (dwStatus == WAIT_OBJECT_0)
    {
        // Content 폴더에 있는 모든 에셋과 메모리에 로딩되어있는 에셋을 동기화
        Content* pContent = (Content*)FindEditorUI("Content");
        pContent->Reload();

        // 다시 대기
        FindNextChangeNotification(m_Sentinel);
    }
}


void CEditorMgr::ImGuiRun()
{
    // Start ImGui
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ParamUI::ResetID();

    ImGuiTick();

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}


EditorUI* CEditorMgr::FindEditorUI(const string& _Name)
{
    map<string, EditorUI*>::iterator iter = m_mapUI.find(_Name);

    if (iter == m_mapUI.end())
    {
        return nullptr;
    }

    return iter->second;
}