#pragma once
#include "CScript.h"

class CPlayerScript :
    public CScript
{
public:
    CPlayerScript();
    ~CPlayerScript();

public:
    virtual void Tick() override;

private:
    float   m_Speed;
};
