#include "pch.h"

#include "ImGui/IconsFontAwesome4.h"
#include "CEditorMgr.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

#include <Engine/CDevice.h>
#include <Engine/CRenderMgr.h>

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
#include <ImGui/imgui_internal.h>
#include <ImGui/ImGuizmo.h>
#include <TreeUI.h>
void CEditorMgr::InitImGui()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    const wstring& wcontentPath = CPathMgr::GetInst()->GetContentPath();
    string iconContentPath = string(wcontentPath.begin(), wcontentPath.end()) + "font\\fontawesome-webfont.ttf";
    string defaultContentPath = string(wcontentPath.begin(), wcontentPath.end()) + "font\\LINESeedKR-Bd.ttf";
    io.Fonts->AddFontFromFileTTF(defaultContentPath.c_str(), 14.0f, NULL, io.Fonts->GetGlyphRangesKorean());
    
    ImFontConfig config;
    config.MergeMode = true;
    static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    io.Fonts->AddFontFromFileTTF(iconContentPath.c_str(), 12.0f, &config, icon_ranges);
    io.Fonts->AddFontFromFileTTF(iconContentPath.c_str(), 12.0f, NULL, io.Fonts->GetGlyphRangesKorean());
    
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows

    //SetThemeMicrosoft();
    //SetThemeMoonlight();
    //SetThemeUnrealEngine();
    SetThemeFutureDark();

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
    pUI->SetTransparent(true);
    pUI->SetDockable(false);
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

void CEditorMgr::RenderViewport()
{
    ImGuiWindowClass window_class;
    window_class.ClassId = ImGui::GetID("Editor Viewport");
    window_class.DockNodeFlagsOverrideSet = 0;
    window_class.DockingAllowUnclassed = true;

    ImGui::SetNextWindowClass(&window_class);

    ImGui::Begin(ICON_FA_GAMEPAD);

    // RT Copy
    CRenderMgr::GetInst()->RenderTargetCopy();

    // Viewport에서의 마우스 위치 등록
    ImVec2 viewportPos = ImGui::GetCursorScreenPos();
    m_ViewportMousePos = Vec2(ImGui::GetIO().MousePos.x - viewportPos.x, ImGui::GetIO().MousePos.y - viewportPos.y);

    // 상태 확인
    m_ViewportFocused = ImGui::IsWindowFocused();
    m_ViewportHovered = ImGui::IsWindowHovered();

    // 크기 등록
    ImVec2 viewportSize = ImGui::GetContentRegionAvail();
    m_ViewportSize = Vec2(viewportSize.x, viewportSize.y);

    // 렌더링
    Ptr<CTexture> pCopyTex = CRenderMgr::GetInst()->GetRenderTargetCopy();
    ImGui::Image((void*)pCopyTex->GetSRV().Get(), viewportSize);

    // ImGuizmo
    if (m_GizmoActive)
        RenderGizmo();

    // Drag & Drop
    if (ImGui::BeginDragDropTarget())
    {
        // Prefab from content browser
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree"))
        {
            TreeNode** ppNode = (TreeNode**)payload->Data;
            TreeNode* pNode = *ppNode;

            Ptr<CPrefab> pPrefab = (CPrefab*)pNode->GetData();
            if (pPrefab != nullptr)
            {
                CGameObject* pInstantiatedObj = pPrefab->Instantiate();
                if (pInstantiatedObj != nullptr)
                {
                    const wstring& objName = pInstantiatedObj->GetName();
                    pInstantiatedObj->SetName(objName);
                    CreateObject(pInstantiatedObj, 0);
                }
            }
        }

        ImGui::EndDragDropTarget();
    }

    ImGui::End();
}

