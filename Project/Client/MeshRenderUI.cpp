#include "pch.h"
#include "MeshRenderUI.h"

#include "Engine/CAssetMgr.h"
#include "Engine/CMeshRender.h"
#include "Engine/CMesh.h"
#include "Engine/CMaterial.h"
#include "CEditorMgr.h"
#include "ListUI.h"
#include "TreeUI.h"
#include "ClientStatic.h"
constexpr const char* noMatStr = "Empty";
constexpr const char* texParamString[] = { "Tex 0", "Tex 1", "Tex 2", "Tex 3" , "Tex 4" , "Tex 5" , "Tex Cube 0" , "Tex Cube 1" , "Tex Arr 0" , "Tex Arr 1" };
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
		UINT curIdx = _MeshRender->GetMaterialIdx();
		Ptr<CMaterial> pMtrl = _MeshRender->GetMaterial(curIdx);
		std::vector<tMtrlSet>& mtSetRef = _MeshRender->GetVecMtrlRef();
		if (ImGui::BeginCombo("##MaterialPreview", "Material Set"))
		{
			for (int i = 0; i < mtrlSetCnt; ++i)
			{
				string strSelectable("MaterialSet " + std::to_string(i));
				
				if (ImGui::Selectable(strSelectable.c_str()))
				{
					_MeshRender->SetMaterialIdx(i);
				}
			}
			ImGui::EndCombo();
		}
		MaterialSetInfo(&mtSetRef[curIdx]);
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

void MeshRenderUI::MaterialSetInfo(tMtrlSet* _CurMaterialSet)
{
	ImGui::SeparatorText("Material Set Info");
	// curMat
	ImGui::BeginTabBar("##MaterialSetInfoTab");
	if (ImGui::BeginTabItem("Current"))
	{
		ImGui::TextColored(HEADER_1, "Current Material");
		MaterialInfo(_CurMaterialSet->pCurMtrl);
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Dynamic"))
	{
		ImGui::TextColored(HEADER_1, "Dynamic Material");
		MaterialInfo(_CurMaterialSet->pDynamicMtrl);
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Shared"))
	{
		ImGui::TextColored(HEADER_1, "Shared Material");
		MaterialInfo(_CurMaterialSet->pSharedMtrl);
		ImGui::EndTabItem();
	}
	ImGui::EndTabBar();
}

void MeshRenderUI::MaterialInfo(Ptr<CMaterial> _Material)
{
	if (nullptr == _Material) return;

	// TEXPARAM
	for (UINT i = 0; i < (UINT)TEX_PARAM::END; ++i)
	{
		Ptr<CTexture> matImg = _Material->GetTexParam((TEX_PARAM)i);
		if (matImg != nullptr)
		{
			ImGui::TextColored(HEADER_2, texParamString[i]);
			ImGui::SameLine(INDENT_2);
			ImGui::Image((ImTextureID)matImg->GetSRV().Get(), { 150, 150 });
		}
	}

	Vec4& ambient = _Material->GetAmbRef();
	ImGui::TextColored(HEADER_2, "Ambient");
	ImGui::SameLine(INDENT_1);
	ClientStatic::ColorPicker("##MaterialAmbientPicker", ambient);

	Vec4& diffuse = _Material->GetDiffRef();
	ImGui::TextColored(HEADER_2, "Diffuse");
	ImGui::SameLine(INDENT_1);
	ClientStatic::ColorPicker("##MaterialDiffusePicker", diffuse);

	Vec4& emissive = _Material->GetEmiRef();
	ImGui::TextColored(HEADER_2, "Emissive");
	ImGui::SameLine(INDENT_1);
	ClientStatic::ColorPicker("##MaterialEmissivePicker", emissive);

	Vec4& specular = _Material->GetSpecRef();
	ImGui::TextColored(HEADER_2, "Specular");
	ImGui::SameLine(INDENT_1);
	ClientStatic::ColorPicker("##MaterialSpecularPicker", specular);
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
