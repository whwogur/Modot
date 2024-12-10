#include "pch.h"
#include "EditorViewport.h"
#include "CAssetMgr.h"
#include "ClientStatic.h"
#include "CEditorMgr.h"
#include <Engine/CRenderMgr.h>

void EditorViewport::DrawLoadingAssetWindow()
{
    if (CAssetMgr::GetInst()->IsAssetLoading())
    {
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.02f, 0.02f, 0.02f, 0.f));
        ImGui::Begin("Loading . . .", 0, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoScrollbar);

        // FPS 30
        const float frameLength = 1.f / 30.f;
        static float frameTimer = frameLength;

        const int atlasRows = 10;
        const int atlasCols = 10;
        const int totalFrames = atlasRows * atlasCols;

        static int currentFrame = 0;

        // UV
        float uvWidth = 1.0f / atlasCols;
        float uvHeight = 1.0f / atlasRows;
        int frameRow = currentFrame / atlasCols;
        int frameCol = currentFrame % atlasCols;
        float uv0_x = frameCol * uvWidth;
        float uv0_y = frameRow * uvHeight;
        float uv1_x = uv0_x + uvWidth;
        float uv1_y = uv0_y + uvHeight;

        ImGui::Image((ImTextureID)CAssetMgr::GetInst()->FindAsset<CTexture>(L"PepeCozy")->GetSRV().Get()
            , ImVec2(500, 400), ImVec2(uv0_x, uv0_y), ImVec2(uv1_x, uv1_y), {1.f, 1.f, 1.f, 0.95f});

        frameTimer -= ImGui::GetIO().DeltaTime;
        if (frameTimer <= 0.f)
        {
            frameTimer = frameLength;
            ++currentFrame;

            if (currentFrame >= totalFrames)
                currentFrame = 0;
        }

        // ���� â�� ũ��� ���� ��ġ ��������
        ImVec2 windowSize = ImGui::GetWindowSize();
        ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();

        // ������ ������ �߾� ��ġ ���
        ImVec2 cursorPosInWindow(
            (windowSize.x - contentRegionAvail.x) * 0.5f + contentRegionAvail.x * 0.5f,
            (windowSize.y - contentRegionAvail.y) * 0.5f + contentRegionAvail.y * 0.5f
        );

        ImGui::SetCursorPos(cursorPosInWindow);
        const float prog = CAssetMgr::GetInst()->GetLoadingProgress();
        ClientStatic::LoadingIndicatorCircle("##spinner", 50.f, { 0.5f, 0.8f, 1.f, 1.f }, { 1.f, 1.f, 1.f, 1.f }, 10, 10.f);

        ImGui::End();
        ImGui::PopStyleColor(1);
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
