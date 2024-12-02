
#ifndef MODOT_SEQUENCER_H
#define MODOT_SEQUENCER_H

#include <ImGui/imgui.h>
#include <vector>

typedef int ModotSequencerFlags;
typedef int ModotSequencerCol;
typedef int ModotTimelineFlags;
typedef int ModotTimelineIsSelectedFlags;

// Flags for ImGui::BeginModotSequencer()
enum ModotSequencerFlags_
{
    ModotSequencerFlags_None                 = 0     ,
    ModotSequencerFlags_AllowLengthChanging  = 1 << 0, // Allows changing length of sequence
    ModotSequencerFlags_EnableSelection      = 1 << 1, // Enables selection of keyframes
    //ModotSequencerFlags_HideZoom             = 1 << 2, // Disables zoom bar
    //ModotSequencerFlags_PH                 = 1 << 3, // PLACEHOLDER
    ModotSequencerFlags_AlwaysShowHeader     = 1 << 4, // Enables overlay header, keeping it visible when scrolling

    // Selection options, only work with enable selection flag
    ModotSequencerFlags_Selection_EnableDragging = 1 << 5,
    ModotSequencerFlags_Selection_EnableDeletion = 1 << 6,

};

// Flags for ImGui::BeginModotTimeline()
enum ModotTimelineFlags_
{
    ModotTimelineFlags_None                 = 0     ,
    ModotTimelineFlags_AllowFrameChanging   = 1 << 0,
    ModotTimelineFlags_Group                = 1 << 1,
};

// Flags for ImGui::IsModotTimelineSelected()
enum ModotTimelineIsSelectedFlags_
{
    ModotTimelineIsSelectedFlags_None            = 0     ,
    ModotTimelineIsSelectedFlags_NewlySelected   = 1 << 0,
};

enum ModotSequencerCol_
{
    ModotSequencerCol_Bg,
    ModotSequencerCol_TopBarBg,
    ModotSequencerCol_SelectedTimeline,
    ModotSequencerCol_TimelineBorder,
    ModotSequencerCol_TimelinesBg,
    ModotSequencerCol_FramePointer,
    ModotSequencerCol_FramePointerHovered,
    ModotSequencerCol_FramePointerPressed,
    ModotSequencerCol_Keyframe,
    ModotSequencerCol_KeyframeHovered,
    ModotSequencerCol_KeyframePressed,
    ModotSequencerCol_KeyframeSelected,
    ModotSequencerCol_FramePointerLine,

    ModotSequencerCol_ZoomBarBg,
    ModotSequencerCol_ZoomBarSlider,
    ModotSequencerCol_ZoomBarSliderHovered,
    ModotSequencerCol_ZoomBarSliderEnds,
    ModotSequencerCol_ZoomBarSliderEndsHovered,

    ModotSequencerCol_SelectionBorder,
    ModotSequencerCol_Selection,

    ModotSequencerCol_COUNT
};

struct ModotSequencerStyle {
    float       SequencerRounding       = 0.f;                 // Corner rounding around whole sequencer
    float       TopBarHeight            = 3.f;                 // Value <= 0.0f = Height is calculated by FontSize + FramePadding.y * 2.0f
    bool        TopBarShowFrameLines    = true;                 // Show line for every frame in top bar
    bool        TopBarShowFrameTexts    = true;                 // Show frame number every 10th frame
    ImVec2      ItemSpacing             = {4.0f,0.5f};
    float       DepthItemSpacing        = 10.0f;                // Amount of text offset per depth level in timeline values
    float       TopBarSpacing           = 3.0f;                 // Space between top bar and timeline
    float       TimelineBorderSize      = 2.0f;
    float       CurrentFramePointerSize = 4.0f;                 // Size of pointing arrow above current frame line
    float       CurrentFrameLineWidth   = 1.0f;                 // Width of line showing current frame over timeline
    float       ZoomHeightScale         = 1.0f;                 // Scale of Zoom bar, base height is font size
    float       CollidedKeyframeOffset  = 3.5f;                 // Offset on which colliding keyframes are rendered

