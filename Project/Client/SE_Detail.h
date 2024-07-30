#pragma once
#include "SE_Sub.h"
class SE_Detail :
    public SE_Sub
{
public:
    SE_Detail();
    ~SE_Detail();

public:
    void SetAtlasTex(Ptr<CTexture> _Tex);
    void SetLeftTop(Vec2 _LT) { m_LeftTop = _LT; }
    void SetSlice(Vec2 _Slice) { m_Slice = _Slice; }
public:
    virtual void Init() override;
    virtual void Update() override;

private:
    void Atlas();
    void AtlasInfo();
    void SelectTexture(DWORD_PTR _ListUI);
private:
    Ptr<CTexture>   m_AtlasTex;
    int             m_BGSizeX = 0;
    int             m_BGSizeY = 0;

    Vec2            m_LeftTop = {};
    Vec2            m_Slice = {};
};