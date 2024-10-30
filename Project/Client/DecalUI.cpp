#include "pch.h"
#include "DecalUI.h"
#include <Engine/CDecal.h>
#include <Engine/CAssetMgr.h>
#include "TreeUI.h"
#include "ListUI.h"
#include "CEditorMgr.h"
#include "ParamUI.h"
DecalUI::DecalUI()
	: ComponentUI(COMPONENT_TYPE::DECAL)
{
}

void DecalUI::Update()
{
	Title();

	if (!Collapsed())
	{
		CDecal* pDecal = GetTargetObject()->Decal();
		
		Ptr<CTexture> pDecalTex = pDecal->GetDecalTexture();
		Ptr<CTexture> pEmissiveTex = pDecal->GetEmissiveTexture();

		if (ParamUI::InputTexture(pDecalTex, "DecalTex", this, (DELEGATE_1)&DecalUI::SelectDecalTexture))
		{
			pDecal->SetDecalTexture((CTexture*)pDecalTex.Get());
		}

		if (ParamUI::InputTexture(pEmissiveTex, "EmissiveTex", this, (DELEGATE_1)&DecalUI::SelectEmissiveTexture))
		{
			pDecal->SetDecalTexture((CTexture*)pEmissiveTex.Get());
		}

	}
}

void DecalUI::SelectDecalTexture(DWORD_PTR _ListUI)
{
	CDecal* pDecal = GetTargetObject()->Decal();

	ListUI* pListUI = (ListUI*)_ListUI;
	string strName = pListUI->GetSelectName();

	if (strName == "None")
	{
		pDecal->SetDecalTexture(nullptr);
		return;
	}

	wstring strAssetName = wstring(strName.begin(), strName.end());

	Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(strAssetName);

	assert(pTex.Get());

	pDecal->SetDecalTexture(pTex);
}

void DecalUI::SelectEmissiveTexture(DWORD_PTR _ListUI)
{
	CDecal* pDecal = GetTargetObject()->Decal();

	ListUI* pListUI = (ListUI*)_ListUI;
	string strName = pListUI->GetSelectName();

	if (strName == "None")
	{
		pDecal->SetEmissiveTexture(nullptr);
		return;
	}

	wstring strAssetName = wstring(strName.begin(), strName.end());

	Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(strAssetName);

	assert(pTex.Get());

	pDecal->SetEmissiveTexture(pTex);
}
