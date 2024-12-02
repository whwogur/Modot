#pragma once
#include "ImSequencer.h"
#include "ImCurveEdit.h"

static const char* SequencerItemTypeNames[] = { "Camera","Music", "ScreenEffect", "FadeIn", "Animation" };

namespace Modot
{
    using namespace ImSequencer;

    struct RampEdit
        : public ImCurveEdit::Delegate
    {
        RampEdit()
        {
            mPts[0][0]      = ImVec2(-10.f, 0);
            mPts[0][1]      = ImVec2(20.f, 0.6f);
            mPts[0][2]      = ImVec2(25.f, 0.2f);
            mPts[0][3]      = ImVec2(70.f, 0.4f);
            mPts[0][4]      = ImVec2(120.f, 1.f);
            mPointCount[0]  = 5;

            mPts[1][0]      = ImVec2(-50.f, 0.2f);
            mPts[1][1]      = ImVec2(33.f, 0.7f);
            mPts[1][2]      = ImVec2(80.f, 0.2f);
            mPts[1][3]      = ImVec2(82.f, 0.8f);
            mPointCount[1]  = 4;


            mPts[2][0]      = ImVec2(40.f, 0);
            mPts[2][1]      = ImVec2(60.f, 0.1f);
            mPts[2][2]      = ImVec2(90.f, 0.82f);
            mPts[2][3]      = ImVec2(150.f, 0.24f);
            mPts[2][4]      = ImVec2(200.f, 0.34f);
            mPts[2][5]      = ImVec2(250.f, 0.12f);
            mPointCount[2]  = 6;
            mbVisible[0]    = mbVisible[1] = mbVisible[2] = true;
            mMax            = ImVec2(1.f, 1.f);
            mMin            = ImVec2(0.f, 0.f);
        }

        size_t GetCurveCount()
        {
            return 3;
        }

        bool IsVisible(size_t curveIndex)
        {
            return mbVisible[curveIndex];
        }
        size_t GetPointCount(size_t curveIndex)
        {
            return mPointCount[curveIndex];
        }

        uint32_t GetCurveColor(size_t curveIndex)
        {
            uint32_t cols[] = { 0xFF0000FF, 0xFF00FF00, 0xFFFF0000 };
            return cols[curveIndex];
        }
        ImVec2* GetPoints(size_t curveIndex)
        {
            return mPts[curveIndex];
        }

        virtual ImCurveEdit::CurveType GetCurveType(size_t curveIndex) const { return ImCurveEdit::CurveSmooth; }
        virtual int EditPoint(size_t curveIndex, int pointIndex, ImVec2 value)
        {
            mPts[curveIndex][pointIndex] = ImVec2(value.x, value.y);
            SortValues(curveIndex);
            for (size_t i = 0; i < GetPointCount(curveIndex); i++)
            {
                if (mPts[curveIndex][i].x == value.x)
                    return (int)i;
            }
            return pointIndex;
        }
        virtual void AddPoint(size_t curveIndex, ImVec2 value)
        {
            if (mPointCount[curveIndex] >= 8)
                return;
            mPts[curveIndex][mPointCount[curveIndex]++] = value;
            SortValues(curveIndex);
        }
        virtual ImVec2& GetMax() { return mMax; }
        virtual ImVec2& GetMin() { return mMin; }
        virtual unsigned int GetBackgroundColor() { return 0; }
        ImVec2 mPts[3][8];
        size_t mPointCount[3];
        bool mbVisible[3];
        ImVec2 mMin;
        ImVec2 mMax;
    private:
        void SortValues(size_t curveIndex)
        {
            auto b = std::begin(mPts[curveIndex]);
            auto e = std::begin(mPts[curveIndex]) + GetPointCount(curveIndex);
            std::sort(b, e, [](ImVec2 a, ImVec2 b) { return a.x < b.x; });

        }
    };
    struct ModotSequenceItem
    {
        int     Type;
        int     FrameStart;
        int     FrameEnd;
        bool    Expanded;
    };
    class ModotSequencer
        : public ImSequencer::SequenceInterface
    {
    public:
        ModotSequencer() = default;
        ~ModotSequencer() = default;

        virtual int GetFrameMin() const { return m_FrameMin; }
        virtual int GetFrameMax() const { return m_FrameMax; }
        virtual int GetItemCount() const { return (int)m_vecItems.size(); }
        virtual int GetItemTypeCount() const { return sizeof(SequencerItemTypeNames) / sizeof(char*); }
        virtual const char* GetItemTypeName(int typeIndex) const { return SequencerItemTypeNames[typeIndex]; }
        virtual const char* GetItemLabel(int index) const
        {
            static char tmps[512];
            _snprintf_s(tmps, 512, "[%02d] %s", index, SequencerItemTypeNames[m_vecItems[index].Type]);
            return tmps;
        }
        virtual void Get(int index, int** start, int** end, int* type, unsigned int* color);
        virtual void Add(int type) { m_vecItems.push_back(ModotSequenceItem{ type, 0, 10, false }); };
        virtual void Del(int index) { m_vecItems.erase(m_vecItems.begin() + index); }
        virtual void Duplicate(int index) { m_vecItems.push_back(m_vecItems[index]); }

        virtual size_t GetCustomHeight(int index) { return m_vecItems[index].Expanded ? 300 : 0; }
        virtual void DoubleClick(int index);
        virtual void CustomDraw(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& legendRect, const ImRect& clippingRect, const ImRect& legendClippingRect);
        virtual void CustomDrawCompact(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& clippingRect);
        
    public:
        std::vector<ModotSequenceItem>& GetItems() { return m_vecItems; }
        ModotSequenceItem& GetItemRef(int _Idx) { return m_vecItems[_Idx]; }
        int& GetFrameMinRef() { return m_FrameMin; }
        int& GetFrameMaxRef() { return m_FrameMax; }

    private:
        int                             m_FrameMin = 0;
        int                             m_FrameMax = 0;
        std::vector<ModotSequenceItem>  m_vecItems;
        RampEdit                        rampEdit;
    };
}
