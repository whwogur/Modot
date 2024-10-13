#include "spch.h"
#include "CScriptMgr.h"

#include "CTestObject.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CTestObject");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CTestObject" == _strScriptName)
		return new CTestObject;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::TESTOBJECT:
		return new CTestObject;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::TESTOBJECT:
		return L"CTestObject";
		break;

	}
	return nullptr;
}