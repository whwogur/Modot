#include "pch.h"
#include "CRenderMgr.h"

#include "CCamera.h"

CRenderMgr::CRenderMgr()
{

}

CRenderMgr::~CRenderMgr()
{

}


void CRenderMgr::Init()
{

}

void CRenderMgr::Tick()
{
	for (size_t i = 0; i < m_vecCam.size(); ++i)
	{
		if (nullptr == m_vecCam[i])
			continue;

		m_vecCam[i]->Render();
	}
}

void CRenderMgr::RegisterCamera(CCamera* _Cam, int _CamPriority)
{
	// ī�޶� �켱������ ���� ������ ������ ���õǾ�� �Ѵ�.
	if (m_vecCam.size() <= _CamPriority + 1)
		m_vecCam.resize(_CamPriority + 1);

	// ī�޶� �켱������ �´� ��ġ�� �ִ´�
	m_vecCam[_CamPriority] = _Cam;
}