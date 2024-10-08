#include "spch.h"
#include "CScriptMgr.h"
#include <Engine/CScript.h>

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	}
	return nullptr;
}