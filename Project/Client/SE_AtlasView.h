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

    ImVec2          ImageRectMin; // Image ���� �»�� ���� ��ġ

    ImVec2          m_MousePos; // ���� ���콺 ��ġ

    ImVec2          m_MouseLT;  // ���콺 ��ġ�� �ش��ϴ� Texture �� �ȼ� ��ǥ
    ImVec2          m_MouseRB; // ���콺 ��ġ�� �ش��ϴ� Texture �� �ȼ� ��ǥ
    ImVec2          m_BGSize;
    float           m_WidthSize;
    float           m_WheelScale;
    float           m_Ratio;        // ���� �ػ� ��� ��� �̹����� ����
};

