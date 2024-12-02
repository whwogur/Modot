
#define IMGUI_DEFINE_MATH_OPERATORS

#include "Modot_sequencer.h"
#include <ImGui/imgui_internal.h>
#include "Modot_sequencer_internal.h"

#include <unordered_map>

namespace Modot
{
    using namespace ImGui;
    // Internal state, used for deletion of old keyframes.
    struct ModotTimelineKeyframes
    {
        ImGuiID TimelineID;
        ImVector<int32_t> KeyframesToDelete;
    };

    // Internal struct holding how many times was keyframe on certain frame rendered, used as offset for duplicates
    struct ModotKeyframeDuplicate
    {
        int32_t Frame;
        uint32_t Count;
    };

    enum class SelectionState
    {
        Idle, // Doing nothing related
        Selecting,  // Selecting selection
        Dragging    // Dragging selection
    };

    struct ModotSequencerInternalData
    {
        ImVec2 TopLeftCursor            = {0, 0};   // Cursor on top of whole widget
        ImVec2 TopBarStartCursor        = {0, 0};   // Cursor on top, below Zoom slider
        ImVec2 StartValuesCursor        = {0, 0};   // Cursor on top of values
        ImVec2 ValuesCursor             = {0, 0};   // Current cursor position, used for values drawing

        ImVec2 Size                     = {0, 0};   // Size of whole sequencer
        ImVec2 TopBarSize               = {0, 0};   // Size of top bar without Zoom

        FrameIndexType StartFrame       = 0;
        FrameIndexType EndFrame         = 0;
        FrameIndexType OffsetFrame      = 0;        // Offset from start

        float ValuesWidth               = 32.0f;    // Width of biggest label in timeline, used for offset of timeline

        float FilledHeight              = 100.0f;     // Height of whole sequencer

        float Zoom = 1.0f;

        ImGuiID Id;

        ImGuiID LastSelectedTimeline    = 0;
        ImGuiID SelectedTimeline        = 0;
        bool LastTimelineOpenned        = false;

        ImVector<ImGuiID> TimelineStack;
        ImVector<ImGuiID> GroupStack;

        FrameIndexType CurrentFrame     = 0;
        bool HoldingCurrentFrame        = false;    // draging current frame?
        ImVec4 CurrentFrameColor;                   // Color of current frame, we have to save it because we render on EndModotSequencer, but process at BeginneoSequencer

        bool HoldingZoomSlider          = false;

        //Selection
        ImVector<ImGuiID> Selection;    // Contains ids of keyframes
        ImVec2 SelectionMouseStart      = {0, 0};
        SelectionState StateOfSelection = SelectionState::Idle;
        ImVec2 DraggingMouseStart       = {0, 0};
        bool StartDragging              = true;
        ImVector<int32_t> DraggingSelectionStart; // Contains start values of all selection elements
        bool DraggingEnabled            = true;
        bool SelectionEnabled           = true;
        bool IsSelectionRightClicked    = false;

        //Last keyframe data
        bool IsLastKeyframeHovered      = false;
        bool IsLastKeyframeSelected     = false;
        bool IsLastKeyframeRightClicked = false;

        //Deletion
        bool DeleteDataDirty            = false;
        bool DeleteEnabled              = true;
        ImVector<ModotTimelineKeyframes> SelectionData;
    };

    static ModotSequencerStyle style;

    //Global context stuff
    static bool inSequencer = false;

    // Height of timeline right now
    static float currentTimelineHeight = 0.0f;

    // Current active sequencer
    static ImGuiID currentSequencer;

    // Current timeline depth, used for offset of label
    static uint32_t currentTimelineDepth = 0;

    static ImVector<ImGuiColorMod> sequencerColorStack;

    // Data of all sequencers, this is main c++ part and I should create C alternative or use imgui ImVector or something
    static std::unordered_map<ImGuiID, ModotSequencerInternalData> sequencerData;

    static ImVector<ModotKeyframeDuplicate> keyframeDuplicates;

    ///////////// STATIC HELPERS ///////////////////////

    static float getPerFrameWidth(ModotSequencerInternalData& context)
    {
        return GetPerFrameWidth(context.Size.x, context.ValuesWidth, context.EndFrame, context.StartFrame,
                                context.Zoom);
    }

    static float getKeyframePositionX(FrameIndexType frame, ModotSequencerInternalData& context)
    {
        const auto perFrameWidth = getPerFrameWidth(context);
        return (float) (frame - context.OffsetFrame - context.StartFrame) * perFrameWidth;
    }

    static float getWorkTimelineWidth(ModotSequencerInternalData& context)
    {
        const auto perFrameWidth = getPerFrameWidth(context);
        return context.Size.x - context.ValuesWidth - perFrameWidth;
    }

    // Dont pull frame from context, its used for dragging
    static ImRect getCurrentFrameBB(FrameIndexType frame, ModotSequencerInternalData& context)
    {
        const auto& imStyle = GetStyle();
        const auto width = style.CurrentFramePointerSize * GetIO().FontGlobalScale;
        const auto cursor =
                context.TopBarStartCursor + ImVec2{context.ValuesWidth + imStyle.FramePadding.x - width / 2.0f, 0};
        const auto currentFrameCursor = cursor + ImVec2{getKeyframePositionX(frame, context), 0};

        float pointerHeight = style.CurrentFramePointerSize * 2.5f;
        ImRect rect{currentFrameCursor, currentFrameCursor + ImVec2{width, pointerHeight * GetIO().FontGlobalScale}};

        return rect;
    }

