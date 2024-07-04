#include "pch.h"
#include "CPathMgr.h"

#include "CEngine.h"

CPathMgr::CPathMgr()	
{
}

CPathMgr::~CPathMgr()
{
}

void CPathMgr::Init()
{
	// 실행경로
	wchar_t szBuffer[256] = {};
	GetCurrentDirectory(256, szBuffer);

	// bin 폴더의 상위폴더로 접근
	GetParentPath(szBuffer);

	m_Content = szBuffer;
	m_Content += L"\\content\\";	

	GetParentPath(szBuffer);
	m_Solution = szBuffer;
	m_Solution += L"\\";
}

void CPathMgr::GetParentPath(_Inout_ wchar_t* _Buffer)
{
	size_t len = wcslen(_Buffer);
	
	for (size_t i = len - 1; 0 < i; --i)
	{
		if (L'\\' == _Buffer[i])
		{
			_Buffer[i] = L'\0';
			break;
		}
	}
}
