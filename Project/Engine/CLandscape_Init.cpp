#include "pch.h"
#include "CLandscape.h"
#include "CAssetMgr.h"
#include "CTexture.h"
#include "CDevice.h"
#include "CStructuredBuffer.h"

void CLandscape::Init()
{
	// LandScape 전용 Mesh 생성
	CreateMesh();
	// LandScape 전용 재질 참조
	Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LandScapeMtrl");
	SetMaterial(pMtrl);
	// LandScape 전용 컴퓨트 셰이더
	CreateComputeShader();
	// LandScape 용 텍스쳐 생성 및 로딩
	CreateTextureAndStructuredBuffer();
	
	//AddBrushTexture(CAssetMgr::GetInst()->FindAsset<CTexture>(L"Soft"));
	AddBrushTexture(CAssetMgr::GetInst()->Load<CTexture>(L"Soft", L"texture\\Engine\\Brush\\Soft.png"));
	
	// TODO
	CreateHeightMap(1024, 1024);
	m_BrushIdx = 0;
}

void CLandscape::CreateMesh()
{
	Vtx v;
	std::vector<Vtx> vecVtx;
	for (UINT Row = 0; Row < m_FaceZ + 1; ++Row)
	{
		for (UINT Col = 0; Col < m_FaceX + 1; ++Col)
		{
			v.vPos = Vec3((float)Col, 0.f, (float)Row);
			v.vNormal = Vec3(0.f, 1.f, 0.f);
			v.vTangent = Vec3(1.f, 0.f, 0.f);
			v.vBinormal = Vec3(0.f, 0.f, -1.f);
			vecVtx.emplace_back(v);
		}
	}

	std::vector<UINT> vecIdx;
	for (UINT Row = 0; Row < m_FaceZ; ++Row)
	{
		for (UINT Col = 0; Col < m_FaceX; ++Col)
		{
			// 0
			// | \
			// 2--1
			vecIdx.push_back((Row * (m_FaceX + 1)) + Col + m_FaceX + 1);
			vecIdx.push_back((Row * (m_FaceX + 1)) + Col + 1);
			vecIdx.push_back((Row * (m_FaceX + 1)) + Col);
			// 1--2 
			//  \ |
			//    0
			vecIdx.push_back((Row * (m_FaceX + 1)) + Col + 1);
			vecIdx.push_back((Row * (m_FaceX + 1)) + Col + m_FaceX + 1);
			vecIdx.push_back((Row * (m_FaceX + 1)) + Col + m_FaceX + 1 + 1);
		}
	}

	Ptr<CMesh> pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	SetMesh(pMesh);
}

void CLandscape::CreateComputeShader()
{
	// HeightMapCS 가 있으면 찾아오고 없으면 컴파일해서 등록한다.
	m_HeightmapCS = (CHeightmapCS*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"HeightmapCS").Get();
	if (nullptr == m_HeightmapCS)
	{
		m_HeightmapCS = new CHeightmapCS;
		CAssetMgr::GetInst()->AddAsset<CComputeShader>(L"HeightmapCS", m_HeightmapCS.Get());
	}
}

void CLandscape::CreateTextureAndStructuredBuffer()
{

}

void CLandscape::CreateHeightMap(UINT _Width, UINT _Height)
{
	m_IsHeightMapCreated = true;
	m_Heightmap = CAssetMgr::GetInst()->CreateTexture(L"LandscapeHeightmap", _Width, _Height
		, DXGI_FORMAT_R32_FLOAT
		, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS);
}