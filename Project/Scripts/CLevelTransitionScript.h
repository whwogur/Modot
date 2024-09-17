#pragma once
#include "C:\Users\cuteg\Desktop\Modot\External\Include\Engine\CScript.h"
class CLevelTransitionScript :
    public CScript
{
public:
    CLevelTransitionScript();
    ~CLevelTransitionScript() = default;
    CLONE(CLevelTransitionScript);
public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
    
    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
private:
    char   m_LevelName[255] = {};
};