    static void processCurrentFrame(FrameIndexType* frame, ModotSequencerInternalData& context)
    {
        auto pointerRect = getCurrentFrameBB(*frame, context);
        pointerRect.Min -= ImVec2{2.0f, 2.0f};
        pointerRect.Max += ImVec2{2.0f, 2.0f};

        const auto& imStyle = GetStyle();

        const auto timelineXmin = context.TopBarStartCursor.x + context.ValuesWidth + imStyle.FramePadding.x;

        const ImVec2 timelineXRange = {
                timelineXmin, //min
                timelineXmin + context.Size.x - context.ValuesWidth
        };

        const auto hovered = ItemHoverable(pointerRect, GetCurrentWindow()->GetID("##_top_selector_neo"),0);

        context.CurrentFrameColor = GetStyleSequencerColorVec4(ModotSequencerCol_FramePointer);

        if (hovered)
        {
            context.CurrentFrameColor = GetStyleSequencerColorVec4(ModotSequencerCol_FramePointerHovered);
        }

        if (context.HoldingCurrentFrame)
        {
            if (IsMouseDragging(ImGuiMouseButton_Left, 0.0f))
            {
                const auto mousePosX = GetMousePos().x;
                const auto v = mousePosX - timelineXRange.x;// Subtract min

                const auto normalized = v / getWorkTimelineWidth(context); //Divide by width to remap to 0 - 1 range

                const auto clamped = ImClamp(normalized, 0.0f, 1.0f);

                const auto viewSize = (float) (context.EndFrame - context.StartFrame) / context.Zoom;

                const auto frameViewVal = (float) context.StartFrame + (clamped * (float) viewSize);

                const auto finalFrame = (FrameIndexType) round(frameViewVal) + context.OffsetFrame;

                context.CurrentFrameColor = GetStyleSequencerColorVec4(ModotSequencerCol_FramePointerPressed);

                *frame = finalFrame;
            }

            if (!IsMouseDown(ImGuiMouseButton_Left))
            {
                context.HoldingCurrentFrame = false;
                context.CurrentFrameColor = GetStyleSequencerColorVec4(ModotSequencerCol_FramePointer);
            }
        }

        if (hovered && IsMouseDown(ImGuiMouseButton_Left) && !context.HoldingCurrentFrame)
        {
            context.HoldingCurrentFrame = true;
            context.CurrentFrameColor = GetStyleSequencerColorVec4(ModotSequencerCol_FramePointerPressed);
        }

        context.CurrentFrame = *frame;
    }

    static void finishPreviousTimeline(ModotSequencerInternalData& context)
    {
        context.ValuesCursor = {context.TopBarStartCursor.x, context.ValuesCursor.y};
        currentTimelineHeight = 0.0f;
    }

    static ImColor getKeyframeColor(ModotSequencerInternalData& context, bool hovered, bool inSelection)
    {
        if (inSelection)
        {
            return ColorConvertFloat4ToU32(GetStyleSequencerColorVec4(ModotSequencerCol_KeyframeSelected));
        }

        return hovered ?
               ColorConvertFloat4ToU32(
                       GetStyleSequencerColorVec4(ModotSequencerCol_KeyframeHovered)) :
               ColorConvertFloat4ToU32(GetStyleSequencerColorVec4(ModotSequencerCol_Keyframe));
    }

    static void addKeyframeToDeleteData(int32_t value, ModotSequencerInternalData& context, const ImGuiID timelineId)
    {
        bool foundTimeline = false;
        for (auto&& val: context.SelectionData)
        {
            if (val.TimelineID == timelineId)
            {
                foundTimeline = true;
                if (!val.KeyframesToDelete.contains(value))
                    val.KeyframesToDelete.push_back(value);
                break;
            }
        }

        if (!foundTimeline)
        {
            context.SelectionData.push_back({});
            auto& data = context.SelectionData.back();
            data.TimelineID = timelineId;
            data.KeyframesToDelete.push_back(value);
        }
    }

    static bool
    getKeyframeInSelection(int32_t value, ImGuiID id, ModotSequencerInternalData& context, const ImRect bb)
    {
        //TODO(matej.vrba): This is kinda slow, it works for smaller data sample, but for bigger sample it should be changed to hashset
        const ImGuiID timelineId = context.TimelineStack.back();

        if (context.DeleteDataDirty && context.Selection.contains(id))
        {
            addKeyframeToDeleteData(value, context, timelineId);
        }

        if (context.StateOfSelection != SelectionState::Selecting)
        {
            return context.Selection.contains(id);
        }

        ImRect sel = {context.SelectionMouseStart, GetMousePos()};

        if (sel.Min.y > sel.Max.y)
        {
            ImVec2 tmp = sel.Min;
            sel.Min = sel.Max;
            sel.Max = tmp;
        }

        if (sel.Min.x > sel.Max.x)
        {
            float tmp = sel.Min.x;
            sel.Min.x = sel.Max.x;
            sel.Max.x = tmp;
        }

        const bool overlaps = bb.Overlaps(sel);

        const bool forceRemove = IsKeyDown(style.ModRemoveKey);
        const bool forceAdd = IsKeyDown(style.ModAddKey);


        auto removeKeyframe = [&]()
        {
            for (auto&& val: context.SelectionData)
            {
                if (val.TimelineID == timelineId)
                {
                    val.KeyframesToDelete.find_erase(value);
                    break;
                }
            }
            context.Selection.find_erase(id);
        };

        if (overlaps)
        {
            if (forceRemove)
            {
                removeKeyframe();
                return context.Selection.contains(id);
            } else
            {
                if (!context.Selection.contains(id))
                {
                    addKeyframeToDeleteData(value, context, timelineId);

                    context.Selection.push_back(id);
                }
            }
        } else
        {
            if (!forceRemove && !forceAdd)
            {
                removeKeyframe();
            } else
            {
                return context.Selection.contains(id);
            }
        }
        return overlaps;
    }

    static ImGuiID getKeyframeID(int32_t* frame)
    {
        return GetCurrentWindow()->GetID(frame);
    }

