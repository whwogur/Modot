#include "pch.h"
#include "CameraUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CCamera.h>

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>

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

    CCamera* pCam = GetTargetObject()->Camera();
    
    LayerCheck();

    Projection();

    float Width = pCam->GetWidth();
    ImGui::Text("Width");
    ImGui::SameLine(100);
    ImGui::InputFloat("##Width", &Width);
    pCam->SetWidth(Width);

    float Height = pCam->GetHeight();
    ImGui::Text("Height");
    ImGui::SameLine(100);
    ImGui::InputFloat("##Height", &Height);
    pCam->SetHeight(Height);

    float AR = pCam->GetAspectRatio();
    ImGui::Text("AspectRatio");
    ImGui::SameLine(100);
    ImGui::InputFloat("##AspectRatio", &AR, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

    float Far = pCam->GetFar();
    ImGui::Text("Far");
    ImGui::SameLine(100);
    ImGui::InputFloat("##Far", &Far);
    pCam->SetFar(Far);

    // Perspective Àü¿ë
    float FOV = pCam->GetFOV();
    FOV = (FOV / XM_PI) * 180.f;

    bool IsPerspective = pCam->GetProjType() == PROJ_TYPE::PERSPECTIVE;
    ImGui::BeginDisabled(!IsPerspective);
    ImGui::Text("FOV");
    ImGui::SameLine(100);
    ImGui::InputFloat("##FOV", &FOV);
    ImGui::EndDisabled();

    ImGui::BeginDisabled(IsPerspective);
    float Scale = pCam->GetScale();
    ImGui::Text("Scale");
    ImGui::SameLine(100);
    ImGui::InputFloat("##Scale", &Scale);
    pCam->SetScale(Scale);
    ImGui::EndDisabled();
}

void CameraUI::LayerCheck()
{
    ImGui::Text("Layer Check");
    ImGui::SameLine(100);
    if (ImGui::Button(ICON_FA_SORT_NUMERIC_ASC "##LayerCheckBtn", ImVec2(50.f, 25.f)))
    {
        m_ShowLayerCheck ? m_ShowLayerCheck = false : m_ShowLayerCheck = true;
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
    ImGui::SameLine(100);
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