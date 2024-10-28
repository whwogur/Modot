#include "pch.h"
#include "DecalUI.h"
#include <Engine/CDecal.h>
#include <Engine/CAssetMgr.h>
#include "TreeUI.h"
#include "ListUI.h"
#include "CEditorMgr.h"
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

		if (pDecalTex != nullptr)
		{
			bool& decalActive = pDecal->GetDecalEnableRef();
			ToggleButton("Decal", &decalActive);

			ImGui::Image(pDecalTex->GetSRV().Get(), { 150, 150 });

			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
				if (payload)
				{
					TreeNode** ppNode = (TreeNode**)payload->Data;
					TreeNode* pNode = *ppNode;

					Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
					if (ASSET_TYPE::TEXTURE == pAsset->GetAssetType())
					{
						pDecal->SetDecalTexture((CTexture*)pAsset.Get());
					}
				}

				ImGui::EndDragDropTarget();
			}

			ImGui::SameLine();
			if (ImGui::Button(ICON_FA_COG "##DecalBtn", ImVec2(20.f, 20.f)))
			{
				ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
				pListUI->SetName("Mesh");
				std::vector<string> vecMeshNames;
				CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecMeshNames);
				pListUI->AddList(vecMeshNames);
				pListUI->AddDelegate(this, (DELEGATE_1)&DecalUI::SelectDecalTexture);
				pListUI->SetActive(true);
			}
		}

		if (pEmissiveTex != nullptr)
		{
			bool& emissionActive = pDecal->GetEmissiveEnableRef();
			ToggleButton("Emission", &emissionActive);

			ImGui::Image(pEmissiveTex->GetSRV().Get(), { 150, 150 });

			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
				if (payload)
				{
					TreeNode** ppNode = (TreeNode**)payload->Data;
					TreeNode* pNode = *ppNode;

					Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
					if (ASSET_TYPE::TEXTURE == pAsset->GetAssetType())
					{
						pDecal->SetEmissiveTexture((CTexture*)pAsset.Get());
					}
				}

				ImGui::EndDragDropTarget();
			}

			ImGui::SameLine();
			if (ImGui::Button(ICON_FA_COG "##DecalBtn", ImVec2(20.f, 20.f)))
			{
				ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
				pListUI->SetName("Mesh");
				std::vector<string> vecMeshNames;
				CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecMeshNames);
				pListUI->AddList(vecMeshNames);
				pListUI->AddDelegate(this, (DELEGATE_1)&DecalUI::SelectEmissiveTexture);
				pListUI->SetActive(true);
			}
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