    static bool createKeyframe(int32_t* frame)
    {
        const auto& imStyle = GetStyle();
        auto& context = sequencerData[currentSequencer];

        const auto timelineOffset = getKeyframePositionX(*frame, context);

        float offset = 0.0f;

        for (auto&& duplicateData: keyframeDuplicates)
        {
            if (duplicateData.Frame == *frame)
            {
                offset = (float) duplicateData.Count * style.CollidedKeyframeOffset;
                duplicateData.Count++;
            }
        }

        if (offset < style.CollidedKeyframeOffset)
        {
            keyframeDuplicates.push_back({});
            keyframeDuplicates.back().Frame = *frame;
            keyframeDuplicates.back().Count = 1;
        }

        const auto pos = ImVec2{context.StartValuesCursor.x + imStyle.FramePadding.x, context.ValuesCursor.y} +
                         ImVec2{timelineOffset + context.ValuesWidth + offset, 0};

        const auto bbPos = pos - ImVec2{currentTimelineHeight / 2, 0};

        const ImRect bb = {bbPos, bbPos + ImVec2{currentTimelineHeight, currentTimelineHeight}};

        const auto drawList = ImGui::GetWindowDrawList();

        const ImGuiID id = getKeyframeID(frame);

        bool hovered = ItemHoverable(bb, id, ImGuiItemFlags_None);

        if (context.SelectionEnabled && context.Selection.contains(id) &&
            (context.StateOfSelection != SelectionState::Selecting))
        {
            // process dragging
            if (bb.Contains(GetMousePos()) && IsMouseClicked(ImGuiMouseButton_Left) &&
                context.StateOfSelection != SelectionState::Dragging &&
                context.DraggingEnabled)
            {
                //Start dragging
                context.StartDragging = true;
            }

            if (context.StateOfSelection == SelectionState::Dragging)
            {
                ImGuiID* it = context.Selection.find(id);
                int32_t index = context.Selection.index_from_ptr(it);

                if (context.DraggingSelectionStart.size() < index + 1 || context.DraggingSelectionStart[index] == -1)
                {
                    if (context.DraggingSelectionStart.size() < index + 1)
                    {
                        context.DraggingSelectionStart.resize(index + 1, -1);
                    }

                    context.DraggingSelectionStart[index] = *frame;
                }
                float mouseDelta = GetMousePos().x - context.DraggingMouseStart.x;

                auto offsetA = int32_t(
                        mouseDelta / (context.Size.x / (float) context.EndFrame - (float) context.StartFrame));

                *frame = context.DraggingSelectionStart[index] + offsetA;
            }
        }

        const bool inSelection = getKeyframeInSelection(*frame, id, context, bb);

        context.IsLastKeyframeSelected = inSelection;

        if (timelineOffset >= 0.0f)
        {

            ImColor color = getKeyframeColor(context, hovered, inSelection);
            float rectWaist = currentTimelineHeight / 2.f;

            float centerX = pos.x;
            float centerY = pos.y + rectWaist;

            ImVec2 p_min(centerX - rectWaist * 0.25f, centerY - rectWaist);
            ImVec2 p_max(centerX + rectWaist * 0.25f, centerY + rectWaist);

            drawList->AddRectFilled(p_min, p_max, color, 0.0f);
        }

        context.IsLastKeyframeHovered = hovered;
        context.IsLastKeyframeRightClicked = hovered && IsMouseClicked(ImGuiMouseButton_Right);

        if (context.Selection.contains(id) && context.IsLastKeyframeRightClicked)
        {
            context.IsSelectionRightClicked = true;
        }

        return true;
    }

    static uint32_t idCounter = 0;
    static char idBuffer[16];

    const char* generateID()
    {
        idBuffer[0] = '#';
        idBuffer[1] = '#';
        memset(idBuffer + 2, 0, 14);
        snprintf(idBuffer + 2, 14, "%o", idCounter++);

        return &idBuffer[0];
    }

    void resetID()
    {
        idCounter = 0;
    }

    static void renderCurrentFrame(ModotSequencerInternalData& context)
    {
        const auto bb = getCurrentFrameBB(context.CurrentFrame, context);

        const auto drawList = ImGui::GetWindowDrawList();

        RenderModotSequencerCurrentFrame(
                GetStyleSequencerColorVec4(ModotSequencerCol_FramePointerLine),
                context.CurrentFrameColor,
                bb,
                context.Size.y - context.TopBarSize.y,
                style.CurrentFrameLineWidth,
                drawList
        );
    }

    static float calculateZoomBarHeight()
    {
        const auto& imStyle = GetStyle();
        return GetFontSize() * style.ZoomHeightScale + imStyle.FramePadding.y * 2.0f;
    }

