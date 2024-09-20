#include "spch.h"
#include "CScriptMgr.h"

#include "CArrowScript.h"
#include "CAttackScript.h"
#include "CBackGroundScript.h"
#include "CBellScript.h"
#include "CBlurControl.h"
#include "CCameraMoveScript.h"
#include "CDemonScript.h"
#include "CHellfireScript.h"
#include "CLevelTransitionScript.h"
#include "CLinethScript.h"
#include "CMinimap.h"
#include "CMonsterAttack.h"
#include "CNPCScript.h"
#include "CNPCUIScript.h"
#include "CParticleScript.h"
#include "CPlatformScript.h"
#include "CPlayerScript.h"
#include "CSpriteRenderScript.h"
#include "CTintedBGScript.h"
#include "CTitleSequence.h"
#include "CUIBarScript.h"
#include "CWaterScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CArrowScript");
	_vec.push_back(L"CAttackScript");
	_vec.push_back(L"CBackGroundScript");
	_vec.push_back(L"CBellScript");
	_vec.push_back(L"CBlurControl");
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CDemonScript");
	_vec.push_back(L"CHellfireScript");
	_vec.push_back(L"CLevelTransitionScript");
	_vec.push_back(L"CLinethScript");
	_vec.push_back(L"CMinimap");
	_vec.push_back(L"CMonsterAttack");
	_vec.push_back(L"CNPCScript");
	_vec.push_back(L"CNPCUIScript");
	_vec.push_back(L"CParticleScript");
	_vec.push_back(L"CPlatformScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CSpriteRenderScript");
	_vec.push_back(L"CTintedBGScript");
	_vec.push_back(L"CTitleSequence");
	_vec.push_back(L"CUIBarScript");
	_vec.push_back(L"CWaterScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CArrowScript" == _strScriptName)
		return new CArrowScript;
	if (L"CAttackScript" == _strScriptName)
		return new CAttackScript;
	if (L"CBackGroundScript" == _strScriptName)
		return new CBackGroundScript;
	if (L"CBellScript" == _strScriptName)
		return new CBellScript;
	if (L"CBlurControl" == _strScriptName)
		return new CBlurControl;
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CDemonScript" == _strScriptName)
		return new CDemonScript;
	if (L"CHellfireScript" == _strScriptName)
		return new CHellfireScript;
	if (L"CLevelTransitionScript" == _strScriptName)
		return new CLevelTransitionScript;
	if (L"CLinethScript" == _strScriptName)
		return new CLinethScript;
	if (L"CMinimap" == _strScriptName)
		return new CMinimap;
	if (L"CMonsterAttack" == _strScriptName)
		return new CMonsterAttack;
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
	if (L"CTitleSequence" == _strScriptName)
		return new CTitleSequence;
	if (L"CUIBarScript" == _strScriptName)
		return new CUIBarScript;
	if (L"CWaterScript" == _strScriptName)
		return new CWaterScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::ARROWSCRIPT:
		return new CArrowScript;
		break;
	case (UINT)SCRIPT_TYPE::ATTACKSCRIPT:
		return new CAttackScript;
		break;
	case (UINT)SCRIPT_TYPE::BACKGROUNDSCRIPT:
		return new CBackGroundScript;
		break;
	case (UINT)SCRIPT_TYPE::BELLSCRIPT:
		return new CBellScript;
		break;
	case (UINT)SCRIPT_TYPE::BLURCONTROL:
		return new CBlurControl;
		break;
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::DEMONSCRIPT:
		return new CDemonScript;
		break;
	case (UINT)SCRIPT_TYPE::HELLFIRESCRIPT:
		return new CHellfireScript;
		break;
	case (UINT)SCRIPT_TYPE::LEVELTRANSITIONSCRIPT:
		return new CLevelTransitionScript;
		break;
	case (UINT)SCRIPT_TYPE::LINETHSCRIPT:
		return new CLinethScript;
		break;
	case (UINT)SCRIPT_TYPE::MINIMAP:
		return new CMinimap;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERATTACK:
		return new CMonsterAttack;
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
	case (UINT)SCRIPT_TYPE::TITLESEQUENCE:
		return new CTitleSequence;
		break;
	case (UINT)SCRIPT_TYPE::UIBARSCRIPT:
		return new CUIBarScript;
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
	case SCRIPT_TYPE::ARROWSCRIPT:
		return L"CArrowScript";
		break;

	case SCRIPT_TYPE::ATTACKSCRIPT:
		return L"CAttackScript";
		break;

	case SCRIPT_TYPE::BACKGROUNDSCRIPT:
		return L"CBackGroundScript";
		break;

	case SCRIPT_TYPE::BELLSCRIPT:
		return L"CBellScript";
		break;

	case SCRIPT_TYPE::BLURCONTROL:
		return L"CBlurControl";
		break;

	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CCameraMoveScript";
		break;

	case SCRIPT_TYPE::DEMONSCRIPT:
		return L"CDemonScript";
		break;

	case SCRIPT_TYPE::HELLFIRESCRIPT:
		return L"CHellfireScript";
		break;

	case SCRIPT_TYPE::LEVELTRANSITIONSCRIPT:
		return L"CLevelTransitionScript";
		break;

	case SCRIPT_TYPE::LINETHSCRIPT:
		return L"CLinethScript";
		break;

	case SCRIPT_TYPE::MINIMAP:
		return L"CMinimap";
		break;

	case SCRIPT_TYPE::MONSTERATTACK:
		return L"CMonsterAttack";
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

	case SCRIPT_TYPE::TITLESEQUENCE:
		return L"CTitleSequence";
		break;

	case SCRIPT_TYPE::UIBARSCRIPT:
		return L"CUIBarScript";
		break;

	case SCRIPT_TYPE::WATERSCRIPT:
		return L"CWaterScript";
		break;

	}
	return nullptr;
}