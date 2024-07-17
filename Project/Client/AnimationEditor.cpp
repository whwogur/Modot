#include "pch.h"
#include "AnimationEditor.h"
#include <numeric>
#include "ImGui/imgui_neo_internal.h"
#include "ImGui/imgui_neo_sequencer.h"
#include "CTimeMgr.h"
#include "CAssetMgr.h"
#include "CEditorMgr.h"
#include "SpriteEditor.h"
#include "TreeUI.h"

AnimationEditor::AnimationEditor()
	: m_Animation(nullptr)
    , m_Play(false)
{
}

void AnimationEditor::Update()
{
    if (ImGui::BeginDragDropTarget())
    {
        const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
        if (payload)
        {
            TreeNode** ppNode = (TreeNode**)payload->Data;
            TreeNode* pNode = *ppNode;

            Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
            if (ASSET_TYPE::ANIMATION == pAsset->GetAssetType())
            {
                m_Animation = (CAnimation*)pAsset.Get();
                Refresh();
            }
        }

        ImGui::EndDragDropTarget();
    }

    if (m_Animation.Get() != nullptr)
    {
        if (m_Play)
        {
            m_Acc += EngineDT;
            if (m_Acc > (1 / m_FPS))
            {
                ++m_CurrentFrame;
                m_Acc = 0.0f;
            }
            if (m_CurrentFrame >= m_EndFrame)
                m_CurrentFrame = 0;
        }

        static float deltaUV = 0.1f;
        Ptr<CSprite>& curSprite = m_Animation->GetSpriteRef(m_CurrentFrame);
        Vec2& offsetUV = curSprite->GetOffsetUVRef();
        Vec2 LeftTop = curSprite->GetLeftTopUV() - offsetUV;
        Vec2 Slice = curSprite->GetSliceUV() - offsetUV;

        ImGui::TextColored({ 0.3f, 0.5f, 0.7f, 1.0f }, "<%f , %f>", offsetUV.x, offsetUV.y);
        ImGui::Image(curSprite->GetAtlasTexture().Get()->GetSRV().Get(), { 150, 150 }, { LeftTop.x, LeftTop.y }, { LeftTop.x + Slice.x, LeftTop.y + Slice.y });
        
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.341f, 0.0f, 1.0f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.44f, 0.2f, 1.0f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.0f, 0.8f, 1.0f });
        ImGui::SameLine(160);
        if (ImGui::Button(ICON_FA_ARROW_LEFT, { 25, 25 }))
        {
            offsetUV.x -= deltaUV;
        }
        ImGui::SameLine(200);
        if (ImGui::Button(ICON_FA_ARROW_RIGHT, {25, 25}))
        {
            offsetUV.x += deltaUV;
        }

        ImGui::SameLine(240);

        ImGui::PushItemWidth(50.f);
        ImGui::DragFloat(u8"UV조절량", &deltaUV, 0.001f, 0.0f, 1.0f, "%.3f");
        ImGui::PopItemWidth();

        ImGui::SameLine(400);
        if (ImGui::Button("Atlas Texture " ICON_FA_PICTURE_O, {30, 30}))
        {
            SpriteEditor* spriteEditor = static_cast<SpriteEditor*>(CEditorMgr::GetInst()->FindEditorUI("SpriteEditor"));
            spriteEditor->SetAtlas(curSprite->GetAtlasTexture());
            spriteEditor->SetActive(true);
        }

        ImGui::SameLine(440);
        if (ImGui::Button(ICON_FA_FLOPPY_O, { 30, 30 }))
        {
            const wstring& contentPath = CPathMgr::GetInst()->GetContentPath();
            wstring savePath = contentPath + L"sprite\\" + curSprite->GetKey();
            curSprite->Save(savePath);
        }

        if (ImGui::Button(m_Play ? ICON_FA_PAUSE : ICON_FA_PLAY, { 50, 30 }))
        {
            m_Play = !m_Play;
        }
        ImGui::PopStyleColor(3);

        ImGui::SameLine(70);
        ImGui::PushItemWidth(100.f);
        ImGui::DragFloat("FPS", &m_FPS, 1.0f, 0.1f, 60.f, "%.1f", 0);
        ImGui::PopItemWidth();

        if (ImGui::BeginNeoSequencer(u8"애니메이션", &m_CurrentFrame, &m_StartFrame, &m_EndFrame, {0, 0})) {
            
                std::vector<ImGui::FrameIndexType> keys(m_EndFrame);
                std::iota(keys.begin(), keys.end(), 0);

                if (ImGui::BeginNeoTimeline(u8"키프레임", keys)) {
                    ImGui::EndNeoTimeLine();
            }

            ImGui::EndNeoSequencer();
        }

	}
    else
    {
        ImGui::SetWindowFontScale(1.18f);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.222f, 0.222f, 0.222f, 0.7f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.222f, 0.222f, 0.222f, 0.7f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.222f, 0.222f, 0.222f, 0.7f });


        ImGui::SameLine(160);
        if (ImGui::Button(ICON_FA_ARROW_LEFT, { 25, 25 }))
        {
        }
        ImGui::SameLine(200);
        if (ImGui::Button(ICON_FA_ARROW_RIGHT, { 25, 25 }))
        {
        }

        ImGui::SameLine(400);
        if (ImGui::Button(ICON_FA_PENCIL_SQUARE_O, { 30, 30 }))
        {
        }

        ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, u8"애니메이션을 선택해주세요!!!");
        ImGui::PopStyleColor(3);
    }
}

void AnimationEditor::Refresh()
{
    m_CurrentFrame = 0;
    m_StartFrame = 0;
    m_EndFrame = m_Animation->GetMaxFrameCount();
    m_Play = false;
    m_FPS = 1.0f;
}
