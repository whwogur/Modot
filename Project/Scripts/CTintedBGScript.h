#pragma once
#include "C:\Users\cuteg\Desktop\Modot\External\Include\Engine\CScript.h"
class CTintedBGScript :
    public CScript
{
public:
    CTintedBGScript();
    ~CTintedBGScript() = default;
    CLONE(CTintedBGScript);

    virtual void Begin() override;
    virtual void Tick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
private:
    Ptr<CTexture>     m_Texture;
    Vec4                m_Tint;
    bool                m_Mobile;
};

