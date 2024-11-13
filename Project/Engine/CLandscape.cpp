#include "pch.h"
#include "CLandscape.h"
#include "CAssetMgr.h"
#include "CTransform.h"
#include "CKeyMgr.h"

CLandscape::CLandscape()
	: CRenderComponent(COMPONENT_TYPE::LANDSCAPE)
{
	SetFace(m_FaceX, m_FaceZ);
	Init();
}

void CLandscape::FinalTick()
{
	if (KEY_TAP(KEY::_6))
	{
		++m_BrushIdx;
		if (m_vecBrush.size() <= m_BrushIdx)
			m_BrushIdx = 0;
	}

	if (m_IsHeightMapCreated && KEY_PRESSED(KEY::LBTN))
	{
		// 높이맵 설정
		m_HeightmapCS->SetBrushPos(Vec2(0.5f, 0.5f));
		m_HeightmapCS->SetBrushScale(m_BrushScale);
		m_HeightmapCS->SetHeightMap(m_Heightmap);
		m_HeightmapCS->SetBrushTex(m_vecBrush[m_BrushIdx]);
		m_HeightmapCS->Execute();
	}
}

void CLandscape::Render()
{
	Transform()->Bind();
	//GetMaterial()->GetShader()->SetRSType(RS_TYPE::WIRE_FRAME);
	
	// 지형의 면 개수
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0,		m_FaceX);
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_1,		m_FaceZ);
	GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0,	m_TessLevel);
	// 지형에 적용시킬 높이맵
	GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_Heightmap);

	// 재질 바인딩
	GetMaterial()->Bind();

	// 렌더링
	GetMesh()->Render();
}

void CLandscape::SetFace(int _X, int _Z)
{
	m_FaceX = _X;
	m_FaceZ = _Z;
	CreateMesh();
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LandScapeMtrl"));
}

void CLandscape::SaveToFile(FILE* _File)
{
}

void CLandscape::LoadFromFile(FILE* _File)
{
}