    static void
    processAndRenderZoom(ModotSequencerInternalData& context, const ImVec2& cursor, bool allowEditingLength,
                         FrameIndexType* start,
                         FrameIndexType* end)
    {
        const auto& imStyle = GetStyle();
        ImGuiWindow* window = GetCurrentWindow();

        const auto zoomHeight = calculateZoomBarHeight();

        auto* drawList = GetWindowDrawList();

        //Input width
        const auto inputWidth = CalcTextSize("123456").x;

        const auto inputWidthWithPadding = inputWidth + imStyle.ItemSpacing.x;

        const auto cursorV = allowEditingLength ? cursor + ImVec2{inputWidthWithPadding, 0}
                                                : cursor;

        const auto size = allowEditingLength ?
                          context.Size.x - 2 * inputWidthWithPadding :
                          context.Size.x;

        const ImRect bb{cursorV, cursorV + ImVec2{size, zoomHeight}};


        const auto zoomBarEndWithSpacing = ImVec2{bb.Max.x + imStyle.ItemSpacing.x, bb.Min.y};

        FrameIndexType startFrameVal = *start;
        FrameIndexType endFrameVal = *end;

        if (allowEditingLength)
        {
            const float sideOffset = imStyle.ItemSpacing.x / 2.0f;
            auto prevWindowCursor = window->DC.CursorPos;

            window->DC.CursorPos = cursor;

            window->DC.CursorPos.x += sideOffset;

            PushItemWidth(inputWidth);
            InputScalar("##input_start_frame", ImGuiDataType_U32, &startFrameVal, NULL, NULL, "%i",
                        allowEditingLength ? 0 : ImGuiInputTextFlags_ReadOnly);

            window->DC.CursorPos = ImVec2{zoomBarEndWithSpacing.x, cursor.y};
            window->DC.CursorPos.x -= sideOffset;

            PushItemWidth(inputWidth);
            InputScalar("##input_end_frame", ImGuiDataType_U32, &endFrameVal, NULL, NULL, "%i",
                        allowEditingLength ? 0 : ImGuiInputTextFlags_ReadOnly);

            window->DC.CursorPos = prevWindowCursor;
        }


        if (endFrameVal <= startFrameVal)
            endFrameVal = (int32_t) *end;

        *start = startFrameVal;
        *end = endFrameVal;

        //Background
        drawList->AddRectFilled(bb.Min, bb.Max,
                                ColorConvertFloat4ToU32(GetStyleSequencerColorVec4(ModotSequencerCol_ZoomBarBg)),
                                10.0f);

        const auto baseWidth = bb.GetSize().x -
                               imStyle.ItemInnerSpacing.x; //There is just half spacing applied, doing it normally makes big gap on sides

        const auto sliderHeight = bb.GetSize().y - imStyle.ItemInnerSpacing.y;

        const auto sliderWidth = baseWidth / context.Zoom;

        const auto sliderMin = bb.Min + imStyle.ItemInnerSpacing / 2.0f;

        //const auto sliderMax = bb.Max - imStyle.ItemInnerSpacing / 2.0f;

        const auto sliderMaxWidth = baseWidth;

        const auto totalFrames = (*end - *start);

        const auto singleFrameWidthOffset = sliderMaxWidth / (float) totalFrames;

        const auto zoomSliderOffset = singleFrameWidthOffset * (float) context.OffsetFrame;

        const auto sliderStart = sliderMin + ImVec2{zoomSliderOffset, 0};

        const float sideSize = sliderHeight;

        const ImRect finalSliderBB{sliderStart, sliderStart + ImVec2{sliderWidth, sliderHeight}};

        const ImRect finalSliderInteractBB = {finalSliderBB.Min + ImVec2{sideSize, 0},
                                              finalSliderBB.Max - ImVec2{sideSize, 0}};


        const auto viewWidth = (uint32_t) ((float) totalFrames / context.Zoom);

        const bool hovered = ItemHoverable(bb, GetCurrentWindow()->GetID("##zoom_slider"), 0);

        if (hovered)
        {
            SetKeyOwner(ImGuiKey_MouseWheelY, GetItemID());
            const float currentScroll = GetIO().MouseWheel;

            context.Zoom = ImClamp(context.Zoom + float(currentScroll) * 0.3f, 1.0f, (float) viewWidth);
            const auto newZoomWidth = (FrameIndexType) ceil((float) totalFrames / (context.Zoom));

            if (*start + context.OffsetFrame + newZoomWidth > *end)
                context.OffsetFrame = ImMax(0U, totalFrames - viewWidth);
        }

        if (context.HoldingZoomSlider)
        {
            if (IsMouseDragging(ImGuiMouseButton_Left, 0.01f))
            {
                const auto currentX = GetMousePos().x;

                const auto v = currentX - bb.Min.x;// Subtract min

                const auto normalized = v / bb.GetWidth(); //Divide by width to remap to 0 - 1 range

                const auto sliderWidthNormalized = 1.0f / context.Zoom;

                const auto singleFrameWidthOffsetNormalized = singleFrameWidthOffset / bb.GetWidth();

                FrameIndexType finalFrame = (FrameIndexType) ((float) (normalized - sliderWidthNormalized / 2.0f) /
                                                              singleFrameWidthOffsetNormalized);

                if (normalized - sliderWidthNormalized / 2.0f < 0.0f)
                {
                    finalFrame = 0;
                }


                if (normalized + sliderWidthNormalized / 2.0f > 1.0f)
                {
                    finalFrame = totalFrames - viewWidth;
                }


                context.OffsetFrame = finalFrame;
            }

            if (!IsMouseDown(ImGuiMouseButton_Left))
            {
                context.HoldingZoomSlider = false;
            }
        }

        if (hovered && IsMouseDown(ImGuiMouseButton_Left))
        {
            context.HoldingZoomSlider = true;
        }


        const auto res = ItemAdd(finalSliderInteractBB, 0);


        const auto viewStart = *start + context.OffsetFrame;
        const auto viewEnd = viewStart + viewWidth;

        if (res)
        {
            auto sliderColor = GetStyleSequencerColorVec4(ModotSequencerCol_ZoomBarSlider);

            if (IsItemHovered())
            {
                sliderColor = GetStyleSequencerColorVec4(ModotSequencerCol_ZoomBarSliderHovered);
            }

            //Render bar
            drawList->AddRectFilled(finalSliderBB.Min, finalSliderBB.Max, ColorConvertFloat4ToU32(sliderColor), 10.0f);

            const auto sliderCenter = finalSliderBB.GetCenter();

            char overlayTextBuffer[128];

            snprintf(overlayTextBuffer, sizeof(overlayTextBuffer), "%i - %i", viewStart, viewEnd);

            const auto overlaySize = CalcTextSize(overlayTextBuffer);

            drawList->AddText(sliderCenter - overlaySize / 2.0f, IM_COL32_WHITE, overlayTextBuffer);
        }
    }

    static void processZoom(ModotSequencerInternalData& context, const ImVec2& cursor,
            FrameIndexType* start,
            FrameIndexType* end)
    {
        const auto& imStyle = GetStyle();
        ImGuiWindow* window = GetCurrentWindow();


        FrameIndexType startFrameVal = *start;
        FrameIndexType endFrameVal = *end;

        if (endFrameVal <= startFrameVal)
            endFrameVal = (int32_t)*end;

        *start = startFrameVal;
        *end = endFrameVal;

        const auto totalFrames = (*end - *start);

        const auto viewWidth = (uint32_t)((float)totalFrames / context.Zoom);

        SetKeyOwner(ImGuiKey_MouseWheelY, GetItemID());
        const float currentScroll = GetIO().MouseWheel;

        context.Zoom = ImClamp(context.Zoom + float(currentScroll) * 0.3f, 1.0f, (float)viewWidth);
        const auto newZoomWidth = (FrameIndexType)ceil((float)totalFrames / (context.Zoom));

        if (*start + context.OffsetFrame + newZoomWidth > *end)
            context.OffsetFrame = ImMax(0U, totalFrames - viewWidth);
    }

