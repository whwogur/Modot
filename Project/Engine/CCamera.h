#pragma once
#include "CComponent.h"

class CCamera :
    public CComponent
{
public:
    CCamera();
    ~CCamera();
public:
    void SetPriority(int _Priority) { m_Priority = _Priority; }
public:
    virtual void Begin() override;
    virtual void FinalTick() override;
    void Render();

private:
    int     m_Priority;
    Matrix  m_matView;
};