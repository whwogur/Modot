#include "spch.h"
#include "CManagerScript.h"

CManagerScript::CManagerScript()
	: CScript(SCRIPT_TYPE::MANAGERSCRIPT)
{
}

void CManagerScript::Begin()
{
	Ptr<CSound> m_BGM = CAssetMgr::GetInst()->FindAsset<CSound>(L"kohovillage"); // юс╫ц
	m_BGM->Play(0, 10, false);
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
