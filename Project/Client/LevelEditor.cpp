#include "pch.h"
#include "LevelEditor.h"
#include "CEditorMgr.h"

#include "CEditorCameraScript.h"
#include "TreeUI.h"
#include "ClientStatic.h"

#include <Engine/CCamera.h>
#include <Engine/CLevel.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CTransform.h>
#include "CGameObjectEx.h"

void LevelEditor::Update()
{
    m_LevelEditorCam->Tick();

    ImGuiWindowClass window_class;
    window_class.ClassId = ImGui::GetID("Editor Viewport");
    window_class.DockNodeFlagsOverrideSet = 0;
    window_class.DockingAllowUnclassed = true;

    ImGui::SetNextWindowClass(&window_class);

    ImGui::Begin(ICON_FA_GAMEPAD" Viewport");

    if (m_GizmoActive)
    {
        ImGui::TextColored(m_GizmoType == ImGuizmo::OPERATION::TRANSLATE ? ImVec4(HEADER_2) : ImVec4(0.5f, 0.5f, 0.5f, 1), ICON_FA_ARROWS); // TRANS
        ImGui::SetItemTooltip("Translation (Z)");
        ImGui::SameLine();
        ImGui::TextColored(m_GizmoType == ImGuizmo::OPERATION::ROTATE ? ImVec4(HEADER_2) : ImVec4(0.5f, 0.5f, 0.5f, 1), ICON_FA_REFRESH); // Rotate
        ImGui::SetItemTooltip("Rotation (X)");
        ImGui::SameLine();
        ImGui::TextColored(m_GizmoType == ImGuizmo::OPERATION::SCALE ? ImVec4(HEADER_2) : ImVec4(0.5f, 0.5f, 0.5f, 1), ICON_FA_EXPAND); // SCALE
        ImGui::SetItemTooltip("Scale (C)");
        ImGui::SameLine();
        ImGui::TextColored({ 0.5f, 0.5f, 0.5f, 1 }, ICON_FA_MOUSE_POINTER);
    }
    else
    {
        ImGui::Text(ICON_FA_ARROWS);
        ImGui::SetItemTooltip(u8"Translation Z");
        ImGui::SameLine();
        ImGui::Text(ICON_FA_REFRESH);
        ImGui::SetItemTooltip(u8"Rotation X");
        ImGui::SameLine();
        ImGui::Text(ICON_FA_EXPAND);
        ImGui::SetItemTooltip(u8"Scale C");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(HEADER_2), ICON_FA_MOUSE_POINTER);
    }

    ImGui::SameLine();
    PROJ_TYPE type = CRenderMgr::GetInst()->GetMainCamera()->GetProjType();
    if (type == PROJ_TYPE::ORTHOGRAPHIC)
        ImGui::TextColored(HEADER_1, ICON_FA_VIDEO_CAMERA" Orthographic");
    else
        ImGui::TextColored(HEADER_1, ICON_FA_VIDEO_CAMERA" Perspective");

    EditorCameraSlider();

    ImGui::SameLine();
    DrawViewportTransitionButtons();

    m_LevelEditorCam->FinalTick();

    DrawDebugLine(Vec3(0.f, 0.f, 0.f), Vec3(1000.f, 0.f, 0.f), Vec4(1.f, 0.f, 0.f, 1.f), 0.f, true);
    DrawDebugLine(Vec3(0.f, 0.f, 0.f), Vec3(0.f, 1000.f, 0.f), Vec4(0.f, 1.f, 0.f, 1.f), 0.f, true);
    DrawDebugLine(Vec3(0.f, 0.f, 0.f), Vec3(0.f, 0.f, 1000.f), Vec4(0.f, 0.f, 1.f, 1.f), 0.f, true);

    DrawRenderTarget();

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

            CAsset* pAsset = reinterpret_cast<CAsset*>(pNode->GetData());
            if (pAsset->GetAssetType() == ASSET_TYPE::PREFAB)
            {
                Ptr<CPrefab> pPrefab = (CPrefab*)pAsset;
                CGameObject* pInstantiatedObj = pPrefab->Instantiate();
                if (pInstantiatedObj != nullptr)
                {
                    const wstring& objName = pInstantiatedObj->GetName();
                    pInstantiatedObj->SetName(objName);
                    CreateObject(pInstantiatedObj, 0);
                }
            }
            else if (pAsset->GetAssetType() == ASSET_TYPE::MESH_DATA)
            {
                Ptr<CMeshData> pMeshData = (CMeshData*)pAsset;
                CGameObject* pInstantiatedObj = pMeshData->Instantiate();
                if (pInstantiatedObj != nullptr)
                {
                    const wstring& objName = pMeshData->GetKey();
                    pInstantiatedObj->SetName(objName);
                    pInstantiatedObj->Transform()->SetRelativePos(0.f, 0.f, 0.f);
                    pInstantiatedObj->Transform()->SetRelativeScale(3.f, 3.f, 3.f);

                    CreateObject(pInstantiatedObj, 0);
                }
            }
        }

        ImGui::EndDragDropTarget();
    }

    ImGui::End();

    if (KEY_TAP(KEY::G) && CRenderMgr::GetInst()->IsViewportHovered())
    {
        m_GizmoActive = !m_GizmoActive;
        m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
    }

    DrawLoadingAssetWindow();
}

