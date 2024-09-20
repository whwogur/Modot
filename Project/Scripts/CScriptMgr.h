#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
	ARROWSCRIPT,
	ATTACKSCRIPT,
	BACKGROUNDSCRIPT,
	BELLSCRIPT,
	BLURCONTROL,
	CAMERAMOVESCRIPT,
	DEMONSCRIPT,
	HELLFIRESCRIPT,
	LEVELTRANSITIONSCRIPT,
	LINETHSCRIPT,
	MANAGERSCRIPT,
	MINIMAP,
	MONSTERATTACK,
	NPCSCRIPT,
	NPCUISCRIPT,
	PARTICLESCRIPT,
	PLATFORMSCRIPT,
	PLAYERSCRIPT,
	SPRITERENDERSCRIPT,
	TINTEDBGSCRIPT,
	TITLESEQUENCE,
	UIBARSCRIPT,
	WATERSCRIPT,
};

using namespace std;

class CScript;

class CScriptMgr
{
public:
	static void GetScriptInfo(vector<wstring>& _vec);
	static CScript * GetScript(const wstring& _strScriptName);
	static CScript * GetScript(UINT _iScriptType);
	static const wchar_t * GetScriptName(CScript * _pScript);
};