    static void processSelection(ModotSequencerInternalData& context)
    {
        context.DeleteDataDirty = false;

        if (context.StartDragging)
        {
            context.StateOfSelection = SelectionState::Dragging;
            context.DraggingMouseStart = GetMousePos();
            context.StartDragging = false;
            return;
        }

        const auto windowWorkRect = GetCurrentWindow()->ClipRect;

        const auto sequencerWorkRect = ImRect{
                context.TopBarStartCursor + ImVec2{context.ValuesWidth, 10.f},
                context.TopBarStartCursor + context.Size - ImVec2{0, 10.f}};

        if (IsMouseDown(ImGuiMouseButton_Left) && windowWorkRect.Contains(GetMousePos()) &&
            sequencerWorkRect.Contains(GetMousePos()))
        {
            // Not dragging yet
            switch (context.StateOfSelection)
            {
                case SelectionState::Idle:
                {
                    if (!IsMouseClicked(ImGuiMouseButton_Left)) return;

                    context.SelectionMouseStart = GetMousePos();
                    context.StateOfSelection = SelectionState::Selecting;
                    break;
                }
                case SelectionState::Selecting:
                {
                    break;
                }
                case SelectionState::Dragging:
                {

                    break;
                }
            }
        }
        else
        {
            switch (context.StateOfSelection)
            {
                case SelectionState::Idle:
                {
                    break;
                }
                case SelectionState::Selecting:
                {
                    context.SelectionMouseStart = {0, 0};
                    context.StateOfSelection = SelectionState::Idle;
                    break;
                }
                case SelectionState::Dragging:
                {
                    context.DraggingSelectionStart.resize(0);
                    context.StateOfSelection = SelectionState::Idle;
                    context.DraggingMouseStart = {0, 0};
                    context.DeleteDataDirty = true;
                    for (auto&& t: context.SelectionData)
                        t.KeyframesToDelete.resize(0);
                    break;
                }
            }
        }
    }

    static void renderSelection(ModotSequencerInternalData& context)
    {
        if (context.StateOfSelection != SelectionState::Selecting)
        {
            return;
        }
        const ImVec2 currentMousePosition = GetMousePos();

        auto* drawList = GetWindowDrawList();

        ImRect sel{context.SelectionMouseStart,
                   currentMousePosition};

        if (sel.Min.y > sel.Max.y)
        {
            ImVec2 tmp = sel.Min;
            sel.Min = sel.Max;
            sel.Max = tmp;
        }

        if (sel.Min.x > sel.Max.x)
        {
            float tmp = sel.Min.x;
            sel.Min.x = sel.Max.x;
            sel.Max.x = tmp;
        }

        if (sel.GetArea() < 32.0f)
            return;

        // Inner
        drawList->AddRectFilled(
                context.SelectionMouseStart,
                currentMousePosition,
                ColorConvertFloat4ToU32(style.Colors[ModotSequencerCol_Selection])
        );

        // border
        drawList->AddRect(
                context.SelectionMouseStart,
                currentMousePosition,
                ColorConvertFloat4ToU32(style.Colors[ModotSequencerCol_SelectionBorder]),
                0.0f,
                0,
                0.5f
        );
    }

    static bool groupBehaviour(const ImGuiID id, bool* open, const ImVec2 labelSize)
    {
        auto& context = sequencerData[currentSequencer];
        ImGuiWindow* window = GetCurrentWindow();

        const bool closable = open != nullptr;

        auto drawList = ImGui::GetWindowDrawList();
        const float arrowWidth = drawList->_Data->FontSize;
        const ImVec2 arrowSize = {arrowWidth, arrowWidth};
        const ImRect arrowBB = {
                context.ValuesCursor,
                context.ValuesCursor + arrowSize
        };
        const ImVec2 groupBBMin = {context.ValuesCursor + ImVec2{arrowSize.x, 0.0f}};
        const ImRect groupBB = {
                groupBBMin,
                groupBBMin + labelSize
        };
        const ImGuiID arrowID = window->GetID(generateID());
        const auto addArrowRes = ItemAdd(arrowBB, arrowID);
        if (addArrowRes)
        {
            if (IsItemClicked() && closable)
                (*open) = !(*open);
        }

        const auto addGroupRes = ItemAdd(groupBB, id);
        if (addGroupRes)
        {
            if (IsItemClicked())
            {
                context.LastSelectedTimeline = context.SelectedTimeline;
                context.SelectedTimeline = context.SelectedTimeline == id ? 0 : id;
            }
        }
        const float width = groupBB.Max.x - arrowBB.Min.x;
        context.ValuesWidth = std::max(context.ValuesWidth, width); // Make left panel wide enough
        return addGroupRes && addArrowRes;
    }

    static bool timelineBehaviour(const ImGuiID id, const ImVec2 labelSize)
    {
        auto& context = sequencerData[currentSequencer];
        //ImGuiWindow *window = GetCurrentWindow();

        const ImRect groupBB = {
                context.ValuesCursor,
                context.ValuesCursor + labelSize
        };

        const auto addGroupRes = ItemAdd(groupBB, id);
        if (addGroupRes)
        {
            if (IsItemClicked())
            {
                context.LastSelectedTimeline = context.SelectedTimeline;
                context.SelectedTimeline = context.SelectedTimeline == id ? 0 : id;
            }
        }
        const float width = groupBB.Max.x - groupBB.Min.x;
        context.ValuesWidth = std::max(context.ValuesWidth, width); // Make left panel wide enough

        return addGroupRes;
    }

    ////////////////////////////////////

    const ImVec4& GetStyleSequencerColorVec4(ModotSequencerCol idx)
    {
        return GetSequencerStyle().Colors[idx];
    }

    ModotSequencerStyle& GetSequencerStyle()
    {
        return style;
    }

