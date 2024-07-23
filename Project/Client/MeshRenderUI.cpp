#include "pch.h"
#include "MeshRenderUI.h"

#include "Engine/CAssetMgr.h"
#include "Engine/CMeshRender.h"
#include "Engine/CMesh.h"
#include "Engine/CMaterial.h"
#include "CEditorMgr.h"
#include "ListUI.h"
#include "TreeUI.h"

MeshRenderUI::MeshRenderUI()
	: ComponentUI(COMPONENT_TYPE::MESHRENDER)
{
}

MeshRenderUI::~MeshRenderUI()
{
}


void MeshRenderUI::Update()
{
	Title();

	ImFont* iconFont = CEditorMgr::GetInst()->GetIconFont();
	ImGui::PushFont(iconFont);

	CMeshRender* pMeshRender = GetTargetObject()->MeshRender();

	Ptr<CMesh> pMesh = pMeshRender->GetMesh();

	string MeshName;

	if (pMesh.Get())
		MeshName = string(pMesh->GetKey().begin(), pMesh->GetKey().end());
	ImGui::Text("Mesh");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(150.f);
	ImGui::InputText("##MeshKey", (char*)MeshName.c_str(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
	
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
		if (payload)
		{
			TreeNode** ppNode = (TreeNode**)payload->Data;
			TreeNode* pNode = *ppNode;

			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (ASSET_TYPE::MESH == pAsset->GetAssetType())
			{
				pMeshRender->SetMesh((CMesh*)pAsset.Get());
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	if (ImGui::Button(ICON_FA_COG "##MeshBtn", ImVec2(20.f, 20.f)))
	{
		ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
		pListUI->SetName("Mesh");
		vector<string> vecMeshNames;
		CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::MESH, vecMeshNames);
		pListUI->AddList(vecMeshNames);
		pListUI->AddDelegate(this, (DELEGATE_1)&MeshRenderUI::SelectMesh);
		pListUI->SetActive(true);
	}

	Ptr<CMaterial> pMtrl = pMeshRender->GetMaterial();

	string MtrlName;
	if (pMtrl.Get())
		MtrlName = string(pMtrl->GetKey().begin(), pMtrl->GetKey().end());
	ImGui::Text("Material");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(150.f);
	ImGui::InputText("##MaterialKey", (char*)MtrlName.c_str(), ImGuiInputTextFlags_ReadOnly);

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
		if (payload)
		{
			TreeNode** ppNode = (TreeNode**)payload->Data;
			TreeNode* pNode = *ppNode;

			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (ASSET_TYPE::MATERIAL == pAsset->GetAssetType())
			{
				pMeshRender->SetMaterial((CMaterial*)pAsset.Get());
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	if (ImGui::Button(ICON_FA_COG "##MtrlBtn", ImVec2(20.f, 20.f)))
	{
		ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
		pListUI->SetName("Material");
		vector<string> vecMtrlNames;
		CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::MATERIAL, vecMtrlNames);
		pListUI->AddList(vecMtrlNames);
		pListUI->AddDelegate(this, (DELEGATE_1)&MeshRenderUI::SelectMaterial);
		pListUI->SetActive(true);
	}

	ImGui::PopFont();
}

void MeshRenderUI::SelectMesh(DWORD_PTR _ListUI)
{
	CMeshRender* pMeshRender = GetTargetObject()->MeshRender();

	ListUI* pListUI = (ListUI*)_ListUI;
	string strName = pListUI->GetSelectName();

	if (strName == "None")
	{
		pMeshRender->SetMesh(nullptr);
		return;
	}

	wstring strAssetName = wstring(strName.begin(), strName.end());

	Ptr<CMesh> pMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(strAssetName);

	assert(pMesh.Get());

	pMeshRender->SetMesh(pMesh);
}

void MeshRenderUI::SelectMaterial(DWORD_PTR _ListUI)
{
	CMeshRender* pMeshRender = GetTargetObject()->MeshRender();

	ListUI* pListUI = (ListUI*)_ListUI;
	string strName = pListUI->GetSelectName();

	if ("None" == strName)
	{
		pMeshRender->SetMaterial(nullptr);
		return;
	}

	wstring strAssetName = wstring(strName.begin(), strName.end());

	Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(strAssetName);

	assert(pMtrl.Get());

	pMeshRender->SetMaterial(pMtrl);
}