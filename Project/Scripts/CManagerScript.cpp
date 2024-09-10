#include "spch.h"
#include "CManagerScript.h"

CManagerScript::CManagerScript()
	: CScript(SCRIPT_TYPE::MANAGERSCRIPT)
{
}

void CManagerScript::Begin()
{
	m_BGM->Play(0, 10.f, false);
}

void CManagerScript::Tick()
{
}

void CManagerScript::SaveToFile(FILE* _File)
{
}

void CManagerScript::LoadFromFile(FILE* _File)
{
}
