#include "spch.h"
#include "CPlatformScript.h"

CPlatformScript::CPlatformScript()
	: CScript(SCRIPT_TYPE::PLATFORMSCRIPT)
{
}

void CPlatformScript::Begin()
{
}

void CPlatformScript::Tick()
{
}

void CPlatformScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	CRigidBody* rb = _OtherObject->RigidBody();
	if (rb != nullptr)
	{
		rb->SetGroundRotation(Transform()->GetRelativeRoatation().z);
		rb->SetGravityAccel(100.f);
	}
}

void CPlatformScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    const Vec3& otherColloffset = _OtherCollider->GetOffsetRef();
    const Vec3& otherObjPos = _OtherObject->Transform()->GetRelativePosRef();
    const Vec3& ownColloffset = _OwnCollider->GetOffsetRef();
    const Vec3& ownObjPos = Transform()->GetRelativePosRef();

    const Vec3& ownSize = Transform()->GetRelativeScaleRef();
    const Vec3& otherSize = _OtherObject->Transform()->GetRelativeScaleRef();

    const Vec3& otherCollRelsize = _OtherCollider->GetScaleRef();
    const Vec3& ownCollRelsize = _OwnCollider->GetScaleRef();

    const bool& otherCollInd = _OtherCollider->GetIndependetScaleRef();
    const bool& ownCollInd = _OwnCollider->GetIndependetScaleRef();

    Vec3 otherCollsize = otherCollInd ? otherSize : otherCollRelsize * otherSize;
    Vec3 ownCollsize = ownCollInd ? ownSize : ownCollRelsize * ownSize;

    Vec3 otherPos = otherCollInd ? otherColloffset + otherObjPos : otherColloffset * otherCollsize + otherObjPos;
    Vec3 ownPos = ownCollInd ? ownColloffset + ownObjPos : ownColloffset * ownCollsize + ownObjPos;

    // X축과 Y축의 차이 계산
    float deltaX = otherPos.x - ownPos.x;
    float deltaY = otherPos.y - ownPos.y;

    // X축과 Y축의 겹침 계산
    float overlapX = (fabs(otherCollsize.x) + ownCollsize.x) * 0.5f - fabs(deltaX);
    float overlapY = (otherCollsize.y + ownCollsize.y) * 0.5f - fabs(deltaY);

    if (overlapX > 0 && overlapY > 0)
    {
        // X축과 Y축의 겹침 크기를 비교하여 더 작은 쪽으로 밀어내기
        if (overlapX < overlapY)
        {
            // X축 밀어내기
            if (deltaX > 0)
            {
                // 다른 오브젝트를 오른쪽으로 밀어냄
                _OtherObject->Transform()->SetRelativePos(ownPos.x + overlapX, otherObjPos.y, otherObjPos.z);
            }
            else
            {
                // 다른 오브젝트를 왼쪽으로 밀어냄
                _OtherObject->Transform()->SetRelativePos(ownPos.x - overlapX, otherObjPos.y, otherObjPos.z);
            }
        }
        //else
        //{
        //    // Y축 밀어내기
        //    if (deltaY > 0)
        //    {
        //        // 다른 오브젝트를 위쪽으로 밀어냄
        //        _OtherObject->Transform()->SetRelativePos(otherObjPos.x, otherObjPos.y + overlapY, otherObjPos.z);
        //    }
        //    else
        //    {
        //        // 다른 오브젝트를 아래쪽으로 밀어냄
        //        _OtherObject->Transform()->SetRelativePos(otherObjPos.x, otherObjPos.y - overlapY, otherObjPos.z);
        //    }
        //}
    }
}


void CPlatformScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CPlatformScript::SaveToFile(FILE* _File)
{
}

void CPlatformScript::LoadFromFile(FILE* _File)
{
}
