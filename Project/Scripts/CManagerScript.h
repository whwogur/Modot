#pragma once
#include "C:\Users\cuteg\Desktop\Modot\External\Include\Engine\CScript.h"
class CManagerScript :
    public CScript
{
public:
    CManagerScript();
    ~CManagerScript() = default;
    CLONE(CManagerScript);
public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    Ptr<CSound>         m_BGM;
};

