#include "pch.h"
#include "AnimationEditor.h"
#include <numeric>
#include "ImGui/imgui_neo_internal.h"
#include "ImGui/imgui_neo_sequencer.h"
#include "CTimeMgr.h"
#include "CAssetMgr.h"
AnimationEditor::AnimationEditor()
	: m_Animation(nullptr)
	, m_Animator(nullptr)
    , m_Play(false)
{
}

void AnimationEditor::Update()
{
    if (m_Animation.Get())
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

        Ptr<CSprite>& curSprite = m_Animation->GetSpriteRef(m_CurrentFrame);
        Vec2 LeftTop = curSprite->GetLeftTopUV();
        Vec2 Slice = curSprite->GetSliceUV();
        Vec2& offsetUV = curSprite->GetOffsetUVRef();

        ImGui::Image(curSprite->GetAtlasTexture().Get()->GetSRV().Get(), { 150, 150 }, { LeftTop.x, LeftTop.y }, { LeftTop.x + Slice.x, LeftTop.y + Slice.y });
        //ImVec2 p = ImGui::GetCursorScreenPos();
        //ImGui::Image(m_CheckerTex.Get()->GetSRV().Get(), { 150, 150 });
        //ImGui::GetWindowDrawList()->AddImage(curSprite->GetAtlasTexture().Get()->GetSRV().Get(), p, { 150,150 }, { LeftTop.x, LeftTop.y }, { LeftTop.x + Slice.x, LeftTop.y + Slice.y });
        
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.22f, 0.23f, 0.77f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.32f, 0.33f, 0.97f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.12f, 0.13f, 0.67f, 1.0f });
        ImGui::SameLine(160);
        if (ImGui::Button("-", { 30, 20 }))
        {
            offsetUV.x -= 0.1;
        }
        ImGui::SameLine(200);
        if (ImGui::Button("+", {30, 20}))
        {
            offsetUV.x += 0.1;
        }
        ImGui::PopStyleColor(3);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.52f, 0.43f, 0.77f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.62f, 0.53f, 0.97f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.42f, 0.43f, 0.67f, 1.0f });
        if (ImGui::Button(m_Play ? "II" : ">", { 50, 30 }))
        {
            m_Play = !m_Play;
        }
        ImGui::PopStyleColor(3);

        ImGui::SameLine(70);
        ImGui::DragFloat("FPS", &m_FPS, 1.0f, 0.1f, 60.f, "%.1f", 0);

        if (ImGui::BeginNeoSequencer(u8"애니메이션", &m_CurrentFrame, &m_StartFrame, &m_EndFrame, {0, 0})) {
            
                std::vector<ImGui::FrameIndexType> keys(m_EndFrame);
                std::iota(keys.begin(), keys.end(), 0);

                if (ImGui::BeginNeoTimeline(u8"키프레임", keys)) {
                    ImGui::EndNeoTimeLine();
            }

            ImGui::EndNeoSequencer();
        }

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
