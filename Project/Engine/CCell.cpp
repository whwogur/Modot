#include "pch.h"
#include "CCell.h"

CCell::CCell()
{
}

HRESULT CCell::Initialize(const Vec3* _Points, int _Idx)
{
	m_iIndex = _Idx;

	memcpy(m_vPoints, _Points, sizeof(Vec3) * POINT_END);

	Vec3		vLines[LINE_END];

	XMStoreFloat3(&vLines[LINE_AB], XMLoadFloat3(&m_vPoints[POINT_B]) - XMLoadFloat3(&m_vPoints[POINT_A]));
	m_vNormals[LINE_AB] = Vec3(vLines[LINE_AB].z * -1.f, 0.f, vLines[LINE_AB].x);

	XMStoreFloat3(&vLines[LINE_BC], XMLoadFloat3(&m_vPoints[POINT_C]) - XMLoadFloat3(&m_vPoints[POINT_B]));
	m_vNormals[LINE_BC] = Vec3(vLines[LINE_BC].z * -1.f, 0.f, vLines[LINE_BC].x);

	XMStoreFloat3(&vLines[LINE_CA], XMLoadFloat3(&m_vPoints[POINT_A]) - XMLoadFloat3(&m_vPoints[POINT_C]));
	m_vNormals[LINE_CA] = Vec3(vLines[LINE_CA].z * -1.f, 0.f, vLines[LINE_CA].x);

	return S_OK;
}

bool CCell::ComparePoints(const Vec3* _From, const Vec3* _To)
{
	if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(_From)))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(_To)))
			return true;

		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), XMLoadFloat3(_To)))
			return true;
	}

	if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(_From)))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), XMLoadFloat3(_To)))
			return true;

		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(_To)))
			return true;
	}

	if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), XMLoadFloat3(_From)))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(_To)))
			return true;

		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(_To)))
			return true;
	}

	return false;
}

bool CCell::isIn(const Vec3& _Position, int* _pNeighborIdx, Vec3* _LastNormal)
{
	for (UINT i = 0; i < LINE_END; ++i)
	{
		Vec3 vDir = XMVector3Normalize(XMFLOAT3{ _Position.x, _Position.y, _Position.z } - XMLoadFloat3(&m_vPoints[i]));
		if (0 < XMVectorGetX(XMVector3Dot(vDir, XMVector3Normalize(XMLoadFloat3(&m_vNormals[i])))))
		{
			*_pNeighborIdx = m_iNeighborIndices[i];
			if (_LastNormal != nullptr)
				*_LastNormal = m_vNormals[i];
			return false;
		}
		if (_LastNormal != nullptr)
			*_LastNormal = m_vNormals[i];
	}

	return true;
}

const Vec3& CCell::GetCenter()
{
	float PositionX = (m_vPoints[POINT_A].x + m_vPoints[POINT_B].x + m_vPoints[POINT_C].x) / 3.f;
	float PositionY = (m_vPoints[POINT_A].y + m_vPoints[POINT_B].y + m_vPoints[POINT_C].y) / 3.f;
	float PositionZ = (m_vPoints[POINT_A].z + m_vPoints[POINT_B].z + m_vPoints[POINT_C].z) / 3.f;
	return XMVectorSet(PositionX, PositionY, PositionZ, 1.f);
}
