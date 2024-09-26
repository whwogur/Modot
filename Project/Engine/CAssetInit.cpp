#include "pch.h"
#include "CAssetMgr.h"
#include "CDevice.h"

void CAssetMgr::Init()
{
	MD_PROFILE_FUNCTION();

	CreateEngineMesh();

	CreateEngineTexture();

	CreateEngineGraphicShader();

	CreateEngineComputeShader();

	CreateEngineMaterial();

	LoadSound();
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

	// Index ����
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

	// �߽���
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vUV = Vec2(0.5f, 0.5f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	vecVtx.push_back(v);

	// �׵θ�
	float Angle = 0.f;
	for (int i = 0; i < Slice + 1; ++i)
	{
		v.vPos = Vec3(Radius * cosf(Angle), Radius * sinf(Angle), 0.f);
		v.vUV = Vec2(v.vPos.x + 0.5f, -(v.vPos.y - 0.5f));
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		vecVtx.push_back(v);

		Angle += fTheta;
	}

	// �ε���
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

	// PostProcess �뵵 �ؽ���
	Vec2 Resolution = CDevice::GetInst()->GetResolution();
	CreateTexture(L"PostProcessTex", (UINT)Resolution.x, (UINT)Resolution.y, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);
	CreateTexture(L"EffectTargetTex", (UINT)(Resolution.x), (UINT)(Resolution.y), DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
	CreateTexture(L"EffectDepthStencilTex", (UINT)(Resolution.x), (UINT)(Resolution.y), DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL);
	CreateTexture(L"EffectBlurTargetTex", (UINT)(Resolution.x), (UINT)(Resolution.y), DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

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
	Load<CTexture>(L"Default-Particle", L"texture\\Default-Particle.png");
}

void CAssetMgr::CreateEngineSprite()
{
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

	pShader = new CGraphicShader;

	pShader->CreateVertexShader(L"shader\\tilemap.fx", "VS_TileMap");
	pShader->CreatePixelShader(L"shader\\tilemap.fx", "PS_TileMap");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ALPHABLEND_COVERAGE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);
	pShader->AddTexParam(TEX_0, "Atlas");
	pShader->AddScalarParam(INT_1, "AtlasMaxRow");
	pShader->AddScalarParam(INT_2, "AtlasMaxCol");
	pShader->AddScalarParam(VEC2_0, "TileSliceUV");
	pShader->AddScalarParam(VEC2_1, "Tile Col*Row");

	AddAsset(L"TileMapShader", pShader);

	// Std2DShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2DSprite");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);
	pShader->AddTexParam(TEX_0, "OutputTexture");
	pShader->AddScalarParam(VEC2_0, "LeftTopUV");
	pShader->AddScalarParam(VEC2_1, "SliceUV");
	pShader->AddScalarParam(VEC2_2, "BackgroundUV");
	pShader->AddScalarParam(VEC2_3, "OffsetUV");
	AddAsset(L"Std2DSpriteShader", pShader);

	// EffectShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Effect");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Effect");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_EFFECT);

	AddAsset(L"EffectShader", pShader);

	// EffectUIShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_EffectUI");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Effect");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_EFFECT);

	AddAsset(L"EffectUIShader", pShader);

	// TintShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2DTint");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);
	pShader->AddTexParam(TEX_0, "OutputTexture");
	pShader->AddScalarParam(VEC4_0, "Tint");
	AddAsset(L"Std2DTintShader", pShader);

	// Shockwave
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_Shockwave");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_Shockwave");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	pShader->AddScalarParam(SCALAR_PARAM::VEC4_0, "float4");
	pShader->AddScalarParam(SCALAR_PARAM::VEC4_1, "Params");
	pShader->AddScalarParam(SCALAR_PARAM::FLOAT_3, "Time");
	AddAsset(L"ShockwaveShader", pShader);

	// Godray
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_Shockwave");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_Godray");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	pShader->AddScalarParam(SCALAR_PARAM::FLOAT_0, u8"���Ǹ鼭 ���ҵǴ� ����");
	pShader->AddScalarParam(SCALAR_PARAM::FLOAT_1, u8"���� �е�");
	pShader->AddScalarParam(SCALAR_PARAM::FLOAT_2, u8"���� ����ġ - �߽ɿ��� �ܰ����� ���Ǹ鼭 ����");
	pShader->AddScalarParam(SCALAR_PARAM::INT_3, u8"Ȱ��ȭ");
	AddAsset(L"GodrayShader", pShader);

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
	AddAsset(L"ParticleRenderShader", pShader);

	// UI
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_UI");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_StdUI");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_UI);
	pShader->AddTexParam(TEX_0, "UI Tex");
	pShader->AddScalarParam(VEC4_0, "Tint");
	AddAsset(L"UIShader", pShader);

	// BlurShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_Blur");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_Blur");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	AddAsset(L"BlurShader", pShader);

	// EffectMerge
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_EffectMerge");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_EffectMerge");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	AddAsset(L"EffectMergeShader", pShader);
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

	// EffectMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"EffectShader"));
	AddAsset(L"EffectMtrl", pMtrl);

	// EffectUIMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"EffectUIShader"));
	AddAsset(L"EffectUIMtrl", pMtrl);

	// DebugShapeMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"DebugShapeShader"));
	AddAsset(L"DebugShapeMtrl", pMtrl);

	// TintMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DTintShader"));
	pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"Checkerboard"));
	pMtrl->SetScalarParam(VEC4_0, Vec4(1, 1, 1, 1));
	AddAsset(L"Std2DTintMtrl", pMtrl);

	// UIMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"UIShader"));
	pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"Checkerboard"));
	pMtrl->SetScalarParam(VEC4_0, Vec4(1, 1, 1, 1));
	AddAsset(L"UIMtrl", pMtrl);

	// TileMapMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"TileMapShader"));
	AddAsset(L"TileMapMtrl", pMtrl);

	// ParticleRenderMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"ParticleRenderShader"));
	AddAsset(L"ParticleRenderMtrl", pMtrl);

	// SpriteRenderMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DSpriteShader"));
	pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"Checkerboard"));
	pMtrl->SetScalarParam(VEC2_0, Vec2(0, 0)); // lefttop
	pMtrl->SetScalarParam(VEC2_1, Vec2(0.9f, 0.9f)); // slice
	pMtrl->SetScalarParam(VEC2_2, Vec2(0.88f, 0.9f)); // background
	pMtrl->SetScalarParam(VEC2_3, Vec2(0, 0)); // offset
	AddAsset(L"SpriteRenderMtrl", pMtrl);

	// Shockwave
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"ShockwaveShader"));
	pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"PostProcessTex"));
	AddAsset(L"ShockwaveMtrl", pMtrl);

	// Godray
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"GodrayShader"));
	pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"PostProcessTex"));
	pMtrl->SetScalarParam(FLOAT_0, 0.97f);
	pMtrl->SetScalarParam(FLOAT_1, 0.5f);
	pMtrl->SetScalarParam(FLOAT_2, 0.1f);
	pMtrl->SetScalarParam(INT_3, 0);
	AddAsset(L"GodrayMtrl", pMtrl);

	// Fire
	//pMtrl = new CMaterial();
	//pMtrl->SetShader(FindAsset<CGraphicShader>(L"FireShader"));
	//pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"fire01"));
	//pMtrl->SetTexParam(TEX_1, FindAsset<CTexture>(L"noise01"));
	//pMtrl->SetTexParam(TEX_2, FindAsset<CTexture>(L"alpha01"));
	//pMtrl->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(0.5f, 0.1, 0.2f, 0.0f));
	//pMtrl->SetScalarParam(SCALAR_PARAM::VEC4_1, Vec4(1.2f, 0.3f, 0.5f, 0.0f));
	//pMtrl->SetScalarParam(SCALAR_PARAM::VEC2_1, Vec2(0.5f, 0.3f));
	//pMtrl->SetScalarParam(SCALAR_PARAM::VEC2_2, Vec2(0.2f, 0.5f));
	//pMtrl->SetScalarParam(SCALAR_PARAM::FLOAT_0, 0.3f);
	//pMtrl->SetScalarParam(SCALAR_PARAM::FLOAT_1, 0.02f);
	//AddAsset(L"FireMtrl2", pMtrl);
	//pMtrl->Save(L"material\\FireMtrl2.mtrl");
	//Load<CMaterial>(L"FireMtrl", L"material\\FireMtrl.mtrl");

	// BlurMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"BlurShader"));
	AddAsset(L"BlurMtrl", pMtrl);

	// EffectMergeMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"EffectMergeShader"));
	AddAsset(L"EffectMergeMtrl", pMtrl);
}

