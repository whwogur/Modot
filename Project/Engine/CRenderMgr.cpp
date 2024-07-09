#include "pch.h"
#include "CRenderMgr.h"
#include "CDevice.h"
#include "CConstBuffer.h"
#include "CCamera.h"
#include "CTimeMgr.h"
#include "CAssetMgr.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLight2D.h"
#include "CStructuredBuffer.h"
#include "CKeyMgr.h"

CRenderMgr::CRenderMgr()
	: m_EditorCamera(nullptr)
	, m_ViewportTexSize{500, 500}
	, m_DebugObject(nullptr)
	, m_DebugShapeList{}
{
	m_Light2DBuffer = new CStructuredBuffer;
}

CRenderMgr::~CRenderMgr()
{
	if (nullptr != m_DebugObject)
		delete m_DebugObject;
	if (nullptr != m_Light2DBuffer)
		delete m_Light2DBuffer;
}


void CRenderMgr::CreateViewportTex(Vec2 _Size)
{
	D3D11_TEXTURE2D_DESC textureDesc = {};
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};

	ZeroMemory(&textureDesc, sizeof(textureDesc));

	textureDesc.Width = _Size.x;
	textureDesc.Height = _Size.y;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	
	MD_ENGINE_ASSERT(SUCCEEDED(DEVICE->CreateTexture2D(&textureDesc, NULL, m_ViewportTex.GetAddressOf())), L"ViewportTex 생성 실패");
	
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	MD_ENGINE_ASSERT(SUCCEEDED(DEVICE->CreateRenderTargetView(m_ViewportTex.Get(), &renderTargetViewDesc, m_ViewportRTV.GetAddressOf())), L"ViewportTex SRV 생성 실패");

	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	MD_ENGINE_ASSERT(SUCCEEDED(DEVICE->CreateShaderResourceView(m_ViewportTex.Get(), &shaderResourceViewDesc, m_ViewportSRV.GetAddressOf())), L"ViewportTex RTV 생성 실패");


	depthStencilDesc.Width = _Size.x;
	depthStencilDesc.Height = _Size.y;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	DEVICE->CreateTexture2D(&depthStencilDesc, nullptr, m_ViewportDSTex.GetAddressOf());

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = depthStencilDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	DEVICE->CreateDepthStencilView(m_ViewportDSTex.Get(), &depthStencilViewDesc, m_ViewportDSV.GetAddressOf());

	MD_ENGINE_TRACE(L"뷰포트 텍스처 새로 생성");
}

void CRenderMgr::ResizeViewportTex(Vec2 _Size)
{
	//MD_ENGINE_TRACE("({0} , {1})", _Size.x, _Size.y);
	//MD_ENGINE_TRACE("({0} , {1})", m_ViewportTexSize.x, m_ViewportTexSize.y);
	if (m_ViewportTex != nullptr && (_Size.x != (UINT)m_ViewportTexSize.x || _Size.y != (UINT)m_ViewportTexSize.y))
	{
		m_ViewportTexSize = _Size;
		m_ViewportTex->Release();

		if (m_ViewportSRV != nullptr)
			m_ViewportSRV->Release();
		if (m_ViewportRTV != nullptr)
			m_ViewportRTV->Release();
		CreateViewportTex(m_ViewportTexSize);
		m_EditorCamera->Camera()->SetHeight(m_ViewportTexSize.y);
		m_EditorCamera->Camera()->SetWidth(m_ViewportTexSize.x);
	}
}

void CRenderMgr::Init()
{
	m_DebugObject = new CGameObject;
	m_DebugObject->AddComponent(new CTransform);
	m_DebugObject->AddComponent(new CMeshRender);
	m_DebugObject->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DebugShapeMtrl"));

	CreateViewportTex(m_ViewportTexSize);
}

