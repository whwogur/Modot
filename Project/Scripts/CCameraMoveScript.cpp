#include "spch.h"
#include "CCameraMoveScript.h"
#include <Engine/CLevelMgr.h>

CCameraMoveScript::CCameraMoveScript()
	: CScript(UINT(SCRIPT_TYPE::CAMERAMOVESCRIPT))
	, m_CamSpeed(500.f)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Camera Speed", &m_CamSpeed);
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
	if (PROJ_TYPE::ORTHOGRAPHIC == Camera()->GetProjType())
	{
		OrthoGraphicMove();
	}

	else if (PROJ_TYPE::PERSPECTIVE == Camera()->GetProjType())
	{
		PerspectiveMove();
	}


	if (KEY_TAP(KEY::P))
	{
		if (PROJ_TYPE::ORTHOGRAPHIC == Camera()->GetProjType())
			Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
		else
			Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	}
}



void CCameraMoveScript::SaveToFile(FILE* _File)
{
	fwrite(&m_CamSpeed, sizeof(float), 1, _File);
}

void CCameraMoveScript::LoadFromFile(FILE* _File)
{
	fread(&m_CamSpeed, sizeof(float), 1, _File);
}

void CCameraMoveScript::OrthoGraphicMove()
{
	if (KEY_PRESSED(KEY::CTRL))
	{
		float Speed = m_CamSpeed;

		if (KEY_PRESSED(KEY::LSHIFT))
		{
			Speed *= 3.f;
		}

		Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
		Vec3 vPos = Transform()->GetRelativePos();

		if (KEY_PRESSED(KEY::W))
		{
			vPos.y += DT * Speed;
		}

		if (KEY_PRESSED(KEY::S))
		{
			vPos.y -= DT * Speed;
		}

		if (KEY_PRESSED(KEY::A))
		{
			vPos.x -= DT * Speed;
		}

		if (KEY_PRESSED(KEY::D))
		{
			vPos.x += DT * Speed;
		}

		Transform()->SetRelativePos(vPos);
	}

	if (m_Target != nullptr)
	{
		const Vec3 objPos = m_Target->Transform()->GetRelativePos();
		Transform()->SetRelativePos(objPos + Vec3(0.f, 340.f, 0.f));
	}
}

void CCameraMoveScript::PerspectiveMove()
{
	if (KEY_PRESSED(KEY::CTRL))
	{
		float Speed = m_CamSpeed;

		if (KEY_PRESSED(KEY::LSHIFT))
		{
			Speed *= 3.f;
		}

		Vec3 vFront = Transform()->GetWorldDir(DIR::FRONT);
		Vec3 vRight = Transform()->GetWorldDir(DIR::RIGHT);

		Vec3 vPos = Transform()->GetRelativePos();

		if (KEY_PRESSED(KEY::W))
		{
			vPos += vFront * DT * Speed;
		}

		if (KEY_PRESSED(KEY::S))
		{
			vPos -= vFront * DT * Speed;
		}

		if (KEY_PRESSED(KEY::A))
		{
			vPos -= vRight * DT * Speed;
		}

		if (KEY_PRESSED(KEY::D))
		{
			vPos += vRight * DT * Speed;
		}

		Transform()->SetRelativePos(vPos);

		if (KEY_PRESSED(KEY::RBTN))
		{
			CKeyMgr::GetInst()->MouseCapture(true);

			// 마우스가 이동하는 방향
			//vDir.x; ==> y축 회전;
			//vDir.y; ==> x축 회전
			Vec2 vDir = CKeyMgr::GetInst()->GetDragDir();

			Vec3 vRot = Transform()->GetRelativeRoatation();
			vRot.y += vDir.x * XM_PI * DT * 5.f;
			vRot.x += vDir.y * XM_PI * DT * 5.f;
			Transform()->SetRelativeRotation(vRot);
		}
		else if (KEY_RELEASED(KEY::RBTN))
		{
			CKeyMgr::GetInst()->MouseCapture(false);
		}
	}
}
