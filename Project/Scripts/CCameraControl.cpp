#include "spch.h"
#include "CCameraControl.h"

CCameraControl::CCameraControl()
	: CScript(SCRIPT_TYPE::CAMERACONTROL)
{
}

void CCameraControl::Begin()
{
	Camera()->SetFrustumDebug(false);
}

void CCameraControl::Tick()
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
		if (CEditorMgr::GetInst()->IsViewportFocused() && CEditorMgr::GetInst()->IsViewportHovered())
		{
			if (PROJ_TYPE::ORTHOGRAPHIC == Camera()->GetProjType())
				Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
			else
				Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
		}
	}
}

void CCameraControl::SaveToFile(FILE* _File)
{
}

void CCameraControl::LoadFromFile(FILE* _File)
{
}

void CCameraControl::OrthoGraphicMove()
{
	if (KEY_PRESSED(KEY::CTRL))
	{
		Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
		Vec3 vPos = Transform()->GetRelativePos();
		if (KEY_PRESSED(KEY::O))
		{
			float& scale = Camera()->GetScaleRef();
			scale += 1 * EngineDT;
		}

		if (KEY_PRESSED(KEY::I))
		{
			float& scale = Camera()->GetScaleRef();
			scale -= 1 * EngineDT;
		}

		if (KEY_PRESSED(KEY::W))
		{
			vPos.y += EngineDT * m_Speed;
		}
		else if (KEY_PRESSED(KEY::S))
		{
			vPos.y -= EngineDT * m_Speed;
		}

		if (KEY_PRESSED(KEY::A))
		{
			vPos.x -= EngineDT * m_Speed;
		}
		else if (KEY_PRESSED(KEY::D))
		{
			vPos.x += EngineDT * m_Speed;
		}

		Transform()->SetRelativePos(vPos);
	}
}

void CCameraControl::PerspectiveMove()
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

			Vec3 vRot = Transform()->GetRelativeRotation();
			vRot.y += vDir.x * XM_PI * DT * 5.f;
			vRot.x += vDir.y * XM_PI * DT * 5.f;
			Transform()->SetRelativeRotation(vRot);
		}
		else if (KEY_RELEASED(KEY::RBTN))
		{
			CKeyMgr::GetInst()->MouseCapture(false);
		}

		if (KEY_TAP(KEY::_7))
		{
			const Vec3& camPos = Transform()->GetRelativePosRef();
			string temp = std::to_string(camPos.x) + " , " + std::to_string(camPos.y) + " , " + std::to_string(camPos.z);
			EDITOR_TRACE(u8"ī�޶� ��ġ :");
			EDITOR_TRACE(temp);
		}
	}
}