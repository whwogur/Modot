#pragma once
#include "C:\Users\cuteg\Desktop\Modot\External\Include\Engine\CScript.h"
class CBackGroundScript :
    public CScript
{
public:
    CBackGroundScript();
    ~CBackGroundScript() = default;
    CLONE(CBackGroundScript);

    virtual void Begin() override;
    virtual void Tick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
private:
    Ptr<CTexture>       m_Texture;
    bool                m_Mobile;
};

