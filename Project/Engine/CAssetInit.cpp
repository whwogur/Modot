#include "pch.h"
#include "CAssetMgr.h"
#include "CDevice.h"

void CAssetMgr::Init()
{
	MD_PROFILE_FUNCTION();
	CreateEngineMesh();

	CreateEngineTexture();

	CreateEngineSprite();

	CreateEngineGraphicShader();

	CreateEngineComputeShader();

	CreateEngineMaterial();
}

void CAssetMgr::CreateEngineMesh()
{
	MD_PROFILE_FUNCTION();
	Ptr<CMesh> pMesh = nullptr;

	Vtx v;

	// PointMesh
	pMesh = new CMesh;
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);

	UINT i = 0;
	pMesh->Create(&v, 1, &i, 1);
	AddAsset(L"PointMesh", pMesh);

	Vtx arrVtx[4] = {};

	arrVtx[0].vPos = Vec3(-0.5f, 0.5f, 0.f);
	arrVtx[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVtx[0].vUV = Vec2(0.f, 0.f);

	arrVtx[1].vPos = Vec3(0.5f, 0.5f, 0.f);
	arrVtx[1].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrVtx[1].vUV = Vec2(1.f, 0.f);

	arrVtx[2].vPos = Vec3(0.5f, -0.5f, 0.f);
	arrVtx[2].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrVtx[2].vUV = Vec2(1.f, 1.f);

	arrVtx[3].vPos = Vec3(-0.5f, -0.5f, 0.f);
	arrVtx[3].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVtx[3].vUV = Vec2(0.f, 1.f);

	// Index 버퍼
	UINT arrIdx[6] = {};
	arrIdx[0] = 0;	arrIdx[1] = 1;	arrIdx[2] = 2;
	arrIdx[3] = 0; 	arrIdx[4] = 2;	arrIdx[5] = 3;

	pMesh = new CMesh;
	pMesh->Create(arrVtx, 4, arrIdx, 6);
	pMesh->SetEngineAsset();
	AddAsset(L"RectMesh", pMesh);

	// RectMesh_Debug
	arrIdx[0] = 0;	arrIdx[1] = 1;	arrIdx[2] = 2; arrIdx[3] = 3; arrIdx[4] = 0;

	pMesh = new CMesh;
	pMesh->Create(arrVtx, 4, arrIdx, 5);
	pMesh->SetEngineAsset();
	AddAsset(L"RectMesh_Debug", pMesh);

	// CircleMesh 
	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;

	int Slice = 40;
	float fTheta = XM_2PI / Slice;
	float Radius = 0.5f;

	// 중심점
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vUV = Vec2(0.5f, 0.5f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	vecVtx.push_back(v);

	// 테두리
	float Angle = 0.f;
	for (int i = 0; i < Slice + 1; ++i)
	{
		v.vPos = Vec3(Radius * cosf(Angle), Radius * sinf(Angle), 0.f);
		v.vUV = Vec2(v.vPos.x + 0.5f, -(v.vPos.y - 0.5f));
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		vecVtx.push_back(v);

		Angle += fTheta;
	}

	// 인덱스
	for (int i = 0; i < Slice; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	pMesh->SetEngineAsset();
	AddAsset(L"CircleMesh", pMesh);

	// CircleMesh_Debug
	vecIdx.clear();
	for (size_t i = 1; i < vecVtx.size(); ++i)
	{
		vecIdx.push_back((UINT)i);
	}

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	pMesh->SetEngineAsset();
	AddAsset(L"CircleMesh_Debug", pMesh);
}

void CAssetMgr::CreateEngineTexture()
{
	MD_PROFILE_FUNCTION();
	// PostProcess 용 텍스쳐
	Vec2 Resolution = CDevice::GetInst()->GetResolution();
	CreateTexture(L"PostProcessTex", (UINT)Resolution.x, (UINT)Resolution.y
		, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);

	Load<CTexture>(L"noise_01", L"texture\\noise\\noise_01.png");
	Load<CTexture>(L"noise_02", L"texture\\noise\\noise_02.png");
	Load<CTexture>(L"noise_03", L"texture\\noise\\noise_03.jpg");
	Load<CTexture>(L"ComponentIcons", L"texture\\ComponentIcons.png");
	Load<CTexture>(L"ScriptIcon", L"texture\\ScriptIcon.png");
	Load<CTexture>(L"Modot_Logo", L"texture\\Modot_Logo.png");
	Load<CTexture>(L"RedDiscFx", L"texture\\RedDiscFx.png");
	Load<CTexture>(L"DiscDonut", L"texture\\DiscDonut.png");
	Load<CTexture>(L"alpha01", L"texture\\alpha01.tga");
	Load<CTexture>(L"noise01", L"texture\\noise01.tga");
	Load<CTexture>(L"fire01", L"texture\\fire01.tga");
}

void CAssetMgr::CreateEngineSprite()
{
	wstring strContentPath = CPathMgr::GetInst()->GetContentPath();
	wstring strSolutionPath = CPathMgr::GetInst()->GetSolutionPath();

	/*Ptr<CTexture> pAtlasTex = Load<CTexture>(L"Momo_192", L"texture\\Momo_192.png");

	Ptr<CSprite> pSprite = nullptr;

	for (int i = 0; i < 7; ++i)
	{
		wchar_t szKey[50] = {};
		swprintf_s(szKey, 50, L"Momo_Idle_%d", i);

		pSprite = new CSprite;
		pSprite->Create(pAtlasTex, Vec2((float)i * 192.f, 0.f), Vec2(192.f, 192.f));
		pSprite->SetBackground(Vec2(192.f, 192.f));
		AddAsset(szKey, pSprite);
		
		wstring relPath(L"sprite\\");
		relPath += szKey;
		relPath += L".sprite";
		pSprite->SetRelativePath(relPath);
		pSprite->Save(relPath);
	}

	Ptr<CAnimation> pAnimation = new CAnimation;

	for (int i = 0; i < 7; ++i)
	{
		wchar_t Buffer[50] = {};
		swprintf_s(Buffer, 50, L"Momo_Idle_%d", i);
		pAnimation->AddSprite(FindAsset<CSprite>(Buffer));
	}

	AddAsset(L"Momo_Idle", pAnimation);
	pAnimation->Save(L"animation\\Momo_Idle.anim");*/

	//Ptr<CAnimation> pAnimation = new CAnimation;
	//pAnimation->Load(L"animation\\Momo_Idle.anim");
	//AddAsset(L"Momo_Idle", pAnimation);

	//// Engine Default / New Assets
	//Ptr<CTexture> checkerboard = Load<CTexture>(L"Checkerboard", L"texture\\Checkerboard.png");
	//Ptr<CSprite> defaultSprite = new CSprite;
	//defaultSprite->Load(L"sprite\\DefaultSprite.sprite");
	//AddAsset(L"DefaultSprite", defaultSprite);
	//defaultSprite->Save(L"sprite\\DefaultSprite.sprite");

	//Ptr<CAnimation> pNewAnimation = new CAnimation;
	//pNewAnimation->Load(L"animation\\NewAnimation.anim");
	//AddAsset(L"NewAnimation", pNewAnimation);
	//pNewAnimation->Save(L"animation\\NewAnimation.anim");
}

void CAssetMgr::CreateEngineGraphicShader()
{
	MD_PROFILE_FUNCTION();
	Ptr<CGraphicShader> pShader = nullptr;
	// Std2DShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);
	pShader->AddTexParam(TEX_0, "OutputTexture");
	AddAsset(L"Std2DShader", pShader);

	// Std2DAlphaBlend
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D_Alphablend");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddAsset(L"Std2DAlphaBlendShader", pShader);


	// DebugShapeShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\debug.fx", "VS_DebugShape");
	pShader->CreatePixelShader(L"shader\\debug.fx", "PS_DebugShape");

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEBUG);

	AddAsset(L"DebugShapeShader", pShader);

	// TileMapShader
	pShader = new CGraphicShader;

	pShader->CreateVertexShader(L"shader\\tilemap.fx", "VS_TileMap");
	pShader->CreatePixelShader(L"shader\\tilemap.fx", "PS_TileMap");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ALPHABLEND_COVERAGE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

	AddAsset(L"TileMapShader", pShader);

	// Shockwave
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_Shockwave");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_Shockwave");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	pShader->AddScalarParam(SCALAR_PARAM::VEC4_0, "float4");
	pShader->AddTexParam(TEX_0, u8"배경");
	AddAsset(L"ShockwaveShader", pShader);

	// Distortion
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_Distortion");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_Distortion");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	AddAsset(L"DistortionShader", pShader);

	// Ripple
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_Ripple");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_Ripple");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	pShader->AddTexParam(TEX_0, u8"샘플링 텍스처");
	AddAsset(L"RippleShader", pShader);

	// SmallRipple
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_SmallRipple");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_SmallRipple");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	AddAsset(L"SmallRippleShader", pShader);

	// Fire
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\myFX.fx", "VS_Fire");
	pShader->CreatePixelShader(L"shader\\myFX.fx", "PS_Fire");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	pShader->AddScalarParam(SCALAR_PARAM::VEC4_0, "Scroll Speed");
	pShader->AddScalarParam(SCALAR_PARAM::VEC4_1, "Noise Tex");
	pShader->AddScalarParam(SCALAR_PARAM::VEC2_0, "Distortion 1");
	pShader->AddScalarParam(SCALAR_PARAM::VEC2_1, "Distortion 2");
	pShader->AddScalarParam(SCALAR_PARAM::VEC2_2, "Distortion 3");
	pShader->AddScalarParam(SCALAR_PARAM::FLOAT_0, "UDistortion 1");
	pShader->AddScalarParam(SCALAR_PARAM::FLOAT_1, "UDistortion 2");
	AddAsset(L"FireShader", pShader);

	// ParticleShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\particle.fx", "VS_Particle");
	pShader->CreateGeometryShader(L"shader\\particle.fx", "GS_Particle");
	pShader->CreatePixelShader(L"shader\\particle.fx", "PS_Particle");
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_PARTICLE);
	pShader->AddTexParam(TEX_PARAM::TEX_0, u8"텍스처");
	pShader->AddScalarParam(SCALAR_PARAM::VEC4_1, "UV");
	AddAsset(L"ParticleRenderShader", pShader);
}

