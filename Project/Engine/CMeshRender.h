#pragma once

#include "CRenderComponent.h"

class CMeshRender :
    public CRenderComponent
{
public:
    CMeshRender();
    ~CMeshRender();

public:
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual CMeshRender* Clone() { return new CMeshRender(*this); }
};