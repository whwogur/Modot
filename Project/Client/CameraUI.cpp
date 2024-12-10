#include "pch.h"
#include "CameraUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CCamera.h>

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CTransform.h>

#include "CEditorMgr.h"
#include <ClientStatic.h>
CameraUI::CameraUI()
	: ComponentUI(COMPONENT_TYPE::CAMERA)
    , m_ShowLayerCheck(false)
{
}

CameraUI::~CameraUI()
{
}

void CameraUI::Update()
{
	Title();

    if (!Collapsed())
    {
        CCamera* pCam = GetTargetObject()->Camera();

        bool& camActive = pCam->GetActiveRef();
        ClientStatic::ToggleButton("##CamActive", &camActive);
        ImGui::SetItemTooltip(u8"ī�޶� Ȱ��ȭ/��Ȱ��ȭ");

        ImGui::SameLine(250);
        if (ImGui::Button(ICON_FA_CLIPBOARD" EditorView"))
        {
            if (CLevelMgr::GetInst()->GetCurrentLevel()->GetState() == LEVEL_STATE::STOP)
            {
                CTransform* pTrans = CRenderMgr::GetInst()->GetMainCamera()->Transform();
                if (pTrans != nullptr)
                {
                    const Vec3& pRot = pTrans->GetRelativeRotationRef();
                    const Vec3& pPos = pTrans->GetRelativePosRef();
                    pCam->Transform()->SetRelativePos(pPos);
                    pCam->Transform()->SetRelativeRotation(pRot);

                    CEditorMgr::GetInst()->EditorTrace("Copied");
                }
                else
                {
                    CEditorMgr::GetInst()->EditorError("Couldn't find EditorCam->Transform !");
                }
            }
            else
            {
                CEditorMgr::GetInst()->EditorError("LevelState Must be at STOP !!");
            }
        }
        ImGui::SetItemTooltip(u8"������ ī�޶� ��ġ ����");
        LayerCheck();

        Projection();

        float Width = pCam->GetWidth();
        ImGui::Text("Width");
        ImGui::SameLine(INDENT_1);
        ImGui::InputFloat("##Width", &Width);
        pCam->SetWidth(Width);

        float Height = pCam->GetHeight();
        ImGui::Text("Height");
        ImGui::SameLine(INDENT_1);
        ImGui::InputFloat("##Height", &Height);
        pCam->SetHeight(Height);

        float AR = pCam->GetAspectRatio();
        ImGui::Text("AspectRatio");
        ImGui::SameLine(INDENT_1);
        ImGui::InputFloat("##AspectRatio", &AR, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

        float Far = pCam->GetFar();
        ImGui::Text("Far");
        ImGui::SameLine(INDENT_1);
        ImGui::InputFloat("##Far", &Far);
        pCam->SetFar(Far);

        // Perspective ����
        float FOV = pCam->GetFOV();
        FOV = (FOV / XM_PI) * 180.f;

        bool IsPerspective = pCam->GetProjType() == PROJ_TYPE::PERSPECTIVE;
        ImGui::BeginDisabled(!IsPerspective);
        ImGui::Text("FOV");
        ImGui::SameLine(INDENT_1);
        ImGui::InputFloat("##FOV", &FOV);
        ImGui::EndDisabled();

        ImGui::BeginDisabled(IsPerspective);
        float Scale = pCam->GetScale();
        ImGui::Text("Scale");
        ImGui::SameLine(INDENT_1);
        if (ImGui::InputFloat("##Scale", &Scale, 0.5f, 0.5f, "%.1f", ImGuiInputTextFlags_EnterReturnsTrue))
        {
            pCam->SetScale(Scale);
        }
        ImGui::EndDisabled();

        int prior = pCam->GetPriority();
        ImGui::Text("Priority");
        ImGui::SameLine(INDENT_1);
        if (ImGui::InputInt("##Priority", &prior, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            pCam->SetPriority(prior);
        }
        ImGui::SetItemTooltip(u8"0: ���� 1: Other");
    }
    
}

void CameraUI::LayerCheck()
{
    ImGui::Text("Layer Check");
    ImGui::SameLine(100);
    if (ImGui::Button(ICON_FA_SORT_NUMERIC_ASC "##LayerCheckBtn", ImVec2(50.f, 25.f)))
    {
        m_ShowLayerCheck ? 
            m_ShowLayerCheck = false : m_ShowLayerCheck = true;
    }

    if (!m_ShowLayerCheck)
        return;


    CCamera* pCam = GetTargetObject()->Camera();

    bool bLayer[32] = {};
    for (int i = 0; i < 32; ++i)
    {
        bLayer[i] = pCam->GetLayerCheck(i);
    }

    int ChangedIdx = -1;
    if (ImGui::BeginTable("##LayerCheckTable", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
    {
        for (int i = 0; i < 32; i++)
        {
            char label[32];
            sprintf_s(label, "Layer %d", i);
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            if (ImGui::Selectable(label, &bLayer[i], ImGuiSelectableFlags_SpanAllColumns))
            {
                ChangedIdx = i;
            }

            wstring wlayerName = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(i)->GetName();
            string layerName(wlayerName.begin(), wlayerName.end());
            ImGui::TableNextColumn();
            ImGui::Text(layerName.c_str());
        }
        ImGui::EndTable();
    }

    if (ChangedIdx != -1)
    {
        pCam->SetLayer(ChangedIdx, bLayer[ChangedIdx]);
    }
}

void CameraUI::Projection()
{
    CCamera* pCam = GetTargetObject()->Camera();
    PROJ_TYPE Type = pCam->GetProjType();

    const char* items[] = { "Orthographic", "Perspective" };
    const char* combo_preview_value = items[Type];

    ImGui::Text("Projection");
    ImGui::SameLine(INDENT_1);
    ImGui::SetNextItemWidth(180);

    if (ImGui::BeginCombo("##ProjectionCombo", combo_preview_value))
    {
        for (int i = 0; i < 2; i++)
        {
            const bool is_selected = (Type == i);

            if (ImGui::Selectable(items[i], is_selected))
            {
                Type = (PROJ_TYPE)i;
            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    pCam->SetProjType(Type);
}