#include "spch.h"
#include "CUIBarScript.h"
#include "../Client/CPlayerManager.h"
#include "../Client/CEditorMgr.h"
#include "CNPCUIScript.h"

CUIBarScript::CUIBarScript()
	: CScript(SCRIPT_TYPE::UIBARSCRIPT)
{
	AddScriptParam(SCRIPT_PARAM::INT, u8"����", &m_Type, 0, 0, "0: HP, 1: MP, 2: Stamina,\n3: BossHP");
	AddScriptParam(SCRIPT_PARAM::TEXTURE, u8"�ؽ�ó", &m_FillTex);
	AddScriptParam(SCRIPT_PARAM::COLOR, u8"ƾƮ", &m_Tint);
}

void CUIBarScript::Begin()
{
	MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_FillTex);
	const std::shared_ptr<PlayerStatus>& playerstat = CPlayerManager::GetInst()->GetPlayerStatusRef();
	switch (m_Type)
	{
	case BarType::HP:
	{
		//Transform()->SetRelativeScale(Vec3(((float)playerstat->HP / playerstat->maxHP) * 0.83f, 0.28f, 1.f));
		//Transform()->SetRelativePos(Vec3(0.07f, 0.05f, 1.f));
		break;
	}
	case BarType::MP:
	{
		//Transform()->SetRelativeScale(Vec3(((float)playerstat->MP / playerstat->maxMP) * 0.83f, 0.28f, 1.f));
		//Transform()->SetRelativePos(Vec3(0.07f, 0.05f, 1.f));
		break;
	}
	case BarType::STAMINA:
	{
		//Transform()->SetRelativeScale(Vec3((float)playerstat->Stamina / playerstat->maxStamina, 0.5f, 1.f));
		//Transform()->SetRelativePos(Vec3(0.f, 0.04f, 1.f));
		break;
	}
	case BarType::BOSSHP:
	{
		//Transform()->SetRelativeScale(Vec3(m_BossHP / 100.f, 0.5f, 1.f));
		//Transform()->SetRelativePos(Vec3(0.037f, -0.25f, 1.f));
		break;
	}
	case BarType::NONE:
	{
		MD_ENGINE_ERROR(L"UI Ÿ�� ���� �ȵ�");
		break;
	}
	}
}

void CUIBarScript::Tick()
{
	const std::shared_ptr<PlayerStatus>& playerstat = CPlayerManager::GetInst()->GetPlayerStatusRef();
	Vec3& barPos = Transform()->GetRelativePosRef();

	switch (m_Type)
	{
	case BarType::HP:
	{
		Transform()->SetRelativeScale(Vec3((playerstat->HP / playerstat->maxHP) * 144.41f, 9.52f, 1.f));
		barPos.x = 13.57f - (playerstat->maxHP - playerstat->HP) / 144.41f * 18.5f;
		break;
	}
	case BarType::MP:
	{
		Transform()->SetRelativeScale(Vec3((playerstat->MP / playerstat->maxMP) * 149.83f, 8.28f, 1.f));
		barPos.x = 8.07f - (playerstat->maxHP - playerstat->HP) / 149.83f * 18.5f;
		break;
	}
	case BarType::STAMINA:
	{
		if (playerstat->Stamina >= playerstat->maxStamina)
		{
			CNPCUIScript* npcUIScript = static_cast<CNPCUIScript*>(GetOwner()->GetParent()->FindScript(SCRIPT_TYPE::NPCUISCRIPT));
			if (npcUIScript != nullptr)
			{
				if (npcUIScript->IsActive())
					npcUIScript->Deactivate();
			}
		}
		else
		{
			CNPCUIScript* npcUIScript = static_cast<CNPCUIScript*>(GetOwner()->GetParent()->FindScript(SCRIPT_TYPE::NPCUISCRIPT));
			if (npcUIScript != nullptr)
			{
				if (!npcUIScript->IsActive())
					npcUIScript->Activate();
			}
			Transform()->SetRelativeScale(Vec3(playerstat->Stamina / playerstat->maxStamina * 98.2f, 7.3f, 1.f));
			barPos.x = 0.f - (playerstat->maxStamina - playerstat->Stamina) / 98.2 * 43.f;
		}
		
		break;
	}
	case BarType::BOSSHP:
	{
		Transform()->SetRelativeScale(Vec3((m_BossHP / 100.f) * 0.9, 0.2f, 1.f));
		break;
	}
	case BarType::NONE:
	{
		break;
	}
	}

	if (GetOwner()->GetParent() == nullptr)
			MeshRender()->GetDynamicMaterial()->SetScalarParam(VEC4_0, m_Tint);

	if (m_Shake)
	{
		m_Acc += DT;
		Vec3& objPos = GetOwner()->GetParent()->Transform()->GetRelativePosRef();
		int x = (rand() % 10 * 2) - 10;
		int y = (rand() % 10 * 2) - 10;
		x = x == 0 ? 1 : x;
		y = y == 0 ? 1 : y;

		objPos.x += (float)x * 0.1f;
		objPos.y += (float)y * 0.1f;


		if (m_Acc > m_Timer)
		{
			m_Acc = 0.f;
			m_Shake = false;
			GetOwner()->GetParent()->Transform()->SetRelativePos(m_OriginalPos);
		}
	}
}

void CUIBarScript::SaveToFile(FILE* _File)
{
	fwrite(&m_Type, sizeof(BarType), 1, _File);
	SaveAssetRef(m_FillTex, _File);
	fwrite(&m_Tint, sizeof(Vec4), 1, _File);
}

void CUIBarScript::LoadFromFile(FILE* _File)
{
	fread(&m_Type, sizeof(BarType), 1, _File);
	LoadAssetRef(m_FillTex, _File);
	fread(&m_Tint, sizeof(Vec4), 1, _File);
}

void CUIBarScript::Shake()
{
	m_Shake = true;
	m_OriginalPos = GetOwner()->GetParent()->Transform()->GetRelativePos();
}
