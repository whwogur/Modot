#include "pch.h"
#include "CTimeMgr.h"

#include "CEngine.h"

CTimeMgr::CTimeMgr()
	: m_llCurCount{}
	, m_llPrevCount{}
	, m_llFrequency{}
	, m_FPS(0)
	, m_DeltaTime(0.f)
	, m_Time(0.f)
{

}

CTimeMgr::~CTimeMgr()
{

}

void CTimeMgr::Init()
{
	// �ʴ� 1000 �� ī�����ϴ� GetTickCount �Լ��� �̼��� �ð��� �����ϱ⿡�� ��Ȯ���� ��������.

	// 1�ʿ� �����ִ� ī��Ʈ ������ ��´�.
	QueryPerformanceFrequency(&m_llFrequency);

	QueryPerformanceCounter(&m_llCurCount);
	m_llPrevCount = m_llCurCount;	
}

void CTimeMgr::Tick()
{
	// ���� ī��Ʈ ���
	QueryPerformanceCounter(&m_llCurCount);

	// ���� ī��Ʈ�� ���� ī��Ʈ�� ���̰��� ���ؼ� 1������ ���� �ð����� ���
	m_DeltaTime = (float)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (float)m_llFrequency.QuadPart;

	// DT ����
	if (1.f / 60.f < m_DeltaTime)
		m_DeltaTime = 1.f / 60.f;

	// �����ð��� ���ؼ� ���α׷��� ����� ���ķ� ������ �ð����� ���
	m_Time += m_DeltaTime;

	// ���� ī��Ʈ ���� ���� ī��Ʈ�� �����ص�
	m_llPrevCount = m_llCurCount;

	// �ʴ� ���� Ƚ��(FPS) ���
	++m_FPS;

	// 1�ʿ� �ѹ��� TextOut ���
	static float AccTime = 0.f;
	AccTime += m_DeltaTime;
	
	if (1.f < AccTime)
	{
		wchar_t szBuff[255] = {};
		swprintf_s(szBuff, L"DeltaTime : %f, FPS : %d ", m_DeltaTime, m_FPS);
		//TextOut(CEngine::GetInst()->GetMainDC(), 10, 10, szBuff, wcslen(szBuff));
		SetWindowText(CEngine::GetInst()->GetMainWnd(), szBuff);
		AccTime = 0.f;
		m_FPS = 0;
	}
}