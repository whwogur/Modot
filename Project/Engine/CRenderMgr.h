#pragma once
#include "singleton.h"

class CCamera;
class CGameObject;
class CLight2D;
class CStructuredBuffer;
#define FONT_RGBA(r, g, b, a) (((((BYTE)a << 24 ) | (BYTE)b << 16) | (BYTE)g << 8) | (BYTE)r)

enum class CameraPriority
{
    Main = 0,
    Menu,
};

struct tRenderText
{
    tRenderText()
        : Detail{}
        , Pos{}
        , FontSize(25.f)
        , RGBA(0)
    {};
    tRenderText(const wstring& detail, const Vec2& pos, float fontSize, UINT rgba)
        : Detail(detail)
        , Pos(pos)
        , FontSize(fontSize)
        , RGBA(rgba)
    {}
    ~tRenderText() = default;

    wstring     Detail;
    Vec2        Pos;
    float       FontSize;
    UINT        RGBA;
};

class CRenderMgr :
    public CSingleton<CRenderMgr>
{
    SINGLE(CRenderMgr)
public:
    void RegisterCamera(CCamera* _Cam, int _CamPriority);
    void Init(CCamera* _Cam) { m_EditorCamera = _Cam; }
    void AddDebugShapeInfo(const tDebugShapeInfo& _Info) { m_DebugShapeList.emplace_back(_Info); }
    void AddRenderText(const tRenderText& _Text) { m_vecText.emplace_back(_Text); }
    void RegisterLight2D(CLight2D* _Light) { m_vecLight2D.push_back(_Light); }
    void PostProcessCopy();

    CCamera* GetEditorCamera() { return m_EditorCamera; }

    CCamera* GetCamera(UINT _Idx)
    {
        if (m_vecCam[_Idx] != nullptr)
            return m_vecCam[_Idx];
    }
    bool& GetDebugRenderRef() { return m_DebugRender; }
public:
    void Init();
    void Tick();

private:
    void RenderStart();
    void Clear();
    void RenderDebugShape();
    void DrawTextOnScreen();
private:
    CCamera*                        m_EditorCamera;
    vector<CCamera*>                m_vecCam;
    list<tDebugShapeInfo>           m_DebugShapeList;
    CGameObject*                    m_DebugObject;
    // Light
    vector<CLight2D*>               m_vecLight2D;
    CStructuredBuffer*              m_Light2DBuffer;
    Ptr<CTexture>                   m_PostProcessTex;
    // TEXT
    vector<tRenderText>             m_vecText;
    bool                            m_DebugRender = true;
};