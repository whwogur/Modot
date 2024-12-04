#include "pch.h"
#include "CEditorCameraScript.h"
#include <Engine/CRenderMgr.h>
#include <Engine/CLevelMgr.h>
#include "CEditorMgr.h"
CEditorCameraScript::CEditorCameraScript()
	: CScript(-1)
	, m_Speed(500.f)
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
		if (CRenderMgr::GetInst()->IsViewportFocused() && CRenderMgr::GetInst()->IsViewportHovered())
		{
			if (PROJ_TYPE::ORTHOGRAPHIC == Camera()->GetProjType())
				Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
			else
				Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
		}
	}
}

void CEditorCameraScript::Begin()
{
	Transform()->SetRelativePos(Vec3(0.f, 0.f, -1000.f));
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

			// 마우스가 이동하는 방향
			//vDir.x; ==> y축 회전;
			//vDir.y; ==> x축 회전
			Vec2 vDir = CKeyMgr::GetInst()->GetDragDir();

			Vec3 vRot = Transform()->GetRelativeRotation();
			vRot.y += vDir.x * XM_PI * EngineDT * (m_Speed / 200.f);
			vRot.x += vDir.y * XM_PI * EngineDT * (m_Speed / 200.f);
			Transform()->SetRelativeRotation(vRot);
		}
		else if (KEY_RELEASED(KEY::RBTN))
		{
			CKeyMgr::GetInst()->MouseCapture(false);
		}

		
		if (KEY_TAP(KEY::_7))
		{
			const Matrix& camWM = Transform()->GetWorldMat();
			string temp("EditorCamera WorldMat:\n" + MatrixToString(camWM, 2));
			EDITOR_TRACE(temp.c_str());

			const Matrix& objWM = CLevelMgr::GetInst()->FindObjectByName(L"Monster")->Transform()->GetWorldMat();
			string temp2("Monster WorldMat:\n" + MatrixToString(objWM, 2));
			EDITOR_TRACE(temp2.c_str());
		}
	}
}
