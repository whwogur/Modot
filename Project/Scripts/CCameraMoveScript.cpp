#include "spch.h"
#include "CCameraMoveScript.h"
#include <Engine/CLevelMgr.h>

CCameraMoveScript::CCameraMoveScript()
	: CScript(UINT(SCRIPT_TYPE::CAMERAMOVESCRIPT))
	, m_CamSpeed(500.f)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Camera Speed", &m_CamSpeed);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Ceiling", &m_Ceiling);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Floor", &m_Floor);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Offset", &m_Offset);
}

CCameraMoveScript::~CCameraMoveScript()
{
}

void CCameraMoveScript::Begin()
{
	m_Target = CLevelMgr::GetInst()->FindObjectByName(L"Player");
}

void CCameraMoveScript::Tick()
{
	if (m_Target != nullptr)
	{
		// AtTargetDir: 타겟의 상대 위치와 카메라 위치의 차이 계산
		const Vec3& targetPos = m_Target->Transform()->GetRelativePos();
		Vec3& camPos = Transform()->GetRelativePosRef();
		
		Vec2 AtTargetDir(targetPos.x - camPos.x, targetPos.y - camPos.y + m_Offset);

		// fdist: AtTargetDir의 길이를 구함
		float fdist = AtTargetDir.Length();

		if (fdist > 50.f)
		{
			// AtTargetDir 벡터를 정규화
			AtTargetDir.Normalize();

			if (right && AtTargetDir.x > 0)
				AtTargetDir.x = 0;
			if (left && AtTargetDir.x < 0)
				AtTargetDir.x = 0;

			Vec2 vAccel(AtTargetDir.x * m_CamSpeed * DT, AtTargetDir.y * m_CamSpeed * DT);

			camPos.x += vAccel.x;

			const Vec3& camSize = Transform()->GetRelativeScaleRef();

			if ((vAccel.y + camPos.y + camSize.y / 2) < m_Ceiling
				&& (vAccel.y + camPos.y - camSize.y / 2) > m_Floor)
				camPos.y += vAccel.y;
		}
	}
}

void CCameraMoveScript::SaveToFile(FILE* _File)
{
	fwrite(&m_CamSpeed, sizeof(float), 1, _File);
	fwrite(&m_Ceiling, sizeof(float), 1, _File);
	fwrite(&m_Floor, sizeof(float), 1, _File);
	fwrite(&m_Offset, sizeof(float), 1, _File);
}

void CCameraMoveScript::LoadFromFile(FILE* _File)
{
	fread(&m_CamSpeed, sizeof(float), 1, _File);
	fread(&m_Ceiling, sizeof(float), 1, _File);
	fread(&m_Floor, sizeof(float), 1, _File);
	fread(&m_Offset, sizeof(float), 1, _File);
}

void CCameraMoveScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	const Vec3& wallPos = _OtherObject->Transform()->GetRelativePos();
	const Vec3& collSize = Transform()->GetRelativeScaleRef();
	const Vec3& curPos = Transform()->GetRelativePosRef();

	if (wallPos.x < curPos.x - collSize.x / 2)
		left = true;
	else if (wallPos.x > curPos.x + collSize.x / 2)
		right = true;
}

void CCameraMoveScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CCameraMoveScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	const Vec3& wallPos = _OtherObject->Transform()->GetRelativePos();
	const Vec3& collSize = Transform()->GetRelativeScaleRef();
	const Vec3& curPos = Transform()->GetRelativePosRef();

	if (wallPos.x < curPos.x - collSize.x / 2)
		left = false;
	if (wallPos.x > curPos.x + collSize.x / 2)
		right = false;
}
