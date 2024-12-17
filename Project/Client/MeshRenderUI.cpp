#include "pch.h"
#include "MeshRenderUI.h"

#include "Engine/CAssetMgr.h"
#include "Engine/CMeshRender.h"
#include "Engine/CMesh.h"
#include "Engine/CMaterial.h"
#include "CEditorMgr.h"
#include "ListUI.h"
#include "TreeUI.h"
constexpr const char* noMatStr = "Empty";

MeshRenderUI::MeshRenderUI()
	: ComponentUI(COMPONENT_TYPE::MESHRENDER)
{
}

void MeshRenderUI::Update()
{
	Title();
	if (!Collapsed())
	{
		CMeshRender* pMeshRender = GetTargetObject()->MeshRender();

		if (pMeshRender != nullptr)
		{
			Material(pMeshRender);
			Mesh(pMeshRender);
		}
	}
}

void MeshRenderUI::Mesh(CMeshRender* _MeshRender)
{
	Ptr<CMesh> pMesh = _MeshRender->GetMesh();

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
				_MeshRender->SetMesh((CMesh*)pAsset.Get());
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	if (ImGui::Button(ICON_FA_COG "##MeshBtn", ImVec2(20.f, 20.f)))
	{
		ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
		pListUI->SetName("Mesh");
		std::vector<string> vecMeshNames;
		CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::MESH, vecMeshNames);
		pListUI->AddList(vecMeshNames);
		pListUI->AddDelegate(this, (DELEGATE_1)&MeshRenderUI::SelectMesh);
		pListUI->SetActive(true);
	}
}

void MeshRenderUI::Material(CMeshRender* _MeshRender)
{

	UINT mtrlSetCnt = _MeshRender->GetMaterialCount();
	ImGui::Text("Material");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(150.f);
	if (mtrlSetCnt)
	{
		Ptr<CMaterial> pMtrl = _MeshRender->GetMaterial(_MeshRender->GetMaterialIdx());
		string MtrlName(pMtrl->GetKey().begin(), pMtrl->GetKey().end());
		if (ImGui::BeginCombo("##MaterialPreview", MtrlName.c_str()))
		{
			std::vector<tMtrlSet>& mtSetRef = _MeshRender->GetVecMtrlRef();
			for (int i = 0; i < mtrlSetCnt; ++i)
			{
				static string strSelectable(std::to_string(i) + "##MaterialSet");
				
				if (ImGui::Selectable(strSelectable.c_str()))
				{
					_MeshRender->SetMaterialIdx(i);
				}
			}
			ImGui::EndCombo();
		}
	}
	else
	{
		ImGui::InputText("##MaterialKey", const_cast<char*>(noMatStr), ImGuiInputTextFlags_ReadOnly);
	}

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
				_MeshRender->EmplaceBackMaterial((CMaterial*)pAsset.Get());
			}
		}

		ImGui::EndDragDropTarget();
	}
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
