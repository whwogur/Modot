#pragma once
#include "CAsset.h"
#include "CSprite.h"

class CAnimation :
    public CAsset
{

public:
    CAnimation();
    ~CAnimation() = default;
public:
    void FinalTick();

    virtual int Load(const wstring& _FilePath) override;
    virtual int Save(const wstring& _FilePath) override;

public:
    void AddSprite(Ptr<CSprite> _Sprite) { m_vecSprite.push_back(_Sprite); }
    Ptr<CSprite> GetSprite(int _Idx) { return m_vecSprite[_Idx]; }
    int GetMaxFrameCount() { return (int)m_vecSprite.size(); }

private:
    vector<Ptr<CSprite>>    m_vecSprite;
};