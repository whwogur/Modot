#pragma once
#include "C:\Users\cuteg\Desktop\Modot\External\Include\Engine\CScript.h"
class CTestObject :
    public CScript
{
public:
    CLONE(CTestObject)
    CTestObject();
    ~CTestObject() = default;

    virtual void Tick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
};