    bool
    BeginSequencer(const char* idin, FrameIndexType* frame, FrameIndexType* startFrame, FrameIndexType* endFrame,
                      const ImVec2& size,
                      ModotSequencerFlags flags)
    {
        IM_ASSERT(!inSequencer && "Called when while in other ModotSequencer, that won't work, call End!");
        IM_ASSERT(*startFrame < *endFrame && "Start frame must be smaller than end frame");

        static char childNameStorage[64];
        snprintf(childNameStorage, sizeof(childNameStorage), "##%s_child_wrapper", idin);
        const bool openChild = BeginChild(childNameStorage);

        if (!openChild)
        {
            EndChild();
            return openChild;
        }

        //ImGuiContext &g = *GImGui;
        ImGuiWindow* window = GetCurrentWindow();
        const auto& imStyle = GetStyle();
        //auto &neoStyle = GetModotSequencerStyle();

        if (inSequencer)
            return false;

        if (window->SkipItems)
            return false;

        const auto drawList = GetWindowDrawList();

        const auto cursor = GetCursorScreenPos();
        const auto area = ImGui::GetContentRegionAvail();

        const auto cursorBasePos = GetCursorScreenPos() + window->Scroll;

        PushID(idin);
        const auto id = window->IDStack[window->IDStack.size() - 1];

        inSequencer = true;

        auto& context = sequencerData[id];
        context.Id = id;

        auto realSize = ImFloor(size);
        if (realSize.x <= 0.0f)
            realSize.x = ImMax(4.0f, area.x);
        if (realSize.y <= 0.0f)
            realSize.y = ImMax(4.0f, context.FilledHeight);

        const bool headerAlwaysVisible = (flags & ModotSequencerFlags_AlwaysShowHeader);
        context.SelectionEnabled = (flags & ModotSequencerFlags_EnableSelection);
        context.DraggingEnabled = context.SelectionEnabled && (flags & ModotSequencerFlags_Selection_EnableDragging);
        context.DeleteEnabled = context.SelectionEnabled && (flags & ModotSequencerFlags_Selection_EnableDeletion);

        context.TopLeftCursor = headerAlwaysVisible ? cursorBasePos : cursor;

        // If Zoom is shown, we offset it by height of Zoom bar + padding
        context.TopBarStartCursor = context.TopLeftCursor;
        context.StartFrame = *startFrame;
        context.EndFrame = *endFrame;
        context.Size = realSize;

        context.TopBarSize = ImVec2(context.Size.x, style.TopBarHeight);

        if (context.TopBarSize.y <= 0.0f)
            context.TopBarSize.y = CalcTextSize("100").y + imStyle.FramePadding.y * 2.0f;

        currentSequencer = window->IDStack[window->IDStack.size() - 1];

        auto backgroundSize = context.Size;
        const float topCut = abs(context.TopLeftCursor.y - cursor.y);
        backgroundSize.y = backgroundSize.y - (topCut);

        RenderModotSequencerBackground(style.Colors[ModotSequencerCol_Bg], context.TopLeftCursor,
                                     backgroundSize,
                                     drawList, style.SequencerRounding);

       RenderModotSequencerTopBarBackground(style.Colors[ModotSequencerCol_TopBarBg],
                                          context.TopBarStartCursor, context.TopBarSize,
                                          drawList, style.SequencerRounding);
       
       
       RenderModotSequencerTopBarOverlay(context.Zoom, context.ValuesWidth, context.StartFrame, context.EndFrame,
                                       context.OffsetFrame,
                                       context.TopBarStartCursor, context.TopBarSize, drawList,
                                       style.TopBarShowFrameLines, style.TopBarShowFrameTexts, style.MaxSizePerTick);

       processZoom(context, context.TopLeftCursor, startFrame, endFrame);

        if (context.Size.y < context.FilledHeight)
            context.Size.y = context.FilledHeight;

        context.FilledHeight = context.TopBarSize.y + style.TopBarSpacing;

        context.StartValuesCursor = cursor + ImVec2{0, context.TopBarSize.y + style.TopBarSpacing};
        
        context.StartValuesCursor = context.StartValuesCursor;
        context.ValuesCursor = context.StartValuesCursor;

        processCurrentFrame(frame, context);

        const auto clipMin = context.TopBarStartCursor + ImVec2(0, context.TopBarSize.y);

        drawList->PushClipRect(clipMin, clipMin + backgroundSize , true);

        return true;
    }

    void EndSequencer()
    {
        IM_ASSERT(inSequencer && "Called end sequencer when BeginSequencer didnt return true or wasn't called at all!");
        IM_ASSERT(sequencerData.count(currentSequencer) != 0 && "Ended sequencer has no context!");

        auto& context = sequencerData[currentSequencer];
        IM_ASSERT(context.TimelineStack.empty() && "Missmatch in timeline Begin / End");

        if (context.SelectionEnabled)
            processSelection(context);

        context.LastSelectedTimeline = context.SelectedTimeline;
        context.IsSelectionRightClicked = false;

        if (context.SelectionEnabled)
            renderSelection(context);

        renderCurrentFrame(context);

        inSequencer = false;

        const ImVec2 min = {0, 0};
        context.Size.y = context.FilledHeight;
        const auto max = context.Size;

        ItemSize({min, max});
        PopID();
        resetID();

        EndChild();
    }

    IMGUI_API bool BeginGroup(const char* label, bool* open)
    {
        return BeginTimeline(label, nullptr, 0, open, ModotTimelineFlags_Group);
    }

    IMGUI_API void EndGroup()
    {
        return EndTimeLine();
    }

#ifdef __cplusplus

    bool
    BeginTimeline(const char* label, std::vector<int32_t>& keyframes, bool* open, ModotTimelineFlags flags)
    {
        std::vector<int32_t*> c_keyframes{keyframes.size()};
        for (uint32_t i = 0; i < keyframes.size(); i++)
            c_keyframes[i] = &keyframes[i];

        return BeginTimeline(label, c_keyframes.data(), uint32_t(c_keyframes.size()), open, flags);
    }

#endif

    void PushSequencerStyleColor(ModotSequencerCol idx, ImU32 col)
    {
        ImGuiColorMod backup;
        backup.Col = idx;
        backup.BackupValue = style.Colors[idx];
        sequencerColorStack.push_back(backup);
        style.Colors[idx] = ColorConvertU32ToFloat4(col);
    }

