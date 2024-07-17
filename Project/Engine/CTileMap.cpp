#include "pch.h"
#include "CTileMap.h"

#include "CTransform.h"
#include "CAssetMgr.h"
#include "CStructuredBuffer.h"

CTileMap::CTileMap()
	: CRenderComponent(COMPONENT_TYPE::TILEMAP)
	, m_Row(1)
	, m_Col(1)
	, m_AtlasMaxRow(0)
	, m_AtlasMaxCol(0)
	, m_Buffer(nullptr)
{
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"TileMapMtrl"));
	m_Buffer = std::make_shared<CStructuredBuffer>();
}

void CTileMap::FinalTick()
{
}

void CTileMap::Render()
{
	// 타일의 정보를 구조화버퍼를 통해서 t 레지스터에 바인딩 시킨다.
	m_Buffer->SetData(m_vecTileInfo.data(), sizeof(tTileInfo) * m_Row * m_Col);
	m_Buffer->Bind(15);

	GetMaterial()->SetTexParam(TEX_0, m_TileAtlas);
	GetMaterial()->SetScalarParam(INT_1, m_AtlasMaxRow);
	GetMaterial()->SetScalarParam(INT_2, m_AtlasMaxCol);
	GetMaterial()->SetScalarParam(VEC2_1, Vec2(m_Col, m_Row));
	GetMaterial()->SetScalarParam(VEC2_0, m_AtlasTileSliceUV);
	GetMaterial()->Bind();
	Transform()->Bind();
	GetMesh()->Render();
}

void CTileMap::SetRowCol(UINT _Row, UINT _Col)
{
	m_Row = _Row;
	m_Col = _Col;

	ChangeTileMapSize();

	// 타일 개수
	UINT TileCount = m_Row * m_Col;

	// 타일 정보를 저장하는 벡터가 타일개수보다 사이즈가 작으면 리사이즈
	if (m_vecTileInfo.size() < TileCount)
	{
		m_vecTileInfo.resize(TileCount);
	}

	// 타일정보를 전달받아서 t 레지스터에 전달시킬 구조화버퍼가 타일 전체 데이터 사이즈보다 작으면 리사이즈
	if (m_Buffer->GetElementSize() < sizeof(tTileInfo) * TileCount)
	{
		m_Buffer->Create(sizeof(tTileInfo), TileCount);
	}
}

void CTileMap::SetTileSize(Vec2 _Size)
{
	m_TileSize = _Size;

	ChangeTileMapSize();
}

void CTileMap::ChangeTileMapSize()
{
	Vec2 vSize = m_TileSize * Vec2((float)m_Col, (float)m_Row);
	Transform()->SetRelativeScale(vSize.x, vSize.y, 1.f);
}

void CTileMap::SetAtlasTexture(Ptr<CTexture> _Atlas)
{
	m_TileAtlas = _Atlas;
	m_AtlasResolution = Vec2((float)_Atlas->Width(), (float)_Atlas->Height());

	SetAtlasTileSize(m_AtlasTileSize);
}

void CTileMap::SetAtlasTileSize(Vec2 _TileSize)
{
	m_AtlasTileSize = _TileSize;

	if (nullptr != m_TileAtlas)
	{
		m_AtlasTileSliceUV = m_AtlasTileSize / m_AtlasResolution;

		m_AtlasMaxCol = int(m_AtlasResolution.x / m_AtlasTileSize.x);
		m_AtlasMaxRow = int(m_AtlasResolution.y / m_AtlasTileSize.y);
	}
}