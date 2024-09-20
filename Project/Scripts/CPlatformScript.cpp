#include "spch.h"
#include "CPlatformScript.h"
#include "../Client/CEditorMgr.h"
#include "CPlayerScript.h"

CPlatformScript::CPlatformScript()
	: CScript(SCRIPT_TYPE::PLATFORMSCRIPT)
{
    AddScriptParam(SCRIPT_PARAM::BOOLEAN_CHECKBOX, u8"하향점프", &m_DownJump);
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

        // X축과 Y축의 차이 계산
        float deltaX = otherObjPos.x - ownObjPos.x;
        float deltaY = otherObjPos.y - ownObjPos.y;

        // X축과 Y축의 겹침 계산
        float overlapX = (fabs(otherSize.x) + ownSize.x) * 0.5f - fabs(deltaX);
        float overlapY = (otherSize.y + ownSize.y) * 0.5f - fabs(deltaY);

        // X축과 Y축의 겹침 크기를 비교하여 더 작은 쪽으로 밀어내기
        if (overlapX < overlapY && !m_DownJump)
        {
            if (deltaX > 0)
                rb->SetLeftWall(true);
            else
                rb->SetRightWall(true);

            _OtherObject->Transform()->SetRelativePos(ownObjPos.x + deltaX, otherObjPos.y, otherObjPos.z);
            rb->SetVelocity(Vec2());
        }
        else
        {
            // Y축 밀어내기
            if (deltaY > 0)
            {
                rb->SetGround(true);
                rb->SetDownJump(m_DownJump);
                rb->SetGroundNormal(m_SlopeNormal);
                if (_OtherObject->GetName() == L"Player" && platformCount == 1)
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
                // 다른 오브젝트를 아래쪽으로 밀어냄
                _OtherObject->Transform()->SetRelativePos(otherObjPos.x, ownObjPos.y + deltaY, otherObjPos.z);
                rb->SetVelocity(Vec2());
            }
        }
    }
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
        else
        {
            if (vel.x > 0)
                rb->SetLeftWall(false);
            else
                rb->SetRightWall(false);
        }
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