void CEditorMgr::RenderGizmo()
{
    if (nullptr == m_TargetObject || -1 == m_TargetObject->GetLayerIdx())
        return;

    CCamera* pCam = CRenderMgr::GetInst()->GetMainCamera();

    if (nullptr == pCam)
        return;

    if (m_ViewportFocused)
    {
        if (KEY_TAP(KEY::Z))
            m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
        else if (KEY_TAP(KEY::X))
            m_GizmoType = ImGuizmo::OPERATION::ROTATE;
        else if (KEY_TAP(KEY::C))
            m_GizmoType = ImGuizmo::OPERATION::SCALE;
    }

    ImGuizmo::SetOrthographic(pCam->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC ? true : false);

    ImGuizmo::SetDrawlist(ImGui::GetCurrentWindow()->DrawList);
    float windowWidth = (float)ImGui::GetWindowWidth();
    float windowHeight = (float)ImGui::GetWindowHeight();
    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

    Matrix CamView = pCam->GetcamViewRef();
    Matrix CamProj = pCam->GetcamProjRef();

    // transform
    CTransform* pTr = m_TargetObject->Transform();
    Matrix WorldMat = pTr->GetWorldMat();

    // Snapping
    bool snap = false;
    if (KEY_PRESSED(KEY::CTRL)) snap = true;

    float snapValue = 0.f;
    if (m_GizmoType == ImGuizmo::OPERATION::TRANSLATE)
        snapValue = 25.f;
    else if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
        snapValue = 30.0f;
    else if (m_GizmoType == ImGuizmo::OPERATION::SCALE)
        snapValue = 1.0f;

    float snapValues[3] = { snapValue, snapValue, snapValue };

    ImGuizmo::Manipulate(*CamView.m, *CamProj.m, (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, *WorldMat.m, nullptr, snap ? snapValues : nullptr);

    if (ImGuizmo::IsUsing())
    {
        Matrix originWorldMat = pTr->GetWorldMat();

        if (nullptr != pTr->GetOwner()->GetParent())
        {
            Matrix InvTransformationMat = pTr->GetTransformationMat().Invert();

            WorldMat *= InvTransformationMat;
            originWorldMat *= InvTransformationMat;
        }

        // ImGuizmo변화량이 적용된 Matrix와 원본 Matrix SRT 분해
        float Ftranslation[3] = { 0.0f, 0.0f, 0.0f }, Frotation[3] = { 0.0f, 0.0f, 0.0f }, Fscale[3] = { 0.0f, 0.0f, 0.0f };
        ImGuizmo::DecomposeMatrixToComponents(*WorldMat.m, Ftranslation, Frotation, Fscale);

        float originFtranslation[3] = { 0.0f, 0.0f, 0.0f }, originFrotation[3] = { 0.0f, 0.0f, 0.0f }, originFscale[3] = { 0.0f, 0.0f, 0.0f };
        ImGuizmo::DecomposeMatrixToComponents(*originWorldMat.m, originFtranslation, originFrotation, originFscale);

        // ImGuizmo로 조정한 변화량 추출
        Vec3 vPosOffset =
            Vec3(originFtranslation[0] - Ftranslation[0], originFtranslation[1] - Ftranslation[1], originFtranslation[2] - Ftranslation[2]);
        Vec3 vRotOffset = Vec3(DirectX::XMConvertToRadians(originFrotation[0]) - DirectX::XMConvertToRadians(Frotation[0]),
            DirectX::XMConvertToRadians(originFrotation[1]) - DirectX::XMConvertToRadians(Frotation[1]),
            DirectX::XMConvertToRadians(originFrotation[2]) - DirectX::XMConvertToRadians(Frotation[2]));
        Vec3 vScaleOffset = Vec3(originFscale[0] - Fscale[0], originFscale[1] - Fscale[1], originFscale[2] - Fscale[2]);

        pTr->SetRelativePos(pTr->GetRelativePos() - vPosOffset);
        pTr->SetRelativeRotation(pTr->GetRelativeRotation() - vRotOffset);
        pTr->SetRelativeScale(pTr->GetRelativeScale() - vScaleOffset);
    }
}


void CEditorMgr::ImGuiRun()
{
    // Start ImGui
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
    ImGuizmo::BeginFrame();

    ParamUI::ResetID();

    ImGuiTick();

    if (KEY_TAP(KEY::G) && m_ViewportFocused)
    {
        m_GizmoActive = !m_GizmoActive;
        m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
    }

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