void CAssetMgr::LoadSound()
{
	Load<CSound>(L"kohovillage", L"sound\\kohovillage.wav");
	Load<CSound>(L"SaveBell", L"sound\\SaveBell.wav");
	Load<CSound>(L"Water1", L"sound\\Water1.wav");
	Load<CSound>(L"Water2", L"sound\\Water2.wav");
	Load<CSound>(L"WarningSFX", L"sound\\WarningSFX.wav");
	Load<CSound>(L"Hit", L"sound\\Hit.wav");
	Load<CSound>(L"Title", L"sound\\Title.wav");
	Load<CSound>(L"ArrowFire", L"sound\\ArrowFire.wav");
	Load<CSound>(L"TitleSparkle", L"sound\\TitleSparkle.wav");

	Load<CSound>(L"DemonIntro", L"sound\\DemonIntro.wav");
	Load<CSound>(L"Demon", L"sound\\Demon.wav");
	Load<CSound>(L"DemonRoar", L"sound\\DemonRoar.wav");
	Load<CSound>(L"DemonRoar2", L"sound\\DemonRoar2.wav");

	Load<CSound>(L"LinethIntro", L"sound\\LinethIntro.wav");
	Load<CSound>(L"Lineth", L"sound\\Lineth.wav");
	Load<CSound>(L"LinBackflip", L"sound\\LinBackflip.wav");
	Load<CSound>(L"LinProjectile", L"sound\\LinProjectile.wav");
	Load<CSound>(L"LinTeleport", L"sound\\LinTeleport.wav");
	Load<CSound>(L"LinSlash", L"sound\\LinSlash.wav");
	Load<CSound>(L"LinLand", L"sound\\LinLand.wav");
	Load<CSound>(L"LinJump", L"sound\\LinJump.wav");

	Load<CSound>(L"Jump", L"sound\\Jump.wav");
	Load<CSound>(L"Sprint", L"sound\\Sprint.wav");
	Load<CSound>(L"Roll", L"sound\\Roll.wav");
	Load<CSound>(L"Leaf12", L"sound\\Leaf12.wav");
	Load<CSound>(L"Leaf3", L"sound\\Leaf3.wav");
	Load<CSound>(L"PerfectDodge", L"sound\\PerfectDodge.wav");
}