#pragma once

#include "CRenderComponent.h"

class CMeshRender :
    public CRenderComponent
{
public:
    CMeshRender();
    ~CMeshRender() = default;
    CLONE(CMeshRender);
public:
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
};