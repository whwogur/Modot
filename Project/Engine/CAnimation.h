#pragma once
#include "CAsset.h"
#include "CSprite.h"

class CAnimation :
    public CAsset
{

public:
    CAnimation();
    ~CAnimation() = default;
    CAnimation(const CAnimation& _Other);
    CLONE(CAnimation);
public:
    void FinalTick();

    virtual int Load(const wstring& _FilePath) override;
    virtual int Save(const wstring& _FilePath) override;

public:
    void AddSprite(Ptr<CSprite> _Sprite) { m_vecSprite.push_back(_Sprite); }
    Ptr<CSprite> GetSprite(int _Idx) { return m_vecSprite[_Idx]; }
    Ptr<CSprite>& GetSpriteRef(int _Idx) { return m_vecSprite[_Idx]; }
    int GetMaxFrameCount() { return (int)m_vecSprite.size(); }
    const std::vector<Ptr<CSprite>>& GetSpritesCRef() { return m_vecSprite; }
    std::vector<Ptr<CSprite>>& GetSpritesRef() { return m_vecSprite; }
private:
    std::vector<Ptr<CSprite>>    m_vecSprite;
};