    void PushSequencerStyleColor(ModotSequencerCol idx, const ImVec4& col)
    {
        ImGuiColorMod backup;
        backup.Col = idx;
        backup.BackupValue = style.Colors[idx];
        sequencerColorStack.push_back(backup);
        style.Colors[idx] = col;
    }

    void PopSequencerStyleColor(int count)
    {
        while (count > 0)
        {
            ImGuiColorMod& backup = sequencerColorStack.back();
            style.Colors[backup.Col] = backup.BackupValue;
            sequencerColorStack.pop_back();
            count--;
        }
    }

    void SetSelectedTimeline(const char* timelineLabel)
    {
        IM_ASSERT(inSequencer && "Not in active sequencer!");

        auto& context = sequencerData[currentSequencer];

        ImGuiWindow* window = GetCurrentWindow();

        ImGuiID timelineID = 0;

        if (timelineLabel)
        {
            timelineID = window->GetID(timelineLabel);
        }
        context.LastSelectedTimeline = context.SelectedTimeline;
        context.SelectedTimeline = timelineID;
    }

    bool IsTimelineSelected(ModotTimelineIsSelectedFlags flags)
    {
        IM_ASSERT(inSequencer && "Not in active sequencer!");
        auto& context = sequencerData[currentSequencer];

        IM_ASSERT(!context.TimelineStack.empty() && "No active timelines are present!");

        const bool newly = flags & ModotTimelineIsSelectedFlags_NewlySelected;

        const auto openTimeline = context.TimelineStack[context.TimelineStack.size() - 1];

        if (!newly)
        {
            return context.SelectedTimeline == openTimeline;
        }

        return (context.SelectedTimeline != context.LastSelectedTimeline) &&
               context.SelectedTimeline == openTimeline;
    }

    bool BeginTimelineEx(const char* label, bool* open, ModotTimelineFlags flags)
    {
        IM_ASSERT(inSequencer && "Not in active sequencer!");

        const bool closable = open != nullptr;

        auto& context = sequencerData[currentSequencer];
        const auto& imStyle = GetStyle();
        ImGuiWindow* window = GetCurrentWindow();
        const ImGuiID id = window->GetID(label);
        auto labelSize = CalcTextSize(label);

        labelSize.y += imStyle.FramePadding.y * 2 + style.ItemSpacing.y * 2;
        labelSize.x += imStyle.FramePadding.x * 2 + style.ItemSpacing.x * 2 +
                       (float) currentTimelineDepth * style.DepthItemSpacing;


        bool isGroup = flags & ModotTimelineFlags_Group && closable;
        bool addRes = false;
        if (isGroup)
        {
            labelSize.x += imStyle.ItemSpacing.x + GetFontSize();
            addRes = groupBehaviour(id, open, labelSize);
        } else
        {
            addRes = timelineBehaviour(id, labelSize);
        }

        if (currentTimelineDepth > 0)
        {
            context.ValuesCursor = {context.TopBarStartCursor.x, context.ValuesCursor.y};
        }

        currentTimelineHeight = labelSize.y;
        context.FilledHeight += currentTimelineHeight;
        const auto result = !closable || (*open);
        context.LastTimelineOpenned = result;

        if (addRes)
        {
            RenderModotTimelane(id == context.SelectedTimeline,
                              context.ValuesCursor + ImVec2{context.ValuesWidth, 0},
                              ImVec2{context.Size.x - context.ValuesWidth, currentTimelineHeight},
                              GetStyleSequencerColorVec4(ModotSequencerCol_SelectedTimeline));

            ImVec4 color = GetStyleColorVec4(ImGuiCol_Text);
            if (IsItemHovered()) color.w *= 0.7f;

            RenderModotTimelineLabel(label,
                                   context.ValuesCursor + imStyle.FramePadding +
                                   ImVec2{(float) currentTimelineDepth * style.DepthItemSpacing, 0},
                                   labelSize,
                                   color,
                                   isGroup,
                                   isGroup && (*open));

        }

        if (result)
            context.TimelineStack.push_back(id);

        if (isGroup)
        { // Group requires special behaviour if its closed
            context.ValuesCursor.y += currentTimelineHeight;
            if (result)
            {
                currentTimelineDepth++;
                context.GroupStack.push_back(id);
            }
        }

        keyframeDuplicates.resize(0);

        return result;
    }

    bool BeginTimeline(const char* label, FrameIndexType** keyframes, uint32_t keyframeCount, bool* open,
                          ModotTimelineFlags flags)
    {
        if (!BeginTimelineEx(label, open, flags))
            return false;

        for (uint32_t i = 0; i < keyframeCount; i++)
        {
            ModotKeyframe(keyframes[i]);
        }

        return true;
    }

    void EndTimeLine()
    {
        IM_ASSERT(inSequencer && "Not in active sequencer!");

        auto& context = sequencerData[currentSequencer];
        const auto& imStyle = GetStyle();

        IM_ASSERT(context.TimelineStack.size() > 0 && "Timeline stack push/pop missmatch!");

        context.ValuesCursor.x += imStyle.FramePadding.x + (float) currentTimelineDepth * style.DepthItemSpacing;
        context.ValuesCursor.y += currentTimelineHeight;

        finishPreviousTimeline(context);

        if (!context.TimelineStack.empty() && !context.GroupStack.empty() &&
            context.TimelineStack.back() == context.GroupStack.back())
        {
            currentTimelineDepth--;
            context.GroupStack.pop_back();
        }

        context.TimelineStack.pop_back();
    }

    void ModotKeyframe(int32_t* value)
    {
        IM_ASSERT(inSequencer && "Not in active sequencer!");
        auto& context = sequencerData[currentSequencer];
        IM_ASSERT(!context.TimelineStack.empty() && "Not in timeline!");

        createKeyframe(value);
    }

    bool IsModotKeyframeHovered()
    {
        IM_ASSERT(inSequencer && "Not in active sequencer!");
        auto& context = sequencerData[currentSequencer];

        return context.IsLastKeyframeHovered;
    }

    bool IsModotKeyframeSelected()
    {
        IM_ASSERT(inSequencer && "Not in active sequencer!");
        auto& context = sequencerData[currentSequencer];

        return context.IsLastKeyframeSelected;
    }

