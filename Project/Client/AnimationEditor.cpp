#include "pch.h"
#include "AnimationEditor.h"
#include <numeric>
#include "ImGui/imgui_neo_internal.h"
#include "ImGui/imgui_neo_sequencer.h"

AnimationEditor::AnimationEditor()
	: m_Animation(nullptr)
	, m_Animator(nullptr)
{
}

void AnimationEditor::Update()
{
	if (m_Animation.Get())
	{
       Ptr<CSprite> curSprite = m_Animation->GetSprite(m_CurrentFrame);
       Vec2 LeftTop = curSprite->GetLeftTopUV();
       Vec2 Slice = curSprite->GetSliceUV();

       ImGui::Image(curSprite->GetAtlasTexture().Get()->GetSRV().Get(), { 100, 100 }, { LeftTop.x, LeftTop.y }, { LeftTop.x + Slice.x, LeftTop.y + Slice.y });

       static bool transformOpen = false;
       ImVec2 availSpace = ImGui::GetContentRegionAvail();
       if (ImGui::BeginNeoSequencer(u8"애니메이션", &m_CurrentFrame, &m_StartFrame, &m_EndFrame, {0, 0})) {
            
                std::vector<ImGui::FrameIndexType> keys(m_EndFrame);
                std::iota(keys.begin(), keys.end(), 0);

                if (ImGui::BeginNeoTimeline(u8"키프레임", keys)) {
                    if (m_CurrentFrame > m_EndFrame)
                        m_CurrentFrame = m_EndFrame;
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
}
