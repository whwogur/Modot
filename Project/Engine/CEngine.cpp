#include "pch.h"
#include "CEngine.h"
#include "Log.h"

#include "CDevice.h"

#include "CPathMgr.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CAssetMgr.h"
#include "CLevelMgr.h"
#include "CRenderMgr.h"
#include "CCollisionMgr.h"
#include "CTaskMgr.h"

CEngine::CEngine()
	: m_hWnd(nullptr)
	, m_ptResolution{}
{
}

CEngine::~CEngine()
{
}

int CEngine::Init(HWND _wnd, POINT _ptResolution)
{
	Modot::Log::Init();
	m_hWnd = _wnd;
	m_ptResolution = _ptResolution;
	ChangeWindowScale(_ptResolution.x, _ptResolution.y);

	MD_ENGINE_ASSERT(SUCCEEDED(CDevice::GetInst()->Init(m_hWnd, m_ptResolution.x, m_ptResolution.y)), L"Device �ʱ�ȭ ����");

	CPathMgr::GetInst()->Init();// MD_ENGINE_TRACE(L"Path Manager �ʱ�ȭ �Ϸ�");
	CKeyMgr::GetInst()->Init();// MD_ENGINE_TRACE(L"Key Manager �ʱ�ȭ �Ϸ�");
	CTimeMgr::GetInst()->Init();// MD_ENGINE_TRACE(L"Time Manager �ʱ�ȭ �Ϸ�");
	CAssetMgr::GetInst()->Init();// MD_ENGINE_TRACE(L"Asset Manager �ʱ�ȭ �Ϸ�");
	CLevelMgr::GetInst()->Init();// MD_ENGINE_TRACE(L"Level Manager �ʱ�ȭ �Ϸ�");
	CRenderMgr::GetInst()->Init();// MD_ENGINE_TRACE(L"Render Manager �ʱ�ȭ �Ϸ�");

	return S_OK;
}


void CEngine::Run()
{
	// Manager
	CKeyMgr::GetInst()->Tick();
	CTimeMgr::GetInst()->Tick();
	CAssetMgr::GetInst()->Tick();
	CLevelMgr::GetInst()->Run();
	CCollisionMgr::GetInst()->Tick();
	// Render
	CRenderMgr::GetInst()->Tick();

	CTaskMgr::GetInst()->Tick();
}




void CEngine::ChangeWindowScale(UINT _Width, UINT _Height)
{
	bool bMenu = false;
	if (GetMenu(m_hWnd))
		bMenu = true;

	RECT rt = { 0, 0, (int)_Width, (int)_Height };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, bMenu);
	SetWindowPos(m_hWnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
}