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
	SetMaterial(pMtrl, 0);
	// LandScape 전용 컴퓨트 셰이더
	CreateComputeShader();
	// LandScape 용 텍스쳐 생성 및 로딩
	CreateTextureAndStructuredBuffer();
	
	AddBrushTexture(CAssetMgr::GetInst()->Load<CTexture>(L"brush0", L"texture\\Engine\\Brush\\brush0.png"));
	AddBrushTexture(CAssetMgr::GetInst()->Load<CTexture>(L"brush1", L"texture\\Engine\\Brush\\brush1.png"));
	AddBrushTexture(CAssetMgr::GetInst()->Load<CTexture>(L"brush2", L"texture\\Engine\\Brush\\brush2.png"));
	
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
			v.vPos		= Vec3((float)Col, 0.f, (float)Row);
			v.vNormal	= Vec3(0.f, 1.f, 0.f);
			v.vTangent	= Vec3(1.f, 0.f, 0.f);
			v.vBinormal = Vec3(0.f, 0.f, -1.f);
			v.vUV		= Vec2((float)Col, (float)m_FaceZ - Row);

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

	// RaycastCS 생성
	m_RaycastCS = (CRaycastCS*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"RaycastCS").Get();
	if (nullptr == m_RaycastCS)
	{
		m_RaycastCS = new CRaycastCS;
		CAssetMgr::GetInst()->AddAsset<CComputeShader>(L"RaycastCS", m_RaycastCS.Get());
	}

	// WeightMapCS 생성
	m_WeightmapCS = (CWeightMapCS*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"WeightMapCS").Get();
	if (nullptr == m_WeightmapCS)
	{
		m_WeightmapCS = new CWeightMapCS;
		CAssetMgr::GetInst()->AddAsset<CComputeShader>(L"WeightMapCS", m_WeightmapCS.Get());
	}
}

void CLandscape::CreateTextureAndStructuredBuffer()
{
	// Raycast 결과를 받는 용도의 구조화버퍼
	m_RaycastOut = std::make_shared<CStructuredBuffer>();
	m_RaycastOut->Create(sizeof(tRaycastOut), 1, SB_TYPE::SRV_UAV, true);

	// LandScape 용 텍스쳐 로딩
	m_ColorTex = CAssetMgr::GetInst()->Load<CTexture>(L"LS_Color", L"texture\\LandScape\\LS_Color.dds");
	m_NormalTex = CAssetMgr::GetInst()->Load<CTexture>(L"LS_Normal", L"texture\\LandScape\\LS_Color.dds");
	m_ColorTex->GenerateMip(6);
	m_NormalTex->GenerateMip(6);

	// 가중치 WeightMap 용 StructuredBuffer
	m_Weightmap = std::make_shared<CStructuredBuffer>();
	m_WeightWidth = 1024;
	m_WeightHeight = 1024;
	m_Weightmap->Create(sizeof(tWeight8), m_WeightWidth * m_WeightHeight, SB_TYPE::SRV_UAV, true, nullptr);
}

void CLandscape::CreateHeightMap(UINT _Width, UINT _Height)
{
	m_IsHeightMapCreated = true;
	Ptr<CTexture> landscapeHM = CAssetMgr::GetInst()->FindAsset<CTexture>(L"LandscapeHeightmap");

	m_Heightmap = landscapeHM.Get() == nullptr ?
		CAssetMgr::GetInst()->CreateTexture(L"LandscapeHeightmap", _Width, _Height
		, DXGI_FORMAT_R32_FLOAT
		, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS) : landscapeHM;
}