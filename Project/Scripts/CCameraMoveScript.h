#pragma once
#include <Engine/CScript.h>

class CCameraMoveScript :
    public CScript
{
public:
    CLONE(CCameraMoveScript);
    CCameraMoveScript();
    ~CCameraMoveScript();

public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
private:
    float           m_CamSpeed = 0.f;
    CGameObject*    m_Target = nullptr;
    Vec2            m_TargetPos2D = {};

    bool            left = false; 
    bool            right = false;

    float           m_Ceiling = 0.f;
    float           m_Floor = 0.f;
    float           m_Offset = 0.f;
};