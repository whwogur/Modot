#include "spch.h"
#include "CScriptMgr.h"

#include "CAttackScript.h"
#include "CBackGroundScript.h"
#include "CBellScript.h"
#include "CCameraMoveScript.h"
#include "CDemonScript.h"
#include "CNPCScript.h"
#include "CNPCUIScript.h"
#include "CParticleScript.h"
#include "CPlatformScript.h"
#include "CPlayerScript.h"
#include "CSpriteRenderScript.h"
#include "CTintedBGScript.h"
#include "CWaterScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CAttackScript");
	_vec.push_back(L"CBackGroundScript");
	_vec.push_back(L"CBellScript");
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CDemonScript");
	_vec.push_back(L"CNPCScript");
	_vec.push_back(L"CNPCUIScript");
	_vec.push_back(L"CParticleScript");
	_vec.push_back(L"CPlatformScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CSpriteRenderScript");
	_vec.push_back(L"CTintedBGScript");
	_vec.push_back(L"CWaterScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CAttackScript" == _strScriptName)
		return new CAttackScript;
	if (L"CBackGroundScript" == _strScriptName)
		return new CBackGroundScript;
	if (L"CBellScript" == _strScriptName)
		return new CBellScript;
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CDemonScript" == _strScriptName)
		return new CDemonScript;
	if (L"CNPCScript" == _strScriptName)
		return new CNPCScript;
	if (L"CNPCUIScript" == _strScriptName)
		return new CNPCUIScript;
	if (L"CParticleScript" == _strScriptName)
		return new CParticleScript;
	if (L"CPlatformScript" == _strScriptName)
		return new CPlatformScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CSpriteRenderScript" == _strScriptName)
		return new CSpriteRenderScript;
	if (L"CTintedBGScript" == _strScriptName)
		return new CTintedBGScript;
	if (L"CWaterScript" == _strScriptName)
		return new CWaterScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::ATTACKSCRIPT:
		return new CAttackScript;
		break;
	case (UINT)SCRIPT_TYPE::BACKGROUNDSCRIPT:
		return new CBackGroundScript;
		break;
	case (UINT)SCRIPT_TYPE::BELLSCRIPT:
		return new CBellScript;
		break;
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::DEMONSCRIPT:
		return new CDemonScript;
		break;
	case (UINT)SCRIPT_TYPE::NPCSCRIPT:
		return new CNPCScript;
		break;
	case (UINT)SCRIPT_TYPE::NPCUISCRIPT:
		return new CNPCUIScript;
		break;
	case (UINT)SCRIPT_TYPE::PARTICLESCRIPT:
		return new CParticleScript;
		break;
	case (UINT)SCRIPT_TYPE::PLATFORMSCRIPT:
		return new CPlatformScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::SPRITERENDERSCRIPT:
		return new CSpriteRenderScript;
		break;
	case (UINT)SCRIPT_TYPE::TINTEDBGSCRIPT:
		return new CTintedBGScript;
		break;
	case (UINT)SCRIPT_TYPE::WATERSCRIPT:
		return new CWaterScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::ATTACKSCRIPT:
		return L"CAttackScript";
		break;

	case SCRIPT_TYPE::BACKGROUNDSCRIPT:
		return L"CBackGroundScript";
		break;

	case SCRIPT_TYPE::BELLSCRIPT:
		return L"CBellScript";
		break;

	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CCameraMoveScript";
		break;

	case SCRIPT_TYPE::DEMONSCRIPT:
		return L"CDemonScript";
		break;

	case SCRIPT_TYPE::NPCSCRIPT:
		return L"CNPCScript";
		break;

	case SCRIPT_TYPE::NPCUISCRIPT:
		return L"CNPCUIScript";
		break;

	case SCRIPT_TYPE::PARTICLESCRIPT:
		return L"CParticleScript";
		break;

	case SCRIPT_TYPE::PLATFORMSCRIPT:
		return L"CPlatformScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::SPRITERENDERSCRIPT:
		return L"CSpriteRenderScript";
		break;

	case SCRIPT_TYPE::TINTEDBGSCRIPT:
		return L"CTintedBGScript";
		break;

	case SCRIPT_TYPE::WATERSCRIPT:
		return L"CWaterScript";
		break;

	}
	return nullptr;
}