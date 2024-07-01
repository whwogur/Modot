#pragma once
#include "CAsset.h"

class CSprite;

class CAnimation :
    public CAsset
{

public:
    CAnimation();
    ~CAnimation() = default;
public:
    void FinalTick();

private:
    vector<CSprite*>    m_vecSprite;
};