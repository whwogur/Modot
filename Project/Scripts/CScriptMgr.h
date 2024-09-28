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
	CAT,
	DEMONSCRIPT,
	DEMONSEQUENCE,
	HELLFIRESCRIPT,
	LEVELTRANSITIONSCRIPT,
	LINETHSCRIPT,
	LINETHSEQUENCE,
	MENUSCRIPT,
	MINIMAP,
	MONSTERATTACK,
	NPCBEHAVIOR,
	NPCSCRIPT,
	NPCUISCRIPT,
	PARTICLESCRIPT,
	PLATFORMSCRIPT,
	PLAYERSCRIPT,
	SIGIL,
	SPRITERENDERSCRIPT,
	TINTEDBGSCRIPT,
	TITLESEQUENCE,
	UIBARSCRIPT,
	WALL,
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
