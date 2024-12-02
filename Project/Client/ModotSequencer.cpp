#include "pch.h"
#include "ModotSequencer.h"
#include "ImGui/imgui_internal.h"
namespace Modot
{
    void ModotSequencer::Get(int index, int** start, int** end, int* type, unsigned int* color)
    {
        ModotSequenceItem& item = m_vecItems[index];
        if (color)
            *color = 0xFFAA8080; // same color for everyone, return color based on type
        if (start)
            *start = &item.FrameStart;
        if (end)
            *end = &item.FrameEnd;
        if (type)
            *type = item.Type;
    }
    void ModotSequencer::DoubleClick(int index)
    {
        if (m_vecItems[index].Expanded)
        {
            m_vecItems[index].Expanded = false;
            return;
        }
        for (auto& item : m_vecItems)
            item.Expanded = false;
        m_vecItems[index].Expanded = !m_vecItems[index].Expanded;
    }

    void ModotSequencer::CustomDraw(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& legendRect, const ImRect& clippingRect, const ImRect& legendClippingRect)
    {
        {
            static const char* labels[] = { "Translation", "Rotation" , "Scale" };

            rampEdit.mMax = ImVec2(float(m_FrameMax), 1.f);
            rampEdit.mMin = ImVec2(float(m_FrameMin), 0.f);
            draw_list->PushClipRect(legendClippingRect.Min, legendClippingRect.Max, true);
            for (int i = 0; i < 3; i++)
            {
                ImVec2 pta(legendRect.Min.x + 30, legendRect.Min.y + i * 14.f);
                ImVec2 ptb(legendRect.Max.x, legendRect.Min.y + (i + 1) * 14.f);
                draw_list->AddText(pta, rampEdit.mbVisible[i] ? 0xFFFFFFFF : 0x80FFFFFF, labels[i]);
                if (ImRect(pta, ptb).Contains(ImGui::GetMousePos()) && ImGui::IsMouseClicked(0))
                    rampEdit.mbVisible[i] = !rampEdit.mbVisible[i];
            }
            draw_list->PopClipRect();

            ImGui::SetCursorScreenPos(rc.Min);
            ImCurveEdit::Edit(rampEdit, ImVec2(rc.Max.x - rc.Min.x, rc.Max.y - rc.Min.y), 137 + index, &clippingRect);
        }
    }
    void ModotSequencer::CustomDrawCompact(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& clippingRect)
    {
        {
            rampEdit.mMax = ImVec2(float(m_FrameMax), 1.f);
            rampEdit.mMin = ImVec2(float(m_FrameMin), 0.f);
            draw_list->PushClipRect(clippingRect.Min, clippingRect.Max, true);
            for (int i = 0; i < 3; i++)
            {
                for (unsigned int j = 0; j < rampEdit.mPointCount[i]; j++)
                {
                    float p = rampEdit.mPts[i][j].x;
                    if (p < m_vecItems[index].FrameStart || p > m_vecItems[index].FrameEnd)
                        continue;
                    float r = (p - m_FrameMin) / float(m_FrameMax - m_FrameMin);
                    float x = ImLerp(rc.Min.x, rc.Max.x, r);
                    draw_list->AddLine(ImVec2(x, rc.Min.y + 6), ImVec2(x, rc.Max.y - 4), 0xAA000000, 4.f);
                }
            }
            draw_list->PopClipRect();
        }
    }
}