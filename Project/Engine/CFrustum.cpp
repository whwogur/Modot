#include "pch.h"
#include "CFrustum.h"
#include "CCamera.h"

CFrustum::CFrustum(CCamera* _Owner)
	: m_Owner(_Owner)
{
	/*				투영공간에서의 시야 최대 위치
	 *				7__   4 -- 5
	 *				   \ /|   /|  <-- Far
	 *					0 -- 1 6
	 *					| /  |/
	 *					3 -- 2  <-- Near
	 */
	m_arrProj[0] = Vec3(-1.f, 1.f, 0.f);
	m_arrProj[1] = Vec3(1.f, 1.f, 0.f);
	m_arrProj[2] = Vec3(1.f, -1.f, 0.f);
	m_arrProj[3] = Vec3(-1.f, -1.f, 0.f);
	m_arrProj[4] = Vec3(-1.f, 1.f, 1.f);
	m_arrProj[5] = Vec3(1.f, 1.f, 1.f);
	m_arrProj[6] = Vec3(1.f, -1.f, 1.f);
	m_arrProj[7] = Vec3(-1.f, -1.f, 1.f);
}

void CFrustum::FinalTick()
{
	// 카메라의 view, proj 역행렬
	const Matrix& matProjInv = m_Owner->GetcamProjInvRef();
	const Matrix& matViewInv = m_Owner->GetcamViewInvRef();
	Matrix matInv = matProjInv * matViewInv;
	// 투영좌표계에 있는 초기 좌표를 projinv, viewinv 를 곱해서 World 공간으로 이동시킨다.
	Vec3 arrWorld[8] = {};
	for (int i = 0; i < 8; ++i)
	{
		arrWorld[i] = XMVector3TransformCoord(m_arrProj[i], matInv);
	}
	/*				투영공간에서의 시야 최대 위치
	 *					  4 -- 5
	 *				7->	 /|   /|  <-- Far
	 *					0 -- 1 6
	 *					| /  |/ 
	 *					3 -- 2  <-- Near
	 *
	 *		월드에 있는 절두체의 8개 꼭지점을 이용해서 6개의 평면을 만들어낸다.
	 */
	m_arrFace[(UINT)FACE_TYPE::FT_NEAR]		= XMPlaneFromPoints(arrWorld[0], arrWorld[1], arrWorld[2]);
	m_arrFace[(UINT)FACE_TYPE::FT_FAR]		= XMPlaneFromPoints(arrWorld[6], arrWorld[5], arrWorld[4]);
	m_arrFace[(UINT)FACE_TYPE::FT_LEFT]		= XMPlaneFromPoints(arrWorld[7], arrWorld[4], arrWorld[0]);
	m_arrFace[(UINT)FACE_TYPE::FT_RIGHT]	= XMPlaneFromPoints(arrWorld[1], arrWorld[5], arrWorld[6]);
	m_arrFace[(UINT)FACE_TYPE::FT_TOP]		= XMPlaneFromPoints(arrWorld[4], arrWorld[5], arrWorld[1]);
	m_arrFace[(UINT)FACE_TYPE::FT_BOT]		= XMPlaneFromPoints(arrWorld[6], arrWorld[7], arrWorld[3]);

	if (m_Debug)
	{
		// 고정된 크기의 작은 프러스텀
		const float nearDist = 0.5f;
		const float farDist = 1.0f;
		const float width = 0.5f;
		const float height = 0.5f;

		Vec3 smallFrustum[8] = {
			{ -width,  height, nearDist },
			{  width,  height, nearDist },
			{  width, -height, nearDist },
			{ -width, -height, nearDist },
			{ -width,  height, farDist },
			{  width,  height, farDist },
			{  width, -height, farDist },
			{ -width, -height, farDist }
		};

		// 월드 변환
		Vec3 smallWorld[8];
		for (int i = 0; i < 8; ++i)
		{
			smallWorld[i] = XMVector3TransformCoord(smallFrustum[i], matInv);
		}

		const Vec4 debugColor = { 1.f, 1.f, 0.f, 1.f };

		// Near Plane
		DrawDebugLine(smallWorld[0], smallWorld[1], debugColor, 0.f, true);
		DrawDebugLine(smallWorld[1], smallWorld[2], debugColor, 0.f, true);
		DrawDebugLine(smallWorld[2], smallWorld[3], debugColor, 0.f, true);
		DrawDebugLine(smallWorld[3], smallWorld[0], debugColor, 0.f, true);

		// Far Plane
		DrawDebugLine(smallWorld[4], smallWorld[5], debugColor, 0.f, true);
		DrawDebugLine(smallWorld[5], smallWorld[6], debugColor, 0.f, true);
		DrawDebugLine(smallWorld[6], smallWorld[7], debugColor, 0.f, true);
		DrawDebugLine(smallWorld[7], smallWorld[4], debugColor, 0.f, true);

		// 연결선
		DrawDebugLine(smallWorld[0], smallWorld[4], debugColor, 0.f, true);
		DrawDebugLine(smallWorld[1], smallWorld[5], debugColor, 0.f, true);
		DrawDebugLine(smallWorld[2], smallWorld[6], debugColor, 0.f, true);
		DrawDebugLine(smallWorld[3], smallWorld[7], debugColor, 0.f, true);
	}
}

bool CFrustum::FrustumCheck(const Vec3& _WorldPos, float _Radius)
{
	for (int i = 0; i < 6; ++i)
	{
		Vec3 vNormal = m_arrFace[i];
		if (vNormal.Dot(_WorldPos) + m_arrFace[i].w > _Radius)
		{
			return false;
		}
	}

	return true;
}