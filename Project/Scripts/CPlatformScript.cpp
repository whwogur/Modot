#include "spch.h"
#include "CPlatformScript.h"
#include "../Client/CEditorMgr.h"
#include "CPlayerScript.h"

CPlatformScript::CPlatformScript()
	: CScript(SCRIPT_TYPE::PLATFORMSCRIPT)
{
    AddScriptParam(SCRIPT_PARAM::BOOLEAN_CHECKBOX, u8"��������", &m_DownJump);
}

void CPlatformScript::Begin()
{
    const Vec3& rotation = Transform()->GetRelativeRoatationRef();
    m_SlopeNormal = Vec2(cosf(rotation.z), sinf(rotation.z));
}

void CPlatformScript::Tick()
{
}

void CPlatformScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    CRigidBody* rb = _OtherObject->RigidBody();
    if (rb != nullptr)
    {
        int& platformCount = rb->GetPlatformCountRef();
        ++platformCount;
        // _OtherObject->RigidBody()->SetGround(true);
        const Vec3& otherObjPos = _OtherObject->Transform()->GetRelativePosRef();
        const Vec3& ownObjPos = Transform()->GetRelativePosRef();

        const Vec3& ownSize = Transform()->GetRelativeScaleRef();
        const Vec3& otherSize = _OtherObject->Transform()->GetRelativeScaleRef();

        // X��� Y���� ���� ���
        float deltaX = otherObjPos.x - ownObjPos.x;
        float deltaY = otherObjPos.y - ownObjPos.y;

        // X��� Y���� ��ħ ���
        float overlapX = (fabs(otherSize.x) + ownSize.x) * 0.5f - fabs(deltaX);
        float overlapY = (otherSize.y + ownSize.y) * 0.5f - fabs(deltaY);

        // X��� Y���� ��ħ ũ�⸦ ���Ͽ� �� ���� ������ �о��
        if (overlapX > overlapY)
        {
            // Y�� �о��
            if (deltaY > 0)
            {
                rb->SetGround(true);
                rb->SetDownJump(m_DownJump);
                rb->SetGroundNormal(m_SlopeNormal);
                if (platformCount <= 2)
                {
                    CPlayerScript* playerScript = static_cast<CPlayerScript*>(_OtherObject->FindScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT));
                    if(playerScript != nullptr)
                    {
                        playerScript->ChangeState(PlayerState::LAND);
                    }
                }
            }
            else if (!m_DownJump)
            {
                // �ٸ� ������Ʈ�� �Ʒ������� �о
                _OtherObject->Transform()->SetRelativePos(otherObjPos.x, ownObjPos.y + deltaY, otherObjPos.z);
                rb->SetVelocity(Vec2());
            }
        }
    }
    /*EDITOR_TRACE(std::to_string(m_SlopeNormal.x));
    EDITOR_TRACE(std::to_string(m_SlopeNormal.y));*/
}

void CPlatformScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}


void CPlatformScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    CRigidBody* rb = _OtherObject->RigidBody();
    if (rb != nullptr)
    {
        int& platformCount = rb->GetPlatformCountRef();
        --platformCount;

        const Vec2 vel = rb->GetVelocity();

        if (platformCount < 1)
            rb->SetGround(false);
    }
}

void CPlatformScript::SaveToFile(FILE* _File)
{
    fwrite(&m_DownJump, sizeof(bool), 1, _File);
}

void CPlatformScript::LoadFromFile(FILE* _File)
{
    fread(&m_DownJump, sizeof(bool), 1, _File);
}
