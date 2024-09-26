#pragma once
#include "C:\Users\cuteg\Desktop\Modot\External\Include\Engine\CScript.h"
class CSigil :
    public CScript
{
public:
    CSigil();
    ~CSigil() = default;
    CLONE(CSigil);
public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    void    Activate() { m_Active = true; m_Acc = 0.f; }
public:
    Ptr<CTexture>       m_Texture;
    float               m_RPS = 1.f;
    float               m_Acc = 0.f;
    bool                m_Active = false;
    Vec4                m_BloomColor;
};

