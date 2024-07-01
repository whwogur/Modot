#pragma once
#include "CAsset.h"
class CSprite :
    public CAsset
{
public:
    CSprite();
    ~CSprite();

public:
    void Create(Ptr<CTexture> _Atlas, Vec2 _LeftTopPixel, Vec2 _SlicePixel);

private:
    Ptr<CTexture> m_Atlas;
    Vec2 m_LeftTop;
    Vec2 m_Slice;
};