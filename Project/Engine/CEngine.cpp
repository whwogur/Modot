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
#include "CFontMgr.h"

#include "../Client/CPlayerManager.h" // ���� ��
CEngine::CEngine()
	: m_hWnd(nullptr)
	, m_ptResolution{}
{
}

CEngine::~CEngine()
{
	if (nullptr != m_FMODSystem)
	{
		m_FMODSystem->release();
		m_FMODSystem = nullptr;
	}
}

int CEngine::Init(HWND _wnd, POINT _ptResolution, OBJECT_SAVE _SaveFunc, OBJECT_LOAD _Func)
{
	MD_PROFILE_FUNCTION();
	Modot::Log::Init();

	// FMOD �ʱ�ȭ
	FMOD::System_Create(&m_FMODSystem);
	assert(m_FMODSystem);

	// 32�� ä�� ����
	m_FMODSystem->init(32, FMOD_DEFAULT, nullptr);

	m_hWnd = _wnd;
	m_ptResolution = _ptResolution;
	ChangeWindowScale(_ptResolution.x, _ptResolution.y);

	MD_ENGINE_ASSERT(SUCCEEDED(CDevice::GetInst()->Init(m_hWnd, m_ptResolution.x, m_ptResolution.y)), L"Device �ʱ�ȭ ����");

	CPathMgr::GetInst()->Init();
	CKeyMgr::GetInst()->Init();
	CTimeMgr::GetInst()->Init();
	CAssetMgr::GetInst()->Init();
	CRenderMgr::GetInst()->Init();
	CFontMgr::GetInst()->Init();
	// Prefab Function ���
	CPrefab::g_ObjectSaveFunc = _SaveFunc;
	CPrefab::g_ObjectLoadFunc = _Func;

	return S_OK;
}


void CEngine::Run()
{
	MD_PROFILE_FUNCTION();
	m_FMODSystem->update();
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