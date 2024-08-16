#include "spch.h"
#include "CSpriteRenderScript.h"
#include <Engine/CAssetMgr.h>

CSpriteRenderScript::CSpriteRenderScript()
	: CScript(UINT(SCRIPT_TYPE::SPRITERENDERSCRIPT))
{
	m_Sprite = CAssetMgr::GetInst()->FindAsset<CSprite>(L"DefaultSprite");
	AddScriptParam(SCRIPT_PARAM::SPRITE, "Sprite", &m_Sprite);
}

void CSpriteRenderScript::Begin()
{
	if (m_Sprite.Get() != nullptr)
	{
		MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_Sprite->GetAtlasTexture());
		MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_0, m_Sprite->GetLeftTopUV());
		MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_1, m_Sprite->GetSliceUV());
		MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_2, m_Sprite->GetBackgroundUV());
		MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_3, m_Sprite->GetOffsetUV());
	}
}

void CSpriteRenderScript::Tick()
{
}

void CSpriteRenderScript::SaveToFile(FILE* _File)
{
	SaveAssetRef(m_Sprite, _File);
}

void CSpriteRenderScript::LoadFromFile(FILE* _File)
{
	LoadAssetRef(m_Sprite, _File);
}
