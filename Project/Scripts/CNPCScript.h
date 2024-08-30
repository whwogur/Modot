#pragma once
#include <Engine/CScript.h>

class CNPCScript :
    public CScript
{
public:
    CNPCScript();
    ~CNPCScript() = default;
    CLONE(CNPCScript);
public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
private:
    int         m_AnimIndex;
    float       m_FPS;
};

