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
	
	CameraEffect();
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

void CCameraMoveScript::CameraEffect()
{
	while (true)
	{
		if (m_EffectList.empty())
			return;

		CAM_EFFECT_INFO& info = m_EffectList.front();
		info.Time += DT;

		if (info.Duration < info.Time)
		{
			m_EffectList.pop_front();
		}
		else
		{
			break;
		}
	}

	CAM_EFFECT_INFO& info = m_EffectList.front();

	
	if (CAM_EFFECT::SHAKE == info.Effect)
	{
		int x = (rand() % 10 * 2) - 10;
		int y = (rand() % 10 * 2) - 10;
		x = x == 0 ? 1 : x;
		y = y == 0 ? 1 : y;

		float ShakeRatio = 1.f - info.Time / info.Duration;
		Vec3& camPos = Transform()->GetRelativePosRef();

		camPos.x += (float)x * ShakeRatio;
		camPos.y += (float)y * ShakeRatio;

		return;
	}
}

void CCameraMoveScript::SetCameraEffect(CAM_EFFECT _Effect, float _Duration)
{
	CAM_EFFECT_INFO info = {};

	info.Effect = _Effect;
	info.Duration = _Duration;
	info.Time = 0.f;
	info.Alpha = 0.f;

	m_EffectList.push_back(info);
}

