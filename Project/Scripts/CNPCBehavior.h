#pragma once
#include "C:\Users\cuteg\Desktop\Modot\External\Include\Engine\CScript.h"
enum class NPCType
{
    YUHACAT,
    DORA,
    OLIVERIA,
    NONE,
};

class CNPCBehavior :
    public CScript
{
public:
    CNPCBehavior();
    ~CNPCBehavior() = default;
    CLONE(CNPCBehavior);
public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    void Activate();
private:
    NPCType     m_Type = NPCType::NONE;
    bool        m_Activated = false;

    float       m_Acc = 0.f;
    float       m_Timer = 0.f;
};

