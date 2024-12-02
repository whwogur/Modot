// https://github.com/CedricGuillemet/ImGuizmo
// v1.91.3 WIP
//
// The MIT License(MIT)
//
// Copyright(c) 2021 Cedric Guillemet
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#pragma once

#include <cstddef>

struct ImDrawList;
struct ImRect;
namespace ImSequencer
{
    enum SEQUENCER_OPTIONS
    {
        SEQUENCER_EDIT_NONE = 0,
        SEQUENCER_EDIT_STARTEND = 1 << 1,
        SEQUENCER_CHANGE_FRAME = 1 << 3,
        SEQUENCER_ADD = 1 << 4,
        SEQUENCER_DEL = 1 << 5,
        SEQUENCER_COPYPASTE = 1 << 6,
        SEQUENCER_EDIT_ALL = SEQUENCER_EDIT_STARTEND | SEQUENCER_CHANGE_FRAME
    };

    struct SequenceInterface
    {
        bool focused = false;
        virtual int GetFrameMin() const = 0;
        virtual int GetFrameMax() const = 0;
        virtual int GetItemCount() const = 0;

        virtual void BeginEdit(int _Idx) {}
        virtual void EndEdit() {}
        virtual int GetItemTypeCount() const { return 0; }
        virtual const char* GetItemTypeName(int _TypeIdx) const { return ""; }
        virtual const char* GetItemLabel(int _Idx) const { return ""; }
        virtual const char* GetCollapseFmt() const { return "%d Frames / %d entries"; }

        virtual void Get(int index, int** start, int** end, int* type, unsigned int* color) = 0;
        virtual void Add(int _Type) {}
        virtual void Del(int _Idx) {}
        virtual void Duplicate(int _Idx) {}

        virtual void Copy() {}
        virtual void Paste() {}

        virtual size_t GetCustomHeight(int _Idx) { return 0; }
        virtual void DoubleClick(int _Idx) {}
        virtual void CustomDraw(int _Idx, ImDrawList* _DrawList, const ImRect& _Rect, const ImRect& _LegendRect, const ImRect& _ClippingRect, const ImRect& _LegendClippingRect) {}
        virtual void CustomDrawCompact(int _Idx, ImDrawList* _DrawList, const ImRect& _Rect, const ImRect& _ClippingRect) {}

        virtual ~SequenceInterface() = default;
    };

    // 선택 시 참 반환
    bool Sequencer(SequenceInterface* sequence, int* currentFrame, bool* expanded, int* selectedEntry, int* firstFrame, int sequenceOptions);
}