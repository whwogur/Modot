#pragma once
#include "CRenderComponent.h"
class CSkyBox :
    public CRenderComponent
{
public:
    CLONE(CSkyBox)
    CSkyBox();
    ~CSkyBox() = default;

public:
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    

};

