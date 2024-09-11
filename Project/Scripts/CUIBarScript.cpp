#include "spch.h"
#include "CUIBarScript.h"
#include "../Client/CPlayerManager.h"
#include "../Client/CEditorMgr.h"

CUIBarScript::CUIBarScript()
	: CScript(SCRIPT_TYPE::UIBARSCRIPT)
{
	AddScriptParam(SCRIPT_PARAM::INT, u8"종류", &m_Type, 0, 0, "0: HP, 1: MP, 2: Stamina");
	AddScriptParam(SCRIPT_PARAM::TEXTURE, u8"텍스처", &m_FillTex);
}

void CUIBarScript::Begin()
{
	MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_FillTex);
	const std::shared_ptr<PlayerStatus>& playerstat = CPlayerManager::GetInst()->GetPlayerStatusRef();
	switch (m_Type)
	{
	case BarType::HP:
	{
		Transform()->SetRelativeScale(Vec3((playerstat->HP / playerstat->maxHP) * 0.83f, 0.28f, 1.f));
		Transform()->SetRelativePos(Vec3(0.07f, 0.05f, 1.f));
		break;
	}
	case BarType::MP:
	{
		Transform()->SetRelativeScale(Vec3((playerstat->MP / playerstat->maxMP) * 0.83f, 0.28f, 1.f));
		Transform()->SetRelativePos(Vec3(0.07f, 0.05f, 1.f));
		break;
	}
	case BarType::STAMINA:
	{
		Transform()->SetRelativeScale(Vec3(playerstat->Stamina / playerstat->maxStamina, 0.5f, 1.f));
		break;
	}
	case BarType::NONE:
	{
		break;
	}
	}
}

void CUIBarScript::Tick()
{
	const std::shared_ptr<PlayerStatus>& playerstat = CPlayerManager::GetInst()->GetPlayerStatusRef();

	switch (m_Type)
	{
	case BarType::HP:
	{
		Transform()->SetRelativeScale(Vec3((playerstat->HP / playerstat->maxHP) * 0.83f, 0.28f, 1.f));
		break;
	}
	case BarType::MP:
	{
		Transform()->SetRelativeScale(Vec3((playerstat->MP / playerstat->maxMP) * 0.83f, 0.28f, 1.f));
		break;
	}
	case BarType::STAMINA:
	{
		Transform()->SetRelativeScale(Vec3(playerstat->Stamina / playerstat->maxStamina, 0.5f, 1.f));
		break;
	}
	case BarType::NONE:
	{
		break;
	}
	}

	if (KEY_TAP(KEY::_6))
	{
		CPlayerManager::GetInst()->TakeDamage(10);
		EDITOR_TRACE(u8"데미지 10 받음");

	}
	if (KEY_TAP(KEY::_7))
	{
		CPlayerManager::GetInst()->Recover(10);
		EDITOR_TRACE(u8"10 회복");
	}
}

void CUIBarScript::SaveToFile(FILE* _File)
{
	fwrite(&m_Type, sizeof(BarType), 1, _File);
	SaveAssetRef(m_FillTex, _File);
}

void CUIBarScript::LoadFromFile(FILE* _File)
{
	fread(&m_Type, sizeof(BarType), 1, _File);
	LoadAssetRef(m_FillTex, _File);
}
