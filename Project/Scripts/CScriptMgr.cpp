#include "spch.h"
#include "CScriptMgr.h"

#include "CCameraControl.h"
#include "CTestObject.h"

void CScriptMgr::GetScriptInfo(std::vector<wstring>& _vec)
{
	_vec.push_back(L"CCameraControl");
	_vec.push_back(L"CTestObject");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CCameraControl" == _strScriptName)
		return new CCameraControl;
	if (L"CTestObject" == _strScriptName)
		return new CTestObject;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::CAMERACONTROL:
		return new CCameraControl;
		break;
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
	case SCRIPT_TYPE::CAMERACONTROL:
		return L"CCameraControl";
		break;

	case SCRIPT_TYPE::TESTOBJECT:
		return L"CTestObject";
		break;

	}
	return nullptr;
}