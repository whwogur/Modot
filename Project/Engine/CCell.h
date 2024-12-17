#pragma once
class CCell
{
public:
	CCell();
	~CCell() = default;
public:
	enum POINT { POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINE { LINE_AB, LINE_BC, LINE_CA, LINE_END };
	enum CELLTYPE { ACCESSIBLE, ONLYJUMP, DROP, UPDOWN };

public:
	const Vec3* Get_Point(POINT ePoint) { return &m_vPoints[ePoint]; }
	const Vec3& Get_PointValue(POINT ePoint) const { return m_vPoints[ePoint]; }

	void InitializeNeighbor(LINE eLine, const CCell* pCell)
	{
		m_iNeighborIndices[eLine] = pCell->m_iIndex;
	}

public:
	HRESULT			Initialize(const Vec3* _Points, int _Idx);
	bool			ComparePoints(const Vec3* _From, const Vec3* _To);
	bool			isIn(const Vec3& _Position, int* _pNeighborIdx, Vec3* _LastNormal = nullptr);

	void			SetCellType(CELLTYPE eType) { m_eCellType = eType; }
	CELLTYPE		GetCellType() { return m_eCellType; }
	const Vec3&		GetCenter();
	int				GetIndex() { return m_iIndex; }
	
private:
	CELLTYPE			m_eCellType = ACCESSIBLE;

	int					m_iIndex = 0;
	Vec3				m_vPoints[POINT_END];
	Vec3				m_vNormals[LINE_END];
	int					m_iNeighborIndices[LINE_END] = { -1, -1, -1 };
};

