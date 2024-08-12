#pragma once
#include "SE_Sub.h"

class SE_AtlasView :
    public SE_Sub
{
public:
    SE_AtlasView();
    ~SE_AtlasView() = default;

public:
    void SetAtlasTex(Ptr<CTexture> _Tex);
    void SetBGSize(ImVec2 _Vec) { m_BGSize = _Vec; }
public:
    virtual void Init() override;
    virtual void Update() override;

private:
    void WheelCheck();
    void SelectCheck();
    void DrawSelectRect();
private:
    Ptr<CTexture>   m_AtlasTex;

    ImVec2          ImageRectMin; // Image 위젯 좌상단 실제 위치

    ImVec2          m_MousePos; // 실제 마우스 위치

    ImVec2          m_MouseLT;  // 마우스 위치에 해당하는 Texture 의 픽셀 좌표
    ImVec2          m_MouseRB; // 마우스 위치에 해당하는 Texture 의 픽셀 좌표
    ImVec2          m_BGSize;
    float           m_WidthSize;
    float           m_WheelScale;
    float           m_Ratio;        // 실제 해상도 대비 출력 이미지의 비율
};

