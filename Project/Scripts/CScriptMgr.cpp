#include "spch.h"
#include "CScriptMgr.h"

#include "CBackGroundScript.h"
#include "CCameraMoveScript.h"
#include "CNPCScript.h"
#include "CParticleTexScript.h"
#include "CPlayerScript.h"
#include "CSpriteRenderScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CBackGroundScript");
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CNPCScript");
	_vec.push_back(L"CParticleTexScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CSpriteRenderScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CBackGroundScript" == _strScriptName)
		return new CBackGroundScript;
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CNPCScript" == _strScriptName)
		return new CNPCScript;
	if (L"CParticleTexScript" == _strScriptName)
		return new CParticleTexScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CSpriteRenderScript" == _strScriptName)
		return new CSpriteRenderScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::BACKGROUNDSCRIPT:
		return new CBackGroundScript;
		break;
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::NPCSCRIPT:
		return new CNPCScript;
		break;
	case (UINT)SCRIPT_TYPE::PARTICLETEXSCRIPT:
		return new CParticleTexScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::SPRITERENDERSCRIPT:
		return new CSpriteRenderScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::BACKGROUNDSCRIPT:
		return L"CBackGroundScript";
		break;

	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CCameraMoveScript";
		break;

	case SCRIPT_TYPE::NPCSCRIPT:
		return L"CNPCScript";
		break;

	case SCRIPT_TYPE::PARTICLETEXSCRIPT:
		return L"CParticleTexScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::SPRITERENDERSCRIPT:
		return L"CSpriteRenderScript";
		break;

	}
	return nullptr;
}