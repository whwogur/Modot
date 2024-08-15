#pragma once
#include <Engine/CScript.h>
class CSpriteRenderScript :
    public CScript
{
public:
    CSpriteRenderScript();
    ~CSpriteRenderScript() = default;
    CLONE(CSpriteRenderScript);
public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    Ptr<CSprite>            m_Sprite;
};