    bool IsModotKeyframeRightClicked()
    {
        IM_ASSERT(inSequencer && "Not in active sequencer!");
        auto& context = sequencerData[currentSequencer];

        return context.IsLastKeyframeRightClicked;
    }

    FrameIndexType GetCurrentFrame()
    {
        IM_ASSERT(inSequencer && "Not in active sequencer!");
        auto& context = sequencerData[currentSequencer];

        return context.CurrentFrame;
    }

    void ClearSelection()
    {
        IM_ASSERT(inSequencer && "Not in active sequencer!");
        auto& context = sequencerData[currentSequencer];

        context.Selection.resize(0);
        context.SelectionData.resize(0);
    }

    bool IsSelecting()
    {
        IM_ASSERT(inSequencer && "Not in active sequencer!");
        auto& context = sequencerData[currentSequencer];

        return context.StateOfSelection == SelectionState::Selecting;
    }

    bool HasSelection()
    {
        IM_ASSERT(inSequencer && "Not in active sequencer!");
        auto& context = sequencerData[currentSequencer];

        return !context.Selection.empty();
    }

    bool IsDraggingSelection()
    {
        IM_ASSERT(inSequencer && "Not in active sequencer!");
        auto& context = sequencerData[currentSequencer];

        return context.StateOfSelection == SelectionState::Dragging;
    }

    uint32_t GetKeyframeSelectionSize()
    {
        IM_ASSERT(inSequencer && "Not in active sequencer!");
        auto& context = sequencerData[currentSequencer];

        if (!context.DeleteEnabled)
            return 0;

        IM_ASSERT(!context.TimelineStack.empty() && "Not in timeline!");
        const ImGuiID timelineId = context.TimelineStack.back();

        for (auto&& deleteSelection: context.SelectionData)
        {
            if (deleteSelection.TimelineID == timelineId)
                return deleteSelection.KeyframesToDelete.size();
        }

        return 0;
    }

    void GetModotKeyframeSelection(FrameIndexType * selection)
    {
        IM_ASSERT(inSequencer && "Not in active sequencer!");
        auto& context = sequencerData[currentSequencer];

        if (!context.DeleteEnabled)
            return;

        IM_ASSERT(!context.TimelineStack.empty() && "Not in timeline!");
        const ImGuiID timelineId = context.TimelineStack.back();

        for (auto&& deleteSelection: context.SelectionData)
        {
            if (deleteSelection.TimelineID == timelineId)
            {
                for (int32_t i = 0; i < deleteSelection.KeyframesToDelete.size(); i++)
                {
                    selection[i] = deleteSelection.KeyframesToDelete[i];
                }
                return;
            }
        }

    }

    bool IsModotKeyframeSelectionRightClicked()
    {
        IM_ASSERT(inSequencer && "Not in active sequencer!");
        auto& context = sequencerData[currentSequencer];

        return context.IsSelectionRightClicked;
    }

    bool CanDeleteSelection()
    {
        IM_ASSERT(inSequencer && "Not in active sequencer!");
        auto& context = sequencerData[currentSequencer];

        return context.DeleteEnabled && HasSelection() && !IsSelecting() && !IsDraggingSelection();
    }
}

ModotSequencerStyle::ModotSequencerStyle()
{
    Colors[ModotSequencerCol_Bg]                         = ImVec4{0.11f, 0.11f, 0.11f, 1.00f};
    Colors[ModotSequencerCol_TopBarBg]                   = ImVec4{0.12f, 0.12f, 0.12f, 0.84f};
    Colors[ModotSequencerCol_SelectedTimeline]           = ImVec4{0.98f, 0.706f, 0.322f, 0.88f};
    Colors[ModotSequencerCol_TimelinesBg]                = Colors[ModotSequencerCol_TopBarBg];
    Colors[ModotSequencerCol_TimelineBorder]             = Colors[ModotSequencerCol_Bg] * ImVec4{0.5f, 0.5f, 0.5f, 1.0f};

    Colors[ModotSequencerCol_FramePointer]               = ImVec4{0.24f, 0.34f, 0.77f, 0.50f};
    Colors[ModotSequencerCol_FramePointerHovered]        = ImVec4{0.34f, 0.44f, 0.77f, 1.00f};
    Colors[ModotSequencerCol_FramePointerPressed]        = ImVec4{0.11f, 0.22f, 0.77f, 1.00f};

    Colors[ModotSequencerCol_Keyframe]                   = ImVec4{0.59f, 0.59f, 0.59f, 0.50f};
    Colors[ModotSequencerCol_KeyframeHovered]            = ImVec4{0.36f, 0.39f, 0.98f, 1.00f};
    Colors[ModotSequencerCol_KeyframePressed]            = ImVec4{0.36f, 0.39f, 0.98f, 1.00f};
    Colors[ModotSequencerCol_KeyframeSelected]           = ImVec4{0.98f, 0.23f, 0.32f, 1.00f};

    Colors[ModotSequencerCol_FramePointerLine]           = ImVec4{0.98f, 0.98f, 0.98f, 0.8f};

    Colors[ModotSequencerCol_ZoomBarBg]                  = ImVec4{0.01f, 0.02f, 0.02f, 0.90f};
    Colors[ModotSequencerCol_ZoomBarSlider]              = ImVec4{0.0f, 0.2f, 0.77f, 0.60f};
    Colors[ModotSequencerCol_ZoomBarSliderHovered]       = ImVec4{0.1f, 0.3f, 0.889f, 0.80f};
    Colors[ModotSequencerCol_ZoomBarSliderEnds]          = ImVec4{ 0.0f, 0.2f, 0.77f, 0.60f };
    Colors[ModotSequencerCol_ZoomBarSliderEndsHovered]   = ImVec4{ 0.1f, 0.3f, 0.889f, 0.80f };

    Colors[ModotSequencerCol_SelectionBorder]            = ImVec4{0.98f, 0.706f, 0.322f, 0.61f};
    Colors[ModotSequencerCol_Selection]                  = ImVec4{0.98f, 0.706f, 0.322f, 0.33f};

}
