#pragma once
#include "C:\Users\cuteg\Desktop\Modot\External\Include\Engine\CScript.h"
class CTestObject :
    public CScript
{
public:
    CLONE(CTestObject)
    CTestObject();
    ~CTestObject() = default;
    
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    bool                m_Move = false;
    Ptr<CTexture>       m_AlbedoTex = nullptr;
    Ptr<CTexture>       m_NormalTex = nullptr;
};

