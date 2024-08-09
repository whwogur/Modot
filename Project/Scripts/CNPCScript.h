#pragma once
#include <Engine/CScript.h>

class CNPCScript :
    public CScript
{
public:
    CNPCScript();
    ~CNPCScript() = default;
    CLONE(CNPCScript);
public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;


private:
    string      m_AnimName;
};

