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
	// ==========
	//    Point
	// ==========
	{
		auto mesh = MakePoint();
		Ptr<CMesh> pMesh = new CMesh();
		pMesh->SetEngineAsset();
		pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
		pMesh->SetName(L"PointMesh");
		AddAsset(L"PointMesh", pMesh);
	}

	// ==========
	//    Rect
	// ==========
	{
		auto mesh = MakeRect();
		Ptr<CMesh> pMesh = new CMesh();
		pMesh->SetEngineAsset();
		pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
		pMesh->SetName(L"RectMesh");
		AddAsset(L"RectMesh", pMesh);
	}

	// ==========
	// Debug Rect
	// ==========
	{
		auto mesh = MakeRectDebug();
		Ptr<CMesh> pMesh = new CMesh();
		pMesh->SetEngineAsset();
		pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
		pMesh->SetName(L"RectMesh_Debug");
		AddAsset(L"RectMesh_Debug", pMesh);
	}

	// ==========
	// RectGrid
	// ==========
	{
		auto mesh = MakeRectGrid(100, 100);
		Ptr<CMesh> pMesh = new CMesh();
		pMesh->SetEngineAsset();
		pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
		pMesh->SetName(L"RectGridMesh");
		AddAsset(L"RectGridMesh", pMesh);
	}

	// ==========
	//   Circle
	// ==========
	{
		auto mesh = MakeCircle(0.5f, 40);
		Ptr<CMesh> pMesh = new CMesh();
		pMesh->SetEngineAsset();
		pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
		pMesh->SetName(L"CircleMesh");
		AddAsset(L"CircleMesh", pMesh);
	}

	// ============
	// Debug Circle
	// ============
	{
		auto mesh = MakeCircleDebug(0.5f, 40);
		Ptr<CMesh> pMesh = new CMesh();
		pMesh->SetEngineAsset();
		pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
		pMesh->SetName(L"CircleMesh_Debug");
		AddAsset(L"CircleMesh_Debug", pMesh);
	}

	// ==========
	//    Cube
	// ==========
	{
		auto mesh = MakeCube();
		Ptr<CMesh> pMesh = new CMesh();
		pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
		pMesh->SetName(L"CubeMesh");
		AddAsset(L"CubeMesh", pMesh);

	// ==========
	// Cube Debug
	// ==========
		mesh.indices.clear();
		mesh.indices.resize(16);
		mesh.indices = {
			0, 1, 2, 3, 4,
			7, 6, 5, 4, 3,
			0, 7, 6, 1, 2, 5
		};
		pMesh = new CMesh();
		pMesh->SetEngineAsset();
		pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
		pMesh->SetName(L"CubeMesh_Debug");
		AddAsset(L"CubeMesh_Debug", pMesh);
	}

	// ============
	//	  Sphere
	// ============
	{
		auto mesh = MakeSphere(1, 25, 25);
		Ptr<CMesh> pMesh = new CMesh();
		pMesh->SetEngineAsset();
		pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
		pMesh->SetName(L"SphereMesh");
		AddAsset(L"SphereMesh", pMesh);
	}

	// =========
	//   Cone
	// =========
	{
		tMeshData meshData = MakeCone(0.5f, 1.f);
		Ptr<CMesh> pMesh = new CMesh();
		pMesh->SetEngineAsset();
		pMesh->Create(meshData.vertices.data(), (UINT)meshData.vertices.size(), meshData.indices.data(), (UINT)meshData.indices.size());
		AddAsset(L"ConeMesh", pMesh);
	}
	
	// ===========
	//   Cylinder
	// ===========
	{
		auto mesh = MakeCylinder(0.5f, 0.5f, 0.5f, 100);
		Ptr<CMesh> pMesh = new CMesh();
		pMesh->SetEngineAsset();
		pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
		pMesh->SetName(L"CylinderMesh");
		AddAsset(L"CylinderMesh", pMesh);
	}

	// ===========
	//   Capsule
	// ===========
	{
		auto mesh = MakeCapsule(0.5f, 0.5f, 30);
		Ptr<CMesh> pMesh = new CMesh();
		pMesh->SetEngineAsset();
		pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
		pMesh->SetName(L"CapsuleMesh");
		AddAsset(L"CapsuleMesh", pMesh);
	}
}

