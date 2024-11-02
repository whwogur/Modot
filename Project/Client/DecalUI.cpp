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
		bool decalEnabled = pDecal->GetDecalEnabled();

		if (ImGui::Checkbox("Decal", &decalEnabled))
		{
			pDecal->ToggleDecalMode(decalEnabled);
		}
		ImGui::SetItemTooltip(u8"ON: µ¥Ä®\nOFF: ¹ß±¤");

		if (ParamUI::InputTexture(pDecalTex, "DecalTex", this, (DELEGATE_1)&DecalUI::SelectDecalTexture))
		{
			pDecal->SetDecalTexture((CTexture*)pDecalTex.Get());
		}

		if (ParamUI::InputTexture(pEmissiveTex, "EmissiveTex", this, (DELEGATE_1)&DecalUI::SelectEmissiveTexture))
		{
			pDecal->SetEmissiveTexture((CTexture*)pEmissiveTex.Get());
		}

		float& emissiveMul = pDecal->GetEmissionMultiplierRef();
		ImGui::SetNextItemWidth(100.f);
		ImGui::SliderFloat(u8"°è¼ö", &emissiveMul, 0.1f, 5.0f, "%.2f");
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
