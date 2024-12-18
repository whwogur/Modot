#include "pch.h"
#include "CNavigation.h"
#include "CCell.h"

CNavigation::CNavigation()
	: CComponent(COMPONENT_TYPE::NAVIGATION)
{
}

float CNavigation::Compute_Height(const Vec3& _Position, float _Offset)
{
	if (m_NaviDesc.iCurrentCellIndex == -1)
		return XMVectorGetY(_Position);

	Vec3 PointA = XMLoadFloat3(&m_Cells[m_NaviDesc.iCurrentCellIndex]->Get_PointValue(CCell::POINT_A));
	PointA = XMVectorSetW(PointA, 1.f);
	Vec3 PointB = XMLoadFloat3(&m_Cells[m_NaviDesc.iCurrentCellIndex]->Get_PointValue(CCell::POINT_B));
	PointB = XMVectorSetW(PointB, 1.f);
	Vec3 PointC = XMLoadFloat3(&m_Cells[m_NaviDesc.iCurrentCellIndex]->Get_PointValue(CCell::POINT_C));
	PointC = XMVectorSetW(PointC, 1.f);

	Vec3 vPlane = XMPlaneFromPoints(PointA, PointB, PointC);

	// _float		fHeight = (-ax - cz - d) / b;
	float fHeight = 
		(-XMVectorGetX(vPlane) * XMVectorGetX(_Position)
			- XMVectorGetZ(vPlane) * XMVectorGetZ(_Position) 
			- XMVectorGetW(vPlane)) / XMVectorGetY(vPlane) 
		+ _Offset;

	return fHeight;
}

void CNavigation::Compute_CurrentIdx_viaDistance(const Vec3& _Position)
{
	int	iIndexNum = 0;
	float fMinDistance = 9999;
	float fDistance;

	for (int i = 0; i < m_Cells.size(); ++i)
	{
		fDistance = XMVectorGetX(XMVector3Length(_Position - m_Cells[i]->GetCenter()));

		if (fMinDistance > fDistance)
		{
			fMinDistance = fDistance;
			iIndexNum = i;
		}
	}

	m_NaviDesc.iCurrentCellIndex = iIndexNum;
}

bool CNavigation::Compute_CurrentIdx_viaHeight(const Vec3& _Position)
{
	int iNeighborIndex = -1;

	//셀을 순회하여 내 위치에 xz로 셀이 존재하는 지 확인한다.
	for (int i = 0; i < m_Cells.size(); ++i)
	{
		if (m_Cells[i]->isIn(_Position, &iNeighborIndex, &m_vLastNormal))
			m_CellsForComputation.push_back(m_Cells[i]);
	}

	//존재하지 않다면 false
	if (m_CellsForComputation.size() == 0)
		return false;
	else
	{
		float fMinDistance = 9999;
		float fDistance;

		for (int i = 0; i < m_CellsForComputation.size(); ++i)
		{
			if (XMVectorGetY(_Position) < XMVectorGetY(m_CellsForComputation[i]->GetCenter()))
				continue;

			fDistance = XMVectorGetX(XMVector3Length(_Position - m_CellsForComputation[i]->GetCenter()));

			if (fMinDistance > fDistance)
			{
				fMinDistance = fDistance;
				iNeighborIndex = m_CellsForComputation[i]->GetIndex();
			}
		}

		m_NaviDesc.iCurrentCellIndex = iNeighborIndex;
		m_CellsForComputation.clear();

		return true;
	}
}

bool CNavigation::CanMove(const Vec3& _Position)
{
	if (m_NaviDesc.iCurrentCellIndex == -1)
	{
		Compute_CurrentIdx_viaDistance(_Position);
	}

	int		iNeighborIndex = -1;

	/* 현재 존재하는 쎌안에서 움직였다. */
	if (true == m_Cells[m_NaviDesc.iCurrentCellIndex]->isIn(_Position, &iNeighborIndex, &m_vLastNormal))
		return true;

	/* 현재 존재하는 쎌을 벗어난다.  */
	else
	{
		/* 나간 방향에 이웃셀이 존재한다. */
		if (0 <= iNeighborIndex)
		{
			while (true)
			{
				if (-1 == iNeighborIndex)
					return false;

				if (true == m_Cells[iNeighborIndex]->isIn(_Position, &iNeighborIndex, &m_vLastNormal))
					break;
			}

			m_NaviDesc.iCurrentCellIndex = iNeighborIndex;
			return true;
		}

		/* 나간 방향에 이웃셀이 존재하지않는다. . */
		else
		{
			/*슬라이딩을 위한 리턴을 정의해도 된다. */
			return false;
		}
	}
	return false;
}

UINT CNavigation::Get_CurrentCelltype()
{
	if (m_NaviDesc.iCurrentCellIndex == -1)
		return false;

	return static_cast<UINT>(m_Cells[m_NaviDesc.iCurrentCellIndex]->GetCellType());
}

const Vec3& CNavigation::Get_CurrentCellCenter()
{
	return m_Cells[m_NaviDesc.iCurrentCellIndex]->GetCenter();
}

void CNavigation::FinalTick()
{
}

void CNavigation::SaveToFile(FILE* _File)
{
}

void CNavigation::LoadFromFile(FILE* _File)
{
}

HRESULT CNavigation::InitializeNeighbors()
{
	for (const auto& From : m_Cells)
	{
		for (const auto& To : m_Cells)
		{
			if (From == To)
				continue;

			if (true == To->ComparePoints(From->Get_Point(CCell::POINT_A), From->Get_Point(CCell::POINT_B)))
			{
				From->InitializeNeighbor(CCell::LINE_AB, To);
			}

			if (true == To->ComparePoints(From->Get_Point(CCell::POINT_B), From->Get_Point(CCell::POINT_C)))
			{
				From->InitializeNeighbor(CCell::LINE_BC, To);
			}

			if (true == To->ComparePoints(From->Get_Point(CCell::POINT_C), From->Get_Point(CCell::POINT_A)))
			{
				From->InitializeNeighbor(CCell::LINE_CA, To);
			}
		}
	}

	return S_OK;
}