void CRenderMgr::Tick()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	if (nullptr == pCurLevel)
		return;

	RenderStart();

	// Level 이 Play 상태인 경우, Level 내에 있는 카메라 시점으로 렌더링
	if (PLAY == pCurLevel->GetState())
	{
		for (size_t i = 0; i < m_vecCam.size(); ++i)
		{
			if (nullptr == m_vecCam[i])
				continue;

			m_vecCam[i]->Render();
		}
	}

	// Level 이 Stop 이나 Pause 인 경우, Editor 용 카메라 시점으로 렌더링
	else
	{
		if (nullptr != m_EditorCamera)
		{
			m_EditorCamera->Render();
		}
	}

	RenderDebugShape();

	Clear();
}

void CRenderMgr::RegisterCamera(CCamera* _Cam, int _CamPriority)
{
	// 카메라 우선순위에 따라서 벡터의 공간의 마련되어야 한다.
	if (m_vecCam.size() <= _CamPriority + 1)
		m_vecCam.resize(_CamPriority + 1);

	// 카메라 우선순위에 맞는 위치에 넣는다
	m_vecCam[_CamPriority] = _Cam;
}

void CRenderMgr::RenderStart()
{
	// 렌더타겟 지정
	CONTEXT->OMSetRenderTargets(1, m_ViewportRTV.GetAddressOf(), m_ViewportDSV.Get());

	// TargetClear
	float color[4] = { 0.f, 0.f, 0.f, 1.f };
	CONTEXT->ClearRenderTargetView(m_ViewportRTV.Get(), color);
	CONTEXT->ClearDepthStencilView(m_ViewportDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	g_GlobalData.g_Resolution = m_ViewportTexSize;
	g_GlobalData.g_Light2DCount = (int)m_vecLight2D.size();

	// Light2D 정보 업데이트 및 바인딩
	vector<tLightInfo> vecLight2DInfo;
	for (size_t i = 0; i < m_vecLight2D.size(); ++i)
	{
		vecLight2DInfo.push_back(m_vecLight2D[i]->GetLightInfoCRef());
	}

	if (m_Light2DBuffer->GetElementCount() < vecLight2DInfo.size())
	{
		m_Light2DBuffer->Create(sizeof(tLightInfo), vecLight2DInfo.size());
	}

	m_Light2DBuffer->SetData(vecLight2DInfo.data());
	m_Light2DBuffer->Bind(11);

	// GlobalData 바인딩
	static CConstBuffer* pGlobalCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL);
	pGlobalCB->SetData(&g_GlobalData);
	pGlobalCB->Bind();
}

void CRenderMgr::Clear()
{
	m_vecLight2D.clear();
}

void CRenderMgr::RenderDebugShape()
{
	list<tDebugShapeInfo>::iterator iter = m_DebugShapeList.begin();

	for (; iter != m_DebugShapeList.end(); )
	{
		// 디버그 요청 모양에 맞는 메시를 가져옴
		switch ((*iter).Shape)
		{
		case DEBUG_SHAPE::RECT:
			m_DebugObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh_Debug"));
			break;
		case DEBUG_SHAPE::CIRCLE:
			m_DebugObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh_Debug"));
			break;
		case DEBUG_SHAPE::LINE:

			break;
		case DEBUG_SHAPE::CUBE:

			break;
		case DEBUG_SHAPE::SPHERE:

			break;
		}

		// 위치 세팅
		m_DebugObject->Transform()->SetWorldMatrix((*iter).matWorld);

		// 재질 세팅
		m_DebugObject->MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, (*iter).vColor);

		// 깊이판정 여부에 따라서, 쉐이더의 깊이판정 방식을 결정한다.
		if ((*iter).DepthTest)
			m_DebugObject->MeshRender()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::LESS);
		else
			m_DebugObject->MeshRender()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

		// 렌더링
		m_DebugObject->MeshRender()->Render();


		// 수명이 다한 디버그 정보를 삭제
		(*iter).Age += EngineDT;
		if ((*iter).LifeTime < (*iter).Age)
		{
			iter = m_DebugShapeList.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}