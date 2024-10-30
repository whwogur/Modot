#pragma once
#include "CEntity.h"
enum class FACE_TYPE
{
    FT_NEAR,
    FT_FAR,
    FT_TOP,
    FT_BOT,
    FT_LEFT,
    FT_RIGHT,
    END,
};

class CCamera;

class CFrustum :
    public CEntity
{
public:
    CLONE(CFrustum);
    CFrustum(CCamera* _Owner);
    ~CFrustum() = default;

public:
    void FinalTick();
    bool FrustumCheck(const Vec3& _WorldPos, float _Radius = 0.f);
private:
    CCamera*    m_Owner;
    // NDC ��ǥ�迡�� �þ��� �ִ���ġ ��ǥ
    Vec3        m_arrProj[8];
    // ����ü�� �����ϴ� 6���� ���(���� �����̽� ����)
    Vec4        m_arrFace[(UINT)FACE_TYPE::END];
};