void LevelEditor::SetViewport(VIEWPORT_TYPE _Type)
{
    ChangeLevelState(LEVEL_STATE::STOP);
    CRenderMgr::GetInst()->SetEditorCam(m_LevelEditorCam->Camera());
    if (_Type == VIEWPORT_TYPE::MODEL)
    {
        EditorUI* hvTree = CEditorMgr::GetInst()->FindEditorUI("HierarchyView");
        if (hvTree != nullptr)
            hvTree->SetChildActive(true);
    }
}

void LevelEditor::Init()
{
    //=============
    // 에디터 카메라
    //=============
    m_LevelEditorCam = std::make_unique<CGameObjectEx>();
    m_LevelEditorCam->SetName(L"LevelEditorCamera");
    m_LevelEditorCam->AddComponent(new CTransform);
    m_LevelEditorCam->AddComponent(new CCamera);
    m_LevelEditorCam->AddComponent(new CEditorCameraScript);

    m_LevelEditorCam->Transform()->SetRelativeScale(1, 1, 1);
    m_LevelEditorCam->Transform()->SetRelativePos(0, 0, 0);
    m_LevelEditorCam->Camera()->SetLayerGeneral();
    m_LevelEditorCam->Camera()->SetFar(10000.f);
    m_LevelEditorCam->Camera()->SetProjType(PERSPECTIVE);
    m_LevelEditorCam->Camera()->SetFrustumDebug(false);

    m_LevelEditorCam->Transform()->SetRelativePos(Vec3(0.f, 0.f, -1000.f));

    CRenderMgr::GetInst()->SetEditorCam(m_LevelEditorCam->Camera()); // 에디터 디폴트 뷰포트
}

void LevelEditor::RenderGizmo()
{
    if (nullptr == GetTargetObject() || -1 == GetTargetObject()->GetLayerIdx())
        return;

    CCamera* pCam = CRenderMgr::GetInst()->GetMainCamera();

    if (nullptr == pCam)
        return;

    if (ImGui::IsWindowFocused())
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
    CTransform* pTr = GetTargetObject()->Transform();
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

void LevelEditor::EditorCameraSlider()
{
    LEVEL_STATE lvState = CLevelMgr::GetInst()->GetCurrentLevel()->GetState();
    if (lvState == LEVEL_STATE::STOP)
    {
        ImGui::SameLine();
        static CGameObject* editorCam = CRenderMgr::GetInst()->GetEditorCamera()->GetOwner();
        const std::vector<CScript*>& vecScript = editorCam->GetScriptsRef();
        CEditorCameraScript* eCamScript = (CEditorCameraScript*)vecScript[0];
        float& camSpeed = eCamScript->GetCamSpeedRef();

        ImGui::SetNextItemWidth(150);
        ClientStatic::HSliderFloat("##EditorCamSpeedSlider", &camSpeed, 100.f, 1000.f, "%.1f", ImGuiSliderFlags_AlwaysClamp);
        ImGui::SetItemTooltip(u8"에디터 카메라 속도 조절");
    }
}