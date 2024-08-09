#include "spch.h"
#include "CNPCScript.h"

CNPCScript::CNPCScript()
	: CScript(UINT(SCRIPT_TYPE::NPCSCRIPT))
{
}

void CNPCScript::Begin()
{
	GetOwner()->Animator2D()->Play(0, 5.0f, true);
}

void CNPCScript::Tick()
{
}

void CNPCScript::SaveToFile(FILE* _File)
{
}

void CNPCScript::LoadFromFile(FILE* _File)
{
}
