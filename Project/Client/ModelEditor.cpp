#include "pch.h"
#include "ModelEditor.h"
#include "CGameObjectEx.h"
#include "CCamera.h"
#include "CTransform.h"
#include "TreeUI.h"
#include "CEditorCameraScript.h"
#include "CEditorMgr.h"

#include <Engine/CRenderMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <ModotHelpers.h>
ModelEditor::ModelEditor()
{
}

ModelEditor::~ModelEditor()
{
}

void ModelEditor::Update()
{
    m_ModelEditorCam->Tick();

    ImGuiWindowClass window_class;
    window_class.ClassId = ImGui::GetID("Editor Viewport");
    window_class.DockNodeFlagsOverrideSet = 0;
    window_class.DockingAllowUnclassed = true;
    ImGui::SetNextWindowClass(&window_class);
    ImGui::Begin(ICON_FA_USER" ModelView");

    GizmoInfo();
    ImGui::SameLine();
    DrawViewportTransitionButtons();

    m_ModelEditorCam->FinalTick();

    DrawRenderTarget();
    //// ImGuizmo
    if (m_GizmoActive)
        RenderGizmo();

    AcceptDragDrop();
    DrawLoadingAssetWindow();

    ImGui::End();

    ImGui::Begin("AnimatorControl##Animation3DView");//Temp

    CGameObject* pTarget = GetTargetObject();
    if (pTarget != nullptr)
    {
        CAnimator3D* pAnimator = pTarget->Animator3D();
        Ptr<CMesh> pSkeletalMesh = pTarget->Animator3D()->GetSkeletalMesh();
        // Animation
        if (nullptr != pSkeletalMesh && pAnimator->IsValid())
        {
            ImGui::SeparatorText(u8"��Ʈ��");

            const std::vector<tMTAnimClip>* vecAnimClip = pSkeletalMesh->GetAnimClip();

            int CurClipIdx = pAnimator->GetCurClipIdx();
            const tMTAnimClip& CurClip = vecAnimClip->at(CurClipIdx);

            string CurClipName(CurClip.strAnimName.begin(), CurClip.strAnimName.end());

            static ImGuiTextFilter filter;
            int ChangedClipIdx = -1;
            ImGui::Text(ICON_FA_FILTER); ImGui::SameLine(INDENT_2);
            filter.Draw("##AnimationFilter");
            ImGui::TextColored(HEADER_1, u8"��� "); ImGui::SameLine(INDENT_2);
            if (ImGui::BeginCombo("##Anim", CurClipName.c_str()))
            {
                ImGui::Separator();

                for (int i = 0; i < vecAnimClip->size(); i++)
                {
                    string ClipName(vecAnimClip->at(i).strAnimName.begin(), vecAnimClip->at(i).strAnimName.end());
                    bool is_selected = (CurClipName == ClipName);

                    if (!filter.PassFilter(ClipName.c_str()))
                        continue;

                    if (ImGui::Selectable(ClipName.c_str(), is_selected))
                    {
                        CurClipName = ClipName;
                        ChangedClipIdx = i;
                    }

                    if (is_selected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }

            if (ChangedClipIdx >= 0)
            {
                pAnimator->SetCurClipIdx(ChangedClipIdx);
            }

            ImGui::TextColored(HEADER_1, u8"������:");
            ImGui::SameLine(INDENT_2);
            int ClipFrameIdx = pAnimator->GetFrameIdx();
            if (ImGui::SliderInt("##AnimationFrameIndex", &ClipFrameIdx, 0, CurClip.iFrameLength - 1))
            {
                pAnimator->SetFrameIdx(ClipFrameIdx);
            }

            ImGui::TextColored(HEADER_1, u8"����ӵ�:"); ImGui::SameLine();
            float fPlaySpeed = pAnimator->GetPlaySpeed();
            if (ImGui::DragFloat("##AnimationPlaySpeed", &fPlaySpeed, 0.01f, 0.f, 10.f))
                pAnimator->SetPlaySpeed(fPlaySpeed);

            // Buttons
            ImGui::NewLine();
            ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.48f);
            bool bPlaying = pAnimator->IsPlayingAnim();
            if (bPlaying)
            {
                if (ImGui::Button(ICON_FA_PAUSE))
                    pAnimator->PauseAnimation();
            }
            else
            {
                if (ImGui::Button(ICON_FA_PLAY))
                    pAnimator->ResumeAnimation();
            }

            ImGui::SameLine();
            bool bRepeat = pAnimator->IsOnRepeat();
            if (ImGui::Button(bRepeat ? ICON_FA_RANDOM : ICON_FA_REPEAT))
                pAnimator->SetRepeat(!bRepeat);
            

#pragma region �ִϸ��̼� ����

            ImGui::SeparatorText(u8"Ŭ�� ����");
            ImGui::NewLine();
            ImGui::TextColored(HEADER_1, u8"Ŭ���̸�:");
            ImGui::SameLine(INDENT_1);
            ImGui::Text("%s", string(CurClip.strAnimName.begin(), CurClip.strAnimName.end()).c_str());
            ImGui::TextColored(HEADER_1, u8"Ŭ���ε���:");
            ImGui::SameLine(INDENT_1);
            ImGui::Text("#%d", CurClipIdx);
            ImGui::TextColored(HEADER_1, "FPS:");
            ImGui::SameLine(INDENT_1);
            ImGui::Text("%d", (int)pAnimator->GetFrameCount());
            ImGui::TextColored(HEADER_1, u8"�����ӹ���:");
            ImGui::SameLine(INDENT_1);
            ImGui::Text("%d ~ %d", CurClip.iStartFrame);

            ImGui::TextColored(HEADER_1, u8"������ ����:");
            ImGui::SameLine(INDENT_1);
            ImGui::Text("%d", CurClip.iFrameLength);

            ImGui::TextColored(HEADER_1, u8"���ӽð�:");
            ImGui::SameLine(INDENT_1);
            ImGui::Text("%.3f ~ %.3f", CurClip.dStartTime, CurClip.dEndTime);

            ImGui::TextColored(HEADER_1, u8"Ŭ������:");
            ImGui::SameLine(INDENT_1);
            ImGui::Text("%.3f", CurClip.dTimeLength);
#pragma endregion
        }
    }
    
    ImGui::End();
}

void ModelEditor::SetViewport(VIEWPORT_TYPE _Type)
{
    ChangeLevelState(LEVEL_STATE::MODEL);
    EditorUI* hvTree = CEditorMgr::GetInst()->FindEditorUI("HierarchyViewTree");
    if (hvTree != nullptr)
        hvTree->SetActive(false);
}

void ModelEditor::Init()
{
    //=============
    // ������ ī�޶�
    //=============
    m_ModelEditorCam = std::make_unique<CGameObjectEx>();
    m_ModelEditorCam->SetName(L"ModelEditorCamera");
    m_ModelEditorCam->AddComponent(new CTransform);
    m_ModelEditorCam->AddComponent(new CCamera);
    m_ModelEditorCam->AddComponent(new CEditorCameraScript);

    m_ModelEditorCam->Transform()->SetRelativeScale(1, 1, 1);
    m_ModelEditorCam->Camera()->ClearLayerAll();
    m_ModelEditorCam->Camera()->SetLayer(21, true);
    m_ModelEditorCam->Camera()->SetFar(10000.f);
    m_ModelEditorCam->Camera()->SetProjType(PERSPECTIVE);
    m_ModelEditorCam->Camera()->SetFrustumDebug(false);

    m_ModelEditorCam->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));

    // Light
    m_LightObj = new CGameObject;
    m_LightObj->SetName(L"ModelEditorLight");
    m_LightObj->AddComponent(new CTransform);
    m_LightObj->AddComponent(new CLight3D);

    m_LightObj->Transform()->SetRelativePos(-100.f, -300.f, 0.f);
    m_LightObj->Transform()->SetRelativeRotation(XM_PIDIV4, XM_PIDIV4, 0.f);
    m_LightObj->Light3D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
    m_LightObj->Light3D()->SetLightColor(Vec3(0.9f, 0.9f, 0.9f));
    m_LightObj->Light3D()->SetLightAmbient(Vec3(0.3f, 0.3f, 0.3f));
    m_LightObj->Light3D()->SetSpecularCoefficient(0.3f);
    m_LightObj->Light3D()->SetRadius(400.f);

    //===========
    // ��ī�̹ڽ�
    //===========
    m_SkyBoxObj = new CGameObject;
    m_SkyBoxObj->SetName(L"ModelEditorSkyBox");
    m_SkyBoxObj->AddComponent(new CTransform);
    m_SkyBoxObj->AddComponent(new CSkyBox);

    //========
    // �ٴ�
    //========
    m_FloorObj = new CGameObject;
    m_FloorObj->SetName(L"ModelEditorFloor");
    m_FloorObj->AddComponent(new CTransform);
    m_FloorObj->AddComponent(new CMeshRender);

    m_FloorObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
    m_FloorObj->Transform()->SetRelativeScale(Vec3(1000.f, 10.f, 1000.f));

    m_FloorObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CubeMesh"));
    m_FloorObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std3DMtrl"), 0);
    //m_FloorObj->MeshRender()->GetDynamicMaterial(0)->SetMaterialCoefficient(Vec4(1.f, 1.f, 1.f, 1.f), Vec4(), Vec4(), Vec4());
    m_FloorObj->MeshRender()->SetFrustumCheck(false);

    //============
    // �� ������Ʈ
    //============
    Ptr<CMeshData> pMeshData = CAssetMgr::GetInst()->Load<CMeshData>(L"mn_vorc_00_ani", L"meshdata\\mn_vorc_00_ani.mdat");
    m_ModelObj = pMeshData->Instantiate(); // TEST
    m_ModelObj->Transform()->SetRelativePos(Vec3(200.f, 0.f, 200.f));
    m_ModelObj->Transform()->SetRelativeScale(Vec3(1.f, 1.f, 1.f));
    m_ModelObj->Transform()->SetRelativeRotation(Vec3(XM_PIDIV4, 0, XM_PIDIV4));
    
    CreateObject(m_ModelObj, LAYER_MODEL);
    CreateObject(m_FloorObj, LAYER_MODEL);
    CreateObject(m_SkyBoxObj, LAYER_MODEL);
    CreateObject(m_LightObj, LAYER_MODEL);

    CRenderMgr::GetInst()->SetModelCam(m_ModelEditorCam->Camera());
}

void ModelEditor::RenderGizmo()
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

void ModelEditor::GizmoInfo()
{
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

    if (KEY_TAP(KEY::G) && CRenderMgr::GetInst()->IsViewportHovered())
    {
        m_GizmoActive = !m_GizmoActive;
        m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
    }
}

void ModelEditor::AcceptDragDrop()
{
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
}