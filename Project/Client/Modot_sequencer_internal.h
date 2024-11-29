
#ifndef MODOT_SEQUENCER_INTERNAL_H
#define MODOT_SEQUENCER_INTERNAL_H

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <cstdint>

namespace Modot {
    IMGUI_API void  RenderModotSequencerBackground(const ImVec4& color, const ImVec2 & cursor, const ImVec2& size, ImDrawList * drawList = nullptr, float sequencerRounding = 0.0f);
    IMGUI_API void  RenderModotSequencerTopBarBackground(const ImVec4& color, const ImVec2 & cursor, const ImVec2& size, ImDrawList * drawList = nullptr, float sequencerRounding = 0.0f);
    IMGUI_API void  RenderModotSequencerTopBarOverlay(float zoom, float valuesWidth,uint32_t startFrame, uint32_t endFrame, uint32_t offsetFrame, const ImVec2 &cursor, const ImVec2& size, ImDrawList * drawList = nullptr, bool drawFrameLines = true, bool drawFrameText = true, float maxPixelsPerTick = -1.0f);
    IMGUI_API void  RenderModotTimelineLabel(const char * label,const ImVec2 & cursor,const ImVec2 & size, const ImVec4& color,bool isGroup = false, bool isOpen = false, ImDrawList *drawList = nullptr );
    IMGUI_API void  RenderModotTimelane(bool selected,const ImVec2 & cursor, const ImVec2& size, const ImVec4& highlightColor, ImDrawList *drawList = nullptr);
    IMGUI_API void  RenderModotTimelinesBorder(const ImVec4& color, const ImVec2 & cursor, const ImVec2& size, ImDrawList * drawList = nullptr, float rounding = 0.0f, float borderSize = 1.0f);
    IMGUI_API void  RenderModotSequencerCurrentFrame(const ImVec4& color,const ImVec4 & topColor,const ImRect & pointerBB ,float timelineHeight, float lineWidth = 1.0f, ImDrawList * drawList = nullptr);

    IMGUI_API float GetPerFrameWidth(float totalSizeX, float valuesWidth, uint32_t endFrame, uint32_t startFrame, float zoom);
}

#endif //IMGUI_NEO_INTERNAL_H