    float       MaxSizePerTick          = 3.0f;                 // Maximum amount of pixels per tick on timeline (if less pixels is present, ticks are not rendered)

    ImVec4      Colors[ModotSequencerCol_COUNT];

    ImGuiKey    ModRemoveKey            = ImGuiMod_Ctrl;        // Key mod which when held removes selected keyframes from present selection
    ImGuiKey    ModAddKey               = ImGuiMod_Shift;       // Key mod which when held adds selected keyframes to present selection

    ModotSequencerStyle();
};

namespace Modot {
    typedef int32_t FrameIndexType;

    IMGUI_API const ImVec4& GetStyleSequencerColorVec4(ModotSequencerCol idx);
    IMGUI_API ModotSequencerStyle& GetSequencerStyle();

    IMGUI_API void PushSequencerStyleColor(ModotSequencerCol idx, ImU32 col);
    IMGUI_API void PushSequencerStyleColor(ModotSequencerCol idx, const ImVec4& col);
    IMGUI_API void PopSequencerStyleColor(int count = 1);

    IMGUI_API bool BeginSequencer(const char* id, FrameIndexType * frame, FrameIndexType * startFrame, FrameIndexType * endFrame,const ImVec2& size = ImVec2(0, 0),ModotSequencerFlags flags = ModotSequencerFlags_None);
    IMGUI_API void EndSequencer(); //Call only when BeginModotSequencer() returns true!!

    IMGUI_API bool BeginGroup(const char* label, bool* open = nullptr);
    IMGUI_API void EndGroup();

    IMGUI_API bool BeginTimeline(const char* label, FrameIndexType** keyframes, uint32_t keyframeCount, bool * open = nullptr, ModotTimelineFlags flags = ModotTimelineFlags_None);
    IMGUI_API void EndTimeLine(); //Call only when BeginModotTimeline() returns true!!

    // Fully customizable timeline with per key callback
    IMGUI_API bool BeginTimelineEx(const char* label, bool * open = nullptr, ModotTimelineFlags flags = ModotTimelineFlags_None);
    IMGUI_API void ModotKeyframe(int32_t* value);

    IMGUI_API bool IsModotKeyframeHovered();
    IMGUI_API bool IsModotKeyframeSelected();
    IMGUI_API bool IsModotKeyframeRightClicked();

    // Selection API
    // DON'T delete keyframes while dragging, internal buffer will get corrupted
    // Order for deletion is generally:
    // CanDelete? -> DataSize? -> GetData() -> Delete your data -> ClearSelection()
    IMGUI_API void ClearSelection(); // Clears selection
    IMGUI_API bool IsSelecting(); // Are we currently selecting?
    IMGUI_API bool HasSelection(); // Is anything selected?
    IMGUI_API bool IsDraggingSelection(); // Are we dragging selection?
    IMGUI_API bool CanDeleteSelection(); // Can selection deletion be done?
    IMGUI_API bool IsModotKeyframeSelectionRightClicked(); // Is selection rightclicked?
    // Call only in BeginModotTimeline / EndModotTimeLine scope, returns selection per timeline and size per timeline
    IMGUI_API uint32_t GetKeyframeSelectionSize();
    IMGUI_API void GetModotKeyframeSelection(FrameIndexType  * selection);

    // Sets currently selected timeline inside BeginModotSequencer scope
    IMGUI_API void SetSelectedTimeline(const char* timelineLabel);

    IMGUI_API bool IsTimelineSelected(ModotTimelineIsSelectedFlags flags = ModotTimelineIsSelectedFlags_None);

#ifdef __cplusplus
    // C++ helper
    IMGUI_API bool BeginTimeline(const char* label,std::vector<int32_t> & keyframes ,bool * open = nullptr, ModotTimelineFlags flags = ModotTimelineFlags_None);
#endif
}


#endif
