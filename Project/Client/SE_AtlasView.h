#pragma once
#include "SE_Sub.h"
using std::set;

class SE_AtlasView :
    public SE_Sub
{
public:
    SE_AtlasView();
    ~SE_AtlasView();

public:
    void SetAtlasTex(Ptr<CTexture> _Tex);
public:
    virtual void Init() override;
    virtual void Update() override;

private:
    void WheelCheck();
    void SelectCheck();
    void DrawSelectRect();
    void CalcSpriteSize(Vec2 _PixelPos);
    bool IsPixelOk(Vec2 _PixelPos);
private:
    Ptr<CTexture>   m_AtlasTex;

    ImVec2          ImageRectMin; // Image ���� �»�� ���� ��ġ

    ImVec2          m_MousePos; // ���� ���콺 ��ġ

    ImVec2          m_MouseLT;  // ���콺 ��ġ�� �ش��ϴ� Texture �� �ȼ� ��ǥ
    ImVec2          m_MouseRB; // ���콺 ��ġ�� �ش��ϴ� Texture �� �ȼ� ��ǥ
    set<Vec2>       m_PixelID;


    float           m_WidthSize;
    float           m_WheelScale;
    float           m_Ratio;        // ���� �ػ� ��� ��� �̹����� ����
};