#include "CParticleTickCS.h"
void CAssetMgr::CreateEngineComputeShader()
{
	// ParticleTick
	Ptr<CComputeShader> pCS = nullptr;

	pCS = new CParticleTickCS;
	AddAsset<CComputeShader>(L"ParticleTickCS", pCS);
}


void CAssetMgr::CreateEngineMaterial()
{
	MD_PROFILE_FUNCTION();
	Ptr<CMaterial>	pMtrl = nullptr;

	// Std2DMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset(L"Std2DMtrl", pMtrl);

	// Std2DAlphaBlendMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DAlphaBlendShader"));
	AddAsset(L"Std2DAlphaBlendMtrl", pMtrl);

	// DebugShapeMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"DebugShapeShader"));
	AddAsset(L"DebugShapeMtrl", pMtrl);

	// TileMapMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"TileMapShader"));
	AddAsset(L"TileMapMtrl", pMtrl);

	// ParticleRenderMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"ParticleRenderShader"));
	pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"DiscDonut"));
	pMtrl->SetScalarParam(VEC4_1, Vec4(0, 0, 1, 1));
	AddAsset(L"ParticleRenderMtrl", pMtrl);

	// GrayFilterMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"ShockwaveShader"));
	pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"PostProcessTex"));
	pMtrl->SetTexParam(TEX_1, FindAsset<CTexture>(L"RedDiscFx"));
	AddAsset(L"ShockwaveMtrl", pMtrl);

	// DistortionMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"DistortionShader"));
	pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"PostProcessTex"));
	pMtrl->SetTexParam(TEX_1, FindAsset<CTexture>(L"noise_01"));
	pMtrl->SetTexParam(TEX_2, FindAsset<CTexture>(L"noise_02"));
	pMtrl->SetTexParam(TEX_3, FindAsset<CTexture>(L"noise_03"));
	AddAsset(L"DistortionMtrl", pMtrl);

	// RippleMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"RippleShader"));
	pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"RedDiscFx"));
	pMtrl->SetTexParam(TEX_1, FindAsset<CTexture>(L"DiscDonut"));
	AddAsset(L"RippleMtrl", pMtrl);

	// SmallRippleMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"SmallRippleShader"));
	pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"PostProcessTex"));
	AddAsset(L"SmallRippleMtrl", pMtrl);

	//// Fire
	//pMtrl = new CMaterial();
	//pMtrl->SetShader(FindAsset<CGraphicShader>(L"FireShader"));
	//pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"fire01"));
	//pMtrl->SetTexParam(TEX_1, FindAsset<CTexture>(L"noise01"));
	//pMtrl->SetTexParam(TEX_2, FindAsset<CTexture>(L"alpha01"));
	//AddAsset(L"FireMtrl", pMtrl);
	//Load<CMaterial>(L"FireMtrl", L"material\\FireMtrl.mtrl");
}
