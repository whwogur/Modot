#include "pch.h"
#include "LevelEditor.h"
#include "CEditorMgr.h"
#include "ImGui/imgui.h"
#include "ImGui/ImGuizmo.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_internal.h"
#include "CEditorCameraScript.h"
#include "TreeUI.h"
#include "ModotHelpers.h"

#include <Engine/CCamera.h>
#include <Engine/CLevel.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CTransform.h>

void LevelEditor::Update()
{
    ImGuiWindowClass window_class;
    window_class.ClassId = ImGui::GetID("Editor Viewport");
    window_class.DockNodeFlagsOverrideSet = 0;
    window_class.DockingAllowUnclassed = true;

    ImGui::SetNextWindowClass(&window_class);

    ImGui::Begin(ICON_FA_GAMEPAD" Viewport");

    if (m_GizmoActive)
    {
        ImGui::TextColored(m_GizmoType == 7 ? ImVec4(HEADER_2) : ImVec4(0.5f, 0.5f, 0.5f, 1), ICON_FA_ARROWS); // TRANS
        ImGui::SetItemTooltip("Translation (Z)");
        ImGui::SameLine();
        ImGui::TextColored(m_GizmoType == 120 ? ImVec4(HEADER_2) : ImVec4(0.5f, 0.5f, 0.5f, 1), ICON_FA_REFRESH); // RITATE
        ImGui::SetItemTooltip("Rotation (X)");
        ImGui::SameLine();
        ImGui::TextColored(m_GizmoType == 896 ? ImVec4(HEADER_2) : ImVec4(0.5f, 0.5f, 0.5f, 1), ICON_FA_EXPAND); // SCALE
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

    // RT Copy
    CRenderMgr::GetInst()->RenderTargetCopy();

    // Viewport������ ���콺 ��ġ ���
    ImVec2 viewportPos = ImGui::GetCursorScreenPos();
    CRenderMgr::GetInst()->SetEditorMousePos(Vec2(ImGui::GetIO().MousePos.x - viewportPos.x, ImGui::GetIO().MousePos.y - viewportPos.y));
    CRenderMgr::GetInst()->SetViewportFocused(ImGui::IsWindowFocused());
    CRenderMgr::GetInst()->SetViewportHovered(ImGui::IsWindowHovered());

    // ũ�� ���
    ImVec2 viewportSize = ImGui::GetContentRegionAvail();
    const Vec2& vpSize = GetSize();
    if (fabs(viewportSize.x - vpSize.x) > 1e-6f || fabs(viewportSize.y - vpSize.y) > 1e-6f)
    {
        SetSize(Vec2(viewportSize.x, viewportSize.y));
        CRenderMgr::GetInst()->SetEditorViewportSize(Vec2(viewportSize.x, viewportSize.y));
    }

    // ������
    Ptr<CTexture> pCopyTex = CRenderMgr::GetInst()->GetRenderTargetCopy();
    ImGui::Image((ImTextureID)(void*)pCopyTex->GetSRV().Get(), viewportSize);

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

    if (CAssetMgr::GetInst()->IsAssetLoading())
    {
        ImGui::Begin("Asset Loading", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking);

        const float prog = CAssetMgr::GetInst()->GetLoadingProgress();
        const ImU32 spinnerCol = ImGui::GetColorU32(ImVec4{ 0.2f, 0.45f, 0.811f, 1.0f });
        const ImU32 barCol = ImGui::GetColorU32(ImVec4{ 0.32f, 0.37f, 0.88f, 1.0f });

        ImGui::SameLine(ImGui::GetContentRegionAvail().x / 2);

        ModotHelpers::LoadingIndicatorCircle("##spinner", 30.f, { 0.5f, 0.8f, 1.f, 1.f }, { 1.f, 1.f, 1.f, 1.f }, 10, 10.f);

        ImGui::End();
    }
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

        // ImGuizmo��ȭ���� ����� Matrix�� ���� Matrix SRT ����
        float Ftranslation[3] = { 0.0f, 0.0f, 0.0f }, Frotation[3] = { 0.0f, 0.0f, 0.0f }, Fscale[3] = { 0.0f, 0.0f, 0.0f };
        ImGuizmo::DecomposeMatrixToComponents(*WorldMat.m, Ftranslation, Frotation, Fscale);

        float originFtranslation[3] = { 0.0f, 0.0f, 0.0f }, originFrotation[3] = { 0.0f, 0.0f, 0.0f }, originFscale[3] = { 0.0f, 0.0f, 0.0f };
        ImGuizmo::DecomposeMatrixToComponents(*originWorldMat.m, originFtranslation, originFrotation, originFscale);

        // ImGuizmo�� ������ ��ȭ�� ����
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
        ModotHelpers::HSliderFloat("##EditorCamSpeedSlider", &camSpeed, 100.f, 1000.f, "%.1f", ImGuiSliderFlags_AlwaysClamp);
        ImGui::SetItemTooltip(u8"������ ī�޶� �ӵ� ����");
    }
}