void CAssetMgr::CreateEngineTexture()
{
	MD_PROFILE_FUNCTION();

	// PostProcess 용도 텍스쳐
	Vec2 Resolution = CDevice::GetInst()->GetResolution();
	CreateTexture(L"PostProcessTex", (UINT)Resolution.x, (UINT)Resolution.y, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);
	CreateTexture(L"RenderTargetCopy", (UINT)Resolution.x, (UINT)Resolution.y, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT);
	CreateTexture(L"EffectTargetTex", (UINT)(Resolution.x), (UINT)(Resolution.y), DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
	CreateTexture(L"EffectDepthStencilTex", (UINT)(Resolution.x), (UINT)(Resolution.y), DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL);
	CreateTexture(L"EffectBlurTargetTex", (UINT)(Resolution.x), (UINT)(Resolution.y), DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

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
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
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

	// DebugLineShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\debug.fx", "VS_DebugLine");
	pShader->CreateGeometryShader(L"shader\\debug.fx", "GS_DebugLine");
	pShader->CreatePixelShader(L"shader\\debug.fx", "PS_DebugLine");
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEBUG);
	AddAsset(L"DebugLineShader", pShader);

	// EffectShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Effect");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Effect");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_EFFECT);

	AddAsset(L"EffectShader", pShader);

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

	// Std3DShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\std3d.fx", "VS_Std3D");
	pShader->CreatePixelShader(L"shader\\std3d.fx", "PS_Std3D");
	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);
	pShader->AddTexParam(TEX_0, "Albedo");
	pShader->AddTexParam(TEX_1, "Normal");
	AddAsset(L"Std3DShader", pShader);

	// Std3D_DeferredShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\std3d_deferred.fx", "VS_Std3D_Deferred");
	pShader->CreatePixelShader(L"shader\\std3d_deferred.fx", "PS_Std3D_Deferred");
	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEFERRED);
	pShader->AddTexParam(TEX_PARAM::TEX_0, "Albedo");
	pShader->AddTexParam(TEX_PARAM::TEX_1, "Normal");
	pShader->AddTexParam(TEX_PARAM::TEXCUBE_0, "Reflection");
	AddAsset(L"Std3D_DeferredShader", pShader);

	// SkyBoxShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\skybox.fx", "VS_SkyBox");
	pShader->CreatePixelShader(L"shader\\skybox.fx", "PS_SkyBox");
	pShader->SetRSType(RS_TYPE::CULL_FRONT);
	pShader->SetDSType(DS_TYPE::LESS_EQUAL);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);
	pShader->AddTexParam(TEX_0, "Albedo Texture");
	AddAsset(L"SkyBoxShader", pShader);

	// DecalShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\decal.fx", "VS_Decal");
	pShader->CreatePixelShader(L"shader\\decal.fx", "PS_Decal");
	pShader->SetRSType(RS_TYPE::CULL_FRONT);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::DECAL);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DECAL);
	pShader->AddTexParam(TEX_PARAM::TEX_1, "Decal Texture");
	pShader->AddTexParam(TEX_PARAM::TEX_2, "Emissive Texture");
	pShader->AddScalarParam(SCALAR_PARAM::FLOAT_0, "Emission Mul");
	AddAsset(L"DecalShader", pShader);

	// DirLightShadowMap Shader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\shadowmap.fx", "VS_DirLightShadowMap");
	pShader->CreatePixelShader(L"shader\\shadowmap.fx", "PS_DirLightShadowMap");
	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_SHADOWMAP);
	AddAsset(L"DirLightShadowMap", pShader);
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

	// DebugShapeMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"DebugShapeShader"));
	AddAsset(L"DebugShapeMtrl", pMtrl);

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

	// BlurMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"BlurShader"));
	AddAsset(L"BlurMtrl", pMtrl);

	// EffectMergeMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"EffectMergeShader"));
	AddAsset(L"EffectMergeMtrl", pMtrl);

	// Std3DMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std3DShader"));
	AddAsset(L"Std3DMtrl", pMtrl);

	// SkyBoxMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"SkyBoxShader"));
	AddAsset(L"SkyBoxMtrl", pMtrl);

	// Std3D_DeferredMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std3D_DeferredShader"));
	pMtrl->SetTexParam(TEX_PARAM::TEXCUBE_0, FindAsset<CTexture>(L"SkyWater"));
	AddAsset(L"DeferredMtrl", pMtrl);

	// DecalMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"DecalShader"));
	AddAsset(L"DecalMtrl", pMtrl);

	// DirLightShadowMapMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"DirLightShadowMap"));
	AddAsset(L"DirLightShadowMapMtrl", pMtrl);
}

void CAssetMgr::LoadSound()
{
	
}

tMeshData CAssetMgr::MakeCone(const float _Radius, const float _Height)
{
	tMeshData meshData;
	Vtx v;

	// Top
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vUV = Vec2(0.5f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vNormal = Vec3(0.f, 0.f, -1.f);
	v.vTangent = Vec3(1.f, 0.f, 0.f);
	meshData.vertices.emplace_back(v);

	// Body
	UINT iSliceCount = 40;

	float fSliceAngle = XM_2PI / iSliceCount;

	float fUVXStep = 1.f / (float)iSliceCount;
	float fUVYStep = 1.f;

	for (UINT i = 0; i <= iSliceCount; ++i)
	{
		float theta = i * fSliceAngle;

		v.vPos = Vec3(_Radius * cosf(theta), _Radius * sinf(theta), _Height);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		v.vUV = Vec2(fUVXStep * i, fUVYStep);

		v.vNormal = Vec3(0.f, 0.f, 1.f);
		v.vTangent = Vec3(1.f, 0.f, 0.f);
		v.vNormal.Cross(v.vTangent, v.vBinormal);
		v.vBinormal.Normalize();
		meshData.vertices.emplace_back(v);

		// 인덱스
		if (i < iSliceCount)
		{
			meshData.indices.emplace_back(0);
			meshData.indices.emplace_back(i + 2);
			meshData.indices.emplace_back(i + 1);
		}
	}

	tMeshData circle2D = MakeCircle(_Radius, iSliceCount);
	{
		int offset = int(meshData.vertices.size());

		for (auto& vtx : circle2D.vertices)
		{
			vtx.vPos = Vector3::Transform(vtx.vPos, Matrix::CreateRotationX(XM_PI));
			vtx.vPos.z += _Height;
			meshData.vertices.emplace_back(vtx);
		}

		for (const auto& index : circle2D.indices)
		{
			meshData.indices.emplace_back(index + offset);
		}
	}

	return meshData;
}

tMeshData CAssetMgr::MakeCircle(const float _Radius, const int _SliceCnt)
{
	tMeshData meshData;
	Vtx v;

	// 중심 점
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vUV = Vec2(0.5f, 0.5f);
	meshData.vertices.emplace_back(v);

	float fTheta = 0.f;

	for (int i = 0; i <= _SliceCnt; ++i)
	{
		fTheta = (XM_2PI / _SliceCnt) * i;

		v.vPos = Vec3(_Radius * cosf(fTheta), _Radius * sinf(fTheta), 0.f);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		v.vUV = Vec2(cosf(fTheta), sinf(fTheta));

		v.vNormal = Vec3(0.f, 0.f, -1.f);
		v.vTangent = Vec3(1.f, 0.f, 0.f);
		v.vNormal.Cross(v.vTangent, v.vBinormal);
		v.vBinormal.Normalize();

		meshData.vertices.emplace_back(v);
	}

	for (int i = 0; i < _SliceCnt; ++i)
	{
		meshData.indices.emplace_back(0);
		meshData.indices.emplace_back(i + 2);
		meshData.indices.emplace_back(i + 1);
	}

	return meshData;
}

tMeshData CAssetMgr::MakePoint()
{
	tMeshData meshData;
	Vtx v;
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vUV = Vec2(0.5f, 0.5f);

	v.vNormal = Vec3(0.f, 0.f, -1.f);
	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vNormal.Cross(v.vTangent, v.vBinormal);
	v.vBinormal.Normalize();

	meshData.vertices.emplace_back(v);
	meshData.indices.emplace_back(0);

	return meshData;
}

tMeshData CAssetMgr::MakeRect()
{
	std::vector<Vec3> positions;
	std::vector<Vec3> colors;
	std::vector<Vec3> normals;
	std::vector<Vec2> texcoords;
	std::vector<Vec3> tangents;

	// 앞면
	positions.emplace_back(Vec3(-0.5f, 0.5f, 0.0f));
	positions.emplace_back(Vec3(0.5f, 0.5f, 0.0f));
	positions.emplace_back(Vec3(0.5f, -0.5f, 0.0f));
	positions.emplace_back(Vec3(-0.5f, -0.5f, 0.0f));
	colors.emplace_back(Vec3(1.0f, 1.0f, 1.0f));
	colors.emplace_back(Vec3(1.0f, 1.0f, 1.0f));
	colors.emplace_back(Vec3(1.0f, 1.0f, 1.0f));
	colors.emplace_back(Vec3(1.0f, 1.0f, 1.0f));
	normals.emplace_back(Vec3(0.0f, 0.0f, -1.0f));
	normals.emplace_back(Vec3(0.0f, 0.0f, -1.0f));
	normals.emplace_back(Vec3(0.0f, 0.0f, -1.0f));
	normals.emplace_back(Vec3(0.0f, 0.0f, -1.0f));

	texcoords.emplace_back(Vec2(0.0f, 0.0f));
	texcoords.emplace_back(Vec2(1.0f, 0.0f));
	texcoords.emplace_back(Vec2(1.0f, 1.0f));
	texcoords.emplace_back(Vec2(0.0f, 1.0f));
	tangents.emplace_back(Vec3(1.0f, 0.0f, 0.0f));
	tangents.emplace_back(Vec3(1.0f, 0.0f, 0.0f));
	tangents.emplace_back(Vec3(1.0f, 0.0f, 0.0f));
	tangents.emplace_back(Vec3(1.0f, 0.0f, 0.0f));

	tMeshData meshData;

	for (size_t i = 0; i < positions.size(); i++)
	{
		Vtx v;
		v.vPos = positions[i];
		v.vColor = colors[i];
		v.vColor.w = 1.f;
		v.vUV = texcoords[i];

		v.vNormal = normals[i];
		v.vTangent = tangents[i];
		v.vNormal.Cross(v.vTangent, v.vBinormal);
		v.vBinormal.Normalize();

		meshData.vertices.emplace_back(v);
	}
	meshData.indices = {
		0, 2, 3, 2, 0, 1,
	};

	return meshData;
}

tMeshData CAssetMgr::MakeRectDebug()
{
	std::vector<Vec3> positions;
	std::vector<Vec3> colors;
	std::vector<Vec3> normals;
	std::vector<Vec2> texcoords;
	std::vector<Vec3> tangents;

	// 앞면
	positions.emplace_back(Vec3(-0.5f, 0.5f, 0.0f));
	positions.emplace_back(Vec3(0.5f, 0.5f, 0.0f));
	positions.emplace_back(Vec3(0.5f, -0.5f, 0.0f));
	positions.emplace_back(Vec3(-0.5f, -0.5f, 0.0f));
	colors.emplace_back(Vec3(1.0f, 1.0f, 1.0f));
	colors.emplace_back(Vec3(1.0f, 1.0f, 1.0f));
	colors.emplace_back(Vec3(1.0f, 1.0f, 1.0f));
	colors.emplace_back(Vec3(1.0f, 1.0f, 1.0f));
	normals.emplace_back(Vec3(0.0f, 0.0f, -1.0f));
	normals.emplace_back(Vec3(0.0f, 0.0f, -1.0f));
	normals.emplace_back(Vec3(0.0f, 0.0f, -1.0f));
	normals.emplace_back(Vec3(0.0f, 0.0f, -1.0f));

	texcoords.emplace_back(Vec2(0.0f, 0.0f));
	texcoords.emplace_back(Vec2(1.0f, 0.0f));
	texcoords.emplace_back(Vec2(1.0f, 1.0f));
	texcoords.emplace_back(Vec2(0.0f, 1.0f));
	tangents.emplace_back(Vec3(1.0f, 0.0f, 0.0f));
	tangents.emplace_back(Vec3(1.0f, 0.0f, 0.0f));
	tangents.emplace_back(Vec3(1.0f, 0.0f, 0.0f));
	tangents.emplace_back(Vec3(1.0f, 0.0f, 0.0f));

	tMeshData meshData;

	for (size_t i = 0; i < positions.size(); i++)
	{
		Vtx v;
		v.vPos = positions[i];
		v.vColor = colors[i];
		v.vUV = texcoords[i];
		v.vColor.w = 1.f;

		v.vNormal = normals[i];
		v.vTangent = tangents[i];
		v.vNormal.Cross(v.vTangent, v.vBinormal);
		v.vBinormal.Normalize();

		meshData.vertices.emplace_back(v);
	}

	meshData.indices = { 0, 1, 2, 3, 0 };

	return meshData;
}

tMeshData CAssetMgr::MakeCircleDebug(const float radius, const int numSlices)
{
	tMeshData meshData;
	Vtx v;

	// 중심
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vUV = Vec2(0.5f, 0.5f);
	meshData.vertices.emplace_back(v);

	float fTheta = 0.f;

	for (int i = 0; i <= numSlices; ++i)
	{
		fTheta = (XM_2PI / numSlices) * i;

		v.vPos = Vec3(radius * cosf(fTheta), radius * sinf(fTheta), 0.f);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		v.vUV = Vec2(cosf(fTheta), sinf(fTheta));

		v.vNormal = Vec3(0.f, 0.f, -1.f);
		v.vTangent = Vec3(1.f, 0.f, 0.f);
		v.vNormal.Cross(v.vTangent, v.vBinormal);
		v.vBinormal.Normalize();

		meshData.vertices.emplace_back(v);
	}

	for (int i = 1; i < meshData.vertices.size(); ++i)
	{
		meshData.indices.emplace_back(i);
	}

	return meshData;
}

tMeshData CAssetMgr::MakeRectGrid(const int _SliceCnt, const int _StackCnt)
{
	tMeshData meshData;

	float dx = 2.0f / _SliceCnt;
	float dy = 2.0f / _StackCnt;

	float y = 1.0f;
	for (int j = 0; j < _StackCnt + 1; j++)
	{
		float x = -1.0f;
		for (int i = 0; i < _SliceCnt + 1; i++)
		{
			Vtx v;
			v.vPos = Vec3(x, y, 0.0f);
			v.vUV = Vec2(x + 1.0f, y + 1.0f) * 0.5f;
			v.vNormal = Vec3(0.0f, 0.0f, -1.0f);
			v.vTangent = Vec3(1.0f, 0.0f, 0.0f);
			v.vNormal.Cross(v.vTangent, v.vBinormal);
			v.vBinormal.Normalize();

			meshData.vertices.push_back(v);

			x += dx;
		}
		y -= dy;
	}

	for (int j = 0; j < _StackCnt; j++)
	{
		for (int i = 0; i < _SliceCnt; i++)
		{
			meshData.indices.push_back((_SliceCnt + 1) * j + i);
			meshData.indices.push_back((_SliceCnt + 1) * j + i + 1);
			meshData.indices.push_back((_SliceCnt + 1) * (j + 1) + i);
			meshData.indices.push_back((_SliceCnt + 1) * (j + 1) + i);
			meshData.indices.push_back((_SliceCnt + 1) * j + i + 1);
			meshData.indices.push_back((_SliceCnt + 1) * (j + 1) + i + 1);
		}
	}

	return meshData;
}

tMeshData CAssetMgr::MakeSphere(const float _Radius, const int _SliceCnt, const int _StackCnt)
{
	const float fStackAngle = XM_PI / _StackCnt;
	const float fSliceAngle = XM_2PI / _SliceCnt;

	const float fUVXStep = 1.f / (float)_SliceCnt;
	const float fUVYStep = 1.f / (float)_StackCnt;

	tMeshData meshData;
	Vtx v;

	// Top
	v.vPos = Vec3(0.f, _Radius, 0.f);
	v.vUV = Vec2(0.5f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vNormal = v.vPos;
	v.vNormal.Normalize();
	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 0.f, -1.f);
	meshData.vertices.emplace_back(v);

	for (UINT i = 1; i < _StackCnt; ++i)
	{
		float phi = i * fStackAngle;

		for (UINT j = 0; j <= _SliceCnt; ++j)
		{
			float theta = j * fSliceAngle;

			v.vPos = Vec3(_Radius * sinf(i * fStackAngle) * cosf(j * fSliceAngle)
				, _Radius * cosf(i * fStackAngle)
				, _Radius * sinf(i * fStackAngle) * sinf(j * fSliceAngle));

			v.vUV = Vec2(fUVXStep * j, fUVYStep * i);
			v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
			v.vNormal = v.vPos;
			v.vNormal.Normalize();

			v.vTangent.x = -_Radius * sinf(phi) * sinf(theta);
			v.vTangent.y = 0.f;
			v.vTangent.z = _Radius * sinf(phi) * cosf(theta);
			v.vTangent.Normalize();

			v.vNormal.Cross(v.vTangent, v.vBinormal);
			v.vBinormal.Normalize();

			meshData.vertices.emplace_back(v);
		}
	}

	// Bottom
	v.vPos = Vec3(0.f, -_Radius, 0.f);
	v.vUV = Vec2(0.5f, 1.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vNormal = v.vPos;
	v.vNormal.Normalize();

	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 0.f, -1.f);
	meshData.vertices.emplace_back(v);

	// 인덱스
	// 북극점
	for (UINT i = 0; i < _SliceCnt; ++i)
	{
		meshData.indices.push_back(0);
		meshData.indices.push_back(i + 2);
		meshData.indices.push_back(i + 1);
	}

	// 몸통
	for (UINT i = 0; i < _StackCnt - 2; ++i)
	{
		for (UINT j = 0; j < _SliceCnt; ++j)
		{
			// + 
			// | \
			// +--+
			meshData.indices.push_back((_SliceCnt + 1) * (i)+(j)+1);
			meshData.indices.push_back((_SliceCnt + 1) * (i + 1) + (j + 1) + 1);
			meshData.indices.push_back((_SliceCnt + 1) * (i + 1) + (j)+1);

			// +--+
			//  \ |
			//    +
			meshData.indices.push_back((_SliceCnt + 1) * (i)+(j)+1);
			meshData.indices.push_back((_SliceCnt + 1) * (i)+(j + 1) + 1);
			meshData.indices.push_back((_SliceCnt + 1) * (i + 1) + (j + 1) + 1);
		}
	}

	// 남극점
	UINT iBottomIdx = (UINT)meshData.vertices.size() - 1;
	for (UINT i = 0; i < _SliceCnt; ++i)
	{
		meshData.indices.push_back(iBottomIdx);
		meshData.indices.push_back(iBottomIdx - (i + 2));
		meshData.indices.push_back(iBottomIdx - (i + 1));
	}
	return meshData;
}

tMeshData CAssetMgr::MakeCylinder(const float _BotRadius, const float _TopRadius, float _Height, int _SliceCnt)
{
	const float dTheta = -XM_2PI / float(_SliceCnt);

	tMeshData meshData;

	for (int i = 0; i <= _SliceCnt; i++)
	{
		Vtx v;
		v.vPos = Vec3::Transform(Vec3(_BotRadius, -0.5f * _Height, 0.0f), Matrix::CreateRotationY(dTheta * float(i)));
		v.vUV = Vec2(float(i) / _SliceCnt, 1.0f);

		v.vNormal = v.vPos - Vec3(0.0f, -0.5f * _Height, 0.0f);
		v.vNormal.Normalize();

		meshData.vertices.emplace_back(v);
	}

	for (int i = 0; i <= _SliceCnt; i++)
	{
		Vtx v;
		v.vPos = Vec3::Transform(Vec3(_TopRadius, 0.5f * _Height, 0.0f), Matrix::CreateRotationY(dTheta * float(i)));
		v.vUV = Vec2(float(i) / _SliceCnt, 0.0f);

		v.vNormal = v.vPos - Vec3(0.0f, 0.5f * _Height, 0.0f);
		v.vNormal.Normalize();

		meshData.vertices.emplace_back(v);
	}

	for (int i = 0; i < _SliceCnt; i++)
	{
		meshData.indices.push_back(i);
		meshData.indices.push_back(i + _SliceCnt + 1);
		meshData.indices.push_back(i + 1 + _SliceCnt + 1);

		meshData.indices.push_back(i);
		meshData.indices.push_back(i + 1 + _SliceCnt + 1);
		meshData.indices.push_back(i + 1);
	}

	return meshData;
}

tMeshData CAssetMgr::MakeCapsule(const float _Radius, const float _WaistHeight, const int _SliceCnt)
{
	tMeshData topSphere = MakeSphere(_Radius, _SliceCnt, _SliceCnt / 2);
	tMeshData bottomSphere = MakeSphere(_Radius, _SliceCnt, _SliceCnt / 2);

	// 상단과 하단을 y축 방향으로 이동시켜서 캡슐의 상단과 하단에 배치
	for (auto& vertex : topSphere.vertices)
		vertex.vPos.y += _WaistHeight;

	for (auto& vertex : bottomSphere.vertices)
		vertex.vPos.y -= _WaistHeight;

	// Cylinder
	tMeshData cylinder = MakeCylinder(_Radius, _Radius, 2.0f * _WaistHeight, _SliceCnt);

	// 상단 + 하단 + 원기둥
	tMeshData capsule;
	capsule.vertices.insert(capsule.vertices.end(), topSphere.vertices.begin(), topSphere.vertices.end());
	capsule.vertices.insert(capsule.vertices.end(), bottomSphere.vertices.begin(), bottomSphere.vertices.end());
	capsule.vertices.insert(capsule.vertices.end(), cylinder.vertices.begin(), cylinder.vertices.end());

	int offsetTop = int(topSphere.vertices.size());
	int offsetCylinder = int(offsetTop + bottomSphere.vertices.size());
	capsule.indices = topSphere.indices;
	for (auto& index : bottomSphere.indices)
		capsule.indices.push_back(index + offsetTop);
	for (auto& index : cylinder.indices)
		capsule.indices.push_back(index + offsetCylinder);

	return capsule;
}

tMeshData CAssetMgr::MakeCube()
{
	std::vector<Vec3> positions;
	std::vector<Vec3> colors;
	std::vector<Vec3> normals;
	std::vector<Vec2> texcoords;
	std::vector<Vec3> tangents;

	// 윗면
	positions.emplace_back(Vec3(-0.5f, 0.5f, 0.5f));
	positions.emplace_back(Vec3(0.5f, 0.5f, 0.5f));
	positions.emplace_back(Vec3(0.5f, 0.5f, -0.5f));
	positions.emplace_back(Vec3(-0.5f, 0.5f, -0.5f));
	colors.emplace_back(Vec3(0.0f, 1.0f, 0.0f));
	colors.emplace_back(Vec3(0.0f, 1.0f, 0.0f));
	colors.emplace_back(Vec3(0.0f, 1.0f, 0.0f));
	colors.emplace_back(Vec3(0.0f, 1.0f, 0.0f));
	normals.emplace_back(Vec3(0.0f, 1.0f, 0.0f));
	normals.emplace_back(Vec3(0.0f, 1.0f, 0.0f));
	normals.emplace_back(Vec3(0.0f, 1.0f, 0.0f));
	normals.emplace_back(Vec3(0.0f, 1.0f, 0.0f));
	texcoords.emplace_back(Vec2(0.0f, 0.0f));
	texcoords.emplace_back(Vec2(1.0f, 0.0f));
	texcoords.emplace_back(Vec2(1.0f, 1.0f));
	texcoords.emplace_back(Vec2(0.0f, 1.0f));
	tangents.emplace_back(Vec3(1.0f, 0.0f, 0.0f));
	tangents.emplace_back(Vec3(1.f, 0.0f, 0.0f));
	tangents.emplace_back(Vec3(1.0f, 0.0f, 0.0f));
	tangents.emplace_back(Vec3(1.0f, 0.0f, 0.0f));

	// 아랫면
	positions.emplace_back(Vec3(-0.5f, -0.5f, -0.5f));
	positions.emplace_back(Vec3(0.5f, -0.5f, -0.5f));
	positions.emplace_back(Vec3(0.5f, -0.5f, 0.5f));
	positions.emplace_back(Vec3(-0.5f, -0.5f, 0.5f));
	colors.emplace_back(Vec3(1.0f, 0.0f, 0.0f));
	colors.emplace_back(Vec3(1.0f, 0.0f, 0.0f));
	colors.emplace_back(Vec3(1.0f, 0.0f, 0.0f));
	colors.emplace_back(Vec3(1.0f, 0.0f, 0.0f));
	normals.emplace_back(Vec3(0.0f, -1.0f, 0.0f));
	normals.emplace_back(Vec3(0.0f, -1.0f, 0.0f));
	normals.emplace_back(Vec3(0.0f, -1.0f, 0.0f));
	normals.emplace_back(Vec3(0.0f, -1.0f, 0.0f));
	texcoords.emplace_back(Vec2(0.0f, 0.0f));
	texcoords.emplace_back(Vec2(1.0f, 0.0f));
	texcoords.emplace_back(Vec2(1.0f, 1.0f));
	texcoords.emplace_back(Vec2(0.0f, 1.0f));
	tangents.emplace_back(Vec3(1.0f, 0.0f, 0.0f));
	tangents.emplace_back(Vec3(1.0f, 0.0f, 0.0f));
	tangents.emplace_back(Vec3(1.0f, 0.0f, 0.0f));
	tangents.emplace_back(Vec3(1.0f, 0.0f, 0.0f));

	// 앞면
	positions.emplace_back(Vec3(-0.5f, 0.5f, -0.5f));
	positions.emplace_back(Vec3(0.5f, 0.5f, -0.5f));
	positions.emplace_back(Vec3(0.5f, -0.5f, -0.5f));
	positions.emplace_back(Vec3(-0.5f, -0.5f, -0.5f));
	colors.emplace_back(Vec3(1.0f, 0.0f, 1.0f));
	colors.emplace_back(Vec3(1.0f, 0.0f, 1.0f));
	colors.emplace_back(Vec3(1.0f, 0.0f, 1.0f));
	colors.emplace_back(Vec3(1.0f, 0.0f, 1.0f));
	normals.emplace_back(Vec3(0.0f, 0.0f, -1.0f));
	normals.emplace_back(Vec3(0.0f, 0.0f, -1.0f));
	normals.emplace_back(Vec3(0.0f, 0.0f, -1.0f));
	normals.emplace_back(Vec3(0.0f, 0.0f, -1.0f));
	texcoords.emplace_back(Vec2(0.0f, 0.0f));
	texcoords.emplace_back(Vec2(1.0f, 0.0f));
	texcoords.emplace_back(Vec2(1.0f, 1.0f));
	texcoords.emplace_back(Vec2(1.0f, 1.0f));
	tangents.emplace_back(Vec3(1.0f, 0.0f, 0.0f));
	tangents.emplace_back(Vec3(1.0f, 0.0f, 0.0f));
	tangents.emplace_back(Vec3(1.0f, 0.0f, 0.0f));
	tangents.emplace_back(Vec3(1.0f, 0.0f, 0.0f));

	// 뒷면
	positions.emplace_back(Vec3(0.5f, 0.5f, 0.5f));
	positions.emplace_back(Vec3(-0.5f, 0.5f, 0.5f));
	positions.emplace_back(Vec3(-0.5f, -0.5f, 0.5f));
	positions.emplace_back(Vec3(0.5f, -0.5f, 0.5f));
	colors.emplace_back(Vec3(1.0f, 0.0f, 0.0f));
	colors.emplace_back(Vec3(1.0f, 0.0f, 0.0f));
	colors.emplace_back(Vec3(1.0f, 0.0f, 0.0f));
	colors.emplace_back(Vec3(1.0f, 0.0f, 0.0f));
	normals.emplace_back(Vec3(0.0f, 0.0f, 1.0f));
	normals.emplace_back(Vec3(0.0f, 0.0f, 1.0f));
	normals.emplace_back(Vec3(0.0f, 0.0f, 1.0f));
	normals.emplace_back(Vec3(0.0f, 0.0f, 1.0f));
	texcoords.emplace_back(Vec2(0.0f, 0.0f));
	texcoords.emplace_back(Vec2(1.0f, 0.0f));
	texcoords.emplace_back(Vec2(1.0f, 1.0f));
	texcoords.emplace_back(Vec2(0.0f, 1.0f));
	tangents.emplace_back(Vec3(-1.0f, 0.0f, 0.0f));
	tangents.emplace_back(Vec3(-1.0f, 0.0f, 0.0f));
	tangents.emplace_back(Vec3(-1.0f, 0.0f, 0.0f));
	tangents.emplace_back(Vec3(-1.0f, 0.0f, 0.0f));

	// 왼쪽
	positions.emplace_back(Vec3(-0.5f, 0.5f, 0.5f));
	positions.emplace_back(Vec3(-0.5f, 0.5f, -0.5f));
	positions.emplace_back(Vec3(-0.5f, -0.5f, -0.5f));
	positions.emplace_back(Vec3(-0.5f, -0.5f, 0.5f));
	colors.emplace_back(Vec3(0.0f, 1.0f, 0.0f));
	colors.emplace_back(Vec3(0.0f, 1.0f, 0.0f));
	colors.emplace_back(Vec3(0.0f, 1.0f, 0.0f));
	colors.emplace_back(Vec3(0.0f, 1.0f, 0.0f));
	normals.emplace_back(Vec3(-1.0f, 0.0f, 0.0f));
	normals.emplace_back(Vec3(-1.0f, 0.0f, 0.0f));
	normals.emplace_back(Vec3(-1.0f, 0.0f, 0.0f));
	normals.emplace_back(Vec3(-1.0f, 0.0f, 0.0f));
	texcoords.emplace_back(Vec2(0.0f, 0.0f));
	texcoords.emplace_back(Vec2(1.0f, 0.0f));
	texcoords.emplace_back(Vec2(1.0f, 1.0f));
	texcoords.emplace_back(Vec2(0.0f, 1.0f));
	tangents.emplace_back(Vec3(0.0f, 0.0f, -1.0f));
	tangents.emplace_back(Vec3(0.0f, 0.0f, -1.0f));
	tangents.emplace_back(Vec3(0.0f, 0.0f, -1.0f));
	tangents.emplace_back(Vec3(0.0f, 0.0f, -1.0f));

	// 오른쪽
	positions.emplace_back(Vec3(0.5f, 0.5f, -0.5f));
	positions.emplace_back(Vec3(0.5f, 0.5f, 0.5f));
	positions.emplace_back(Vec3(0.5f, -0.5f, 0.5f));
	positions.emplace_back(Vec3(0.5f, -0.5f, -0.5f));
	colors.emplace_back(Vec3(0.0f, 0.0f, 1.0f));
	colors.emplace_back(Vec3(0.0f, 0.0f, 1.0f));
	colors.emplace_back(Vec3(0.0f, 0.0f, 1.0f));
	colors.emplace_back(Vec3(0.0f, 0.0f, 1.0f));
	normals.emplace_back(Vec3(1.0f, 0.0f, 0.0f));
	normals.emplace_back(Vec3(1.0f, 0.0f, 0.0f));
	normals.emplace_back(Vec3(1.0f, 0.0f, 0.0f));
	normals.emplace_back(Vec3(1.0f, 0.0f, 0.0f));
	texcoords.emplace_back(Vec2(0.0f, 0.0f));
	texcoords.emplace_back(Vec2(1.0f, 0.0f));
	texcoords.emplace_back(Vec2(1.0f, 1.0f));
	texcoords.emplace_back(Vec2(0.0f, 1.0f));
	tangents.emplace_back(Vec3(0.0f, 0.0f, 1.0f));
	tangents.emplace_back(Vec3(0.0f, 0.0f, 1.0f));
	tangents.emplace_back(Vec3(0.0f, 0.0f, 1.0f));
	tangents.emplace_back(Vec3(0.0f, 0.0f, 1.0f));

	tMeshData meshData;
	for (size_t i = 0; i < positions.size(); i++)
	{
		Vtx v;
		v.vPos = positions[i];
		v.vColor = colors[i];
		v.vColor.w = 1.f;
		v.vUV = texcoords[i];

		v.vNormal = normals[i];
		v.vTangent = tangents[i];
		v.vNormal.Cross(v.vTangent, v.vBinormal);
		v.vBinormal.Normalize();

		meshData.vertices.push_back(v);
	}

	// 인덱스
	for (int i = 0; i < 12; i += 2)
	{
		meshData.indices.push_back(i * 2);
		meshData.indices.push_back(i * 2 + 1);
		meshData.indices.push_back(i * 2 + 2);

		meshData.indices.push_back(i * 2);
		meshData.indices.push_back(i * 2 + 2);
		meshData.indices.push_back(i * 2 + 3);
	}

	return meshData;
}
