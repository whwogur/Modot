#include "pch.h"
#include "EditorViewport.h"
#include "CAssetMgr.h"
#include "ModotHelpers.h"
#include "CEditorMgr.h"
#include <Engine/CRenderMgr.h>

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
        if (ImGui::Button(ICON_FA_USER"##ModelEditorTransition"))
        {
            CEditorMgr::GetInst()->ChangeViewport(VIEWPORT_TYPE::MODEL);
        }
        ImGui::SetItemTooltip(u8"�� ������ ����");
        break;
    }
    case VIEWPORT_TYPE::MODEL:
    {
        if (ImGui::Button(ICON_FA_MAP_O"##LevelEditorTransition"))
        {
            CEditorMgr::GetInst()->ChangeViewport(VIEWPORT_TYPE::LEVEL);
        }
        ImGui::SetItemTooltip(u8"���� ������ ����");
        break;
    }
    }
}

void EditorViewport::DrawRenderTarget()
{
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
}
