#include "pch.h"
#include "EditorViewport.h"
#include "CAssetMgr.h"
#include "ModotHelpers.h"
#include "CEditorMgr.h"
void EditorViewport::DrawLoadingAssetWindow()
{
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

void EditorViewport::DrawViewportTransitionButtons()
{
    VIEWPORT_TYPE vpType = CEditorMgr::GetInst()->GetCurViewportType();
    switch (vpType)
    {
    case VIEWPORT_TYPE::LEVEL:
    {
        if (ImGui::Button("##ModelEditorTransition"))
        {
            CEditorMgr::GetInst()->ChangeViewport(VIEWPORT_TYPE::MODEL);
        }
        ImGui::SetItemTooltip(u8"모델 에디터 열기");
        break;
    }
    case VIEWPORT_TYPE::MODEL:
    {
        if (ImGui::Button(ICON_FA_MAP_O"##LevelEditorTransition"))
        {
            CEditorMgr::GetInst()->ChangeViewport(VIEWPORT_TYPE::LEVEL);
        }
        ImGui::SetItemTooltip(u8"레벨 에디터 열기");
        break;
    }
    }

    
}
