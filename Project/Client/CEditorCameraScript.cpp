#include "pch.h"
#include "CEditorCameraScript.h"

CEditorCameraScript::CEditorCameraScript()
	: CScript(-1)
	, m_Speed(500.f)
{

}

CEditorCameraScript::~CEditorCameraScript()
{

}

void CEditorCameraScript::Tick()
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

	DrawDebugRect(Vec3(0, 0, 0), Vec3(1664, 936, 1), Vec3(0, 0, 0), Vec4(1, 1, 1, 1), 0.f, false);
}



void CEditorCameraScript::OrthoGraphicMove()
{
	
	if (KEY_PRESSED(KEY::CTRL))
	{
		float Speed = m_Speed;

		Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
		Vec3 vPos = Transform()->GetRelativePos();
		if (KEY_PRESSED(KEY::O))
		{
			float scale = Camera()->GetScale();
			Camera()->SetScale(float(scale + 0.005));
		}

		if (KEY_PRESSED(KEY::I))
		{
			float scale = Camera()->GetScale();
			Camera()->SetScale(float(scale - 0.005));
		}

		if (KEY_PRESSED(KEY::W))
		{
			vPos.y += EngineDT * Speed;
		}

		if (KEY_PRESSED(KEY::S))
		{
			vPos.y -= EngineDT * Speed;
		}

		if (KEY_PRESSED(KEY::A))
		{
			vPos.x -= EngineDT * Speed;
		}

		if (KEY_PRESSED(KEY::D))
		{
			vPos.x += EngineDT * Speed;
		}

		Transform()->SetRelativePos(vPos);
	}
	
}

void CEditorCameraScript::PerspectiveMove()
{
	if (KEY_PRESSED(KEY::CTRL))
	{
		float Speed = m_Speed;

		if (KEY_PRESSED(KEY::LSHIFT))
		{
			Speed *= 3.f;
		}

		Vec3 vFront = Transform()->GetWorldDir(DIR::FRONT);
		Vec3 vRight = Transform()->GetWorldDir(DIR::RIGHT);

		Vec3 vPos = Transform()->GetRelativePos();

		if (KEY_PRESSED(KEY::W))
		{
			vPos += vFront * EngineDT * Speed;
		}

		if (KEY_PRESSED(KEY::S))
		{
			vPos -= vFront * EngineDT * Speed;
		}

		if (KEY_PRESSED(KEY::A))
		{
			vPos -= vRight * EngineDT * Speed;
		}

		if (KEY_PRESSED(KEY::D))
		{
			vPos += vRight * EngineDT * Speed;
		}

		Transform()->SetRelativePos(vPos);

		if (KEY_PRESSED(KEY::RBTN))
		{
			CKeyMgr::GetInst()->MouseCapture(true);

			// ���콺�� �̵��ϴ� ����
			//vDir.x; ==> y�� ȸ��;
			//vDir.y; ==> x�� ȸ��
			Vec2 vDir = CKeyMgr::GetInst()->GetDragDir();

			Vec3 vRot = Transform()->GetRelativeRoatation();
			vRot.y += vDir.x * XM_PI * EngineDT * 10.f;
			vRot.x += vDir.y * XM_PI * EngineDT * 10.f;
			Transform()->SetRelativeRotation(vRot);
		}
		else if (KEY_RELEASED(KEY::RBTN))
		{
			CKeyMgr::GetInst()->MouseCapture(false);
		}
	}
}
