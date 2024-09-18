#include "spch.h"
#include "CMinimap.h"
#include <Engine/CLevelMgr.h>
#include "CNPCUIScript.h"

CMinimap::CMinimap()
	: CScript(SCRIPT_TYPE::MINIMAP)
{
	AddScriptParam(SCRIPT_PARAM::INT, u8"맵", &m_Type, 0, 0, "0: KohoVill, 1: KohoShrine\n2: Street, 3:Demon, 4: Lineth");
}

void CMinimap::Begin()
{
	m_Player = CLevelMgr::GetInst()->FindObjectByName(L"Player");
	MD_ENGINE_ASSERT(m_Player != nullptr, L"플레이어 못찾음");
	CNPCUIScript* uiScript = static_cast<CNPCUIScript*>(GetOwner()->FindScript((UINT)NPCUISCRIPT));
	uiScript->Activate();

	switch (m_Type)
	{
	case MapType::KOHOVILL:
	{//TODO
		m_BGM = CAssetMgr::GetInst()->FindAsset<CSound>(L"kohovillage");
		break;
	}
	case MapType::KOHOSHRINE:
	{
		m_BGM = CAssetMgr::GetInst()->FindAsset<CSound>(L"kohovillage");
		
		break;
	}
	case MapType::KOHOSTREET:
	{
		m_BGM = CAssetMgr::GetInst()->FindAsset<CSound>(L"kohovillage");

		break;
	}
	case MapType::DEMON:
	{
		m_BGM = CAssetMgr::GetInst()->FindAsset<CSound>(L"kohovillage");
		break;
	}
	case MapType::LINETH:
	{
		m_BGM = CAssetMgr::GetInst()->FindAsset<CSound>(L"kohovillage");
		break;
	}
	}

	m_PlayerIcon = CLevelMgr::GetInst()->FindObjectByName(L"PlayerIcon");
	uiScript = static_cast<CNPCUIScript*>(m_PlayerIcon->FindScript((UINT)NPCUISCRIPT));
	uiScript->SetSpeed(500.f);

	m_BGM->Play(0, 0.5f, false);
}

void CMinimap::Tick()
{
	m_Acc += DT;
	if (m_Acc > m_Timer)
	{
		CNPCUIScript* uiScript;
		uiScript = static_cast<CNPCUIScript*>(m_PlayerIcon->FindScript((UINT)NPCUISCRIPT));

		if (m_Type == MapType::KOHOSHRINE)
		{
			Vec3& playerIconPos = m_PlayerIcon->Transform()->GetRelativePosRef();
			Vec3& playerPos = m_Player->Transform()->GetRelativePosRef();

			if (playerPos.x > 800.f)
				playerIconPos.x = 0.115f;
			else
				playerIconPos.x = 0.f;
		}

		uiScript->IsActive() ? uiScript->Deactivate() : uiScript->Activate();

		m_Acc = 0.f;
	}
}

void CMinimap::SaveToFile(FILE* _File)
{
	SaveAssetRef(m_MapTileTex, _File);
	fwrite(&m_Type, sizeof(MapType), 1, _File);
}

void CMinimap::LoadFromFile(FILE* _File)
{
	LoadAssetRef(m_MapTileTex, _File);
	fread(&m_Type, sizeof(MapType), 1, _File);
}
