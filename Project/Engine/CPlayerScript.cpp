#include "pch.h"
#include "CPlayerScript.h"

CPlayerScript::CPlayerScript()
	: m_Speed(200.f)
{
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::Tick()
{
	Vec3 vPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::LEFT))
	{
		vPos.x -= DT * m_Speed;
	}

	if (KEY_PRESSED(KEY::RIGHT))
	{
		vPos.x += DT * m_Speed;
	}

	if (KEY_PRESSED(KEY::SPACE))
		DrawDebugCircle(Transform()->GetRelativePos(), Transform()->GetRelativeScale().x / 2, Vec4(0.f, 1.0f, 0.f, 1.f), 1.f, false);
	if (KEY_PRESSED(KEY::Z))
	{
		MeshRender()->GetMaterial()->SetScalarParam(INT_0, 1);

		Vec3 vScale = Transform()->GetRelativeScale();
		vScale += DT * 100.f * Vec3(1.f, 1.f, 1.f);
		Transform()->SetRelativeScale(vScale);
	}

	Transform()->SetRelativePos(vPos);
}