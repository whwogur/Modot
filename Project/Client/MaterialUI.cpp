#include "pch.h"
#include "MaterialUI.h"

#include "ListUI.h"
#include "TreeUI.h"
#include "CEditorMgr.h"

#include <Engine/CAssetMgr.h>
MaterialUI::MaterialUI()
	: AssetUI(ASSET_TYPE::MATERIAL)
{
}

MaterialUI::~MaterialUI()
{
}


void MaterialUI::Update()
{
	Title();

	// ���� �̸�
	OutputAssetName();

	Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();

	// �����ϴ� ���̴� �̸�
	Ptr<CGraphicShader> pShader = pMtrl->GetShader();
	string Name;

	if (nullptr == pShader)
		Name = "None";
	else
		Name = string(pShader->GetKey().begin(), pShader->GetKey().end());

	// ���̴� �̸�
	ImGui::Text("Name");
	ImGui::SameLine(100);
	ImGui::InputText("##ShaderName", (char*)Name.c_str(), Name.length(), ImGuiInputTextFlags_ReadOnly);

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
		if (payload)
		{
			TreeNode** ppNode = (TreeNode**)payload->Data;
			TreeNode* pNode = *ppNode;

			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (ASSET_TYPE::GRAPHIC_SHADER == pAsset->GetAssetType())
			{
				pMtrl->SetShader((CGraphicShader*)pAsset.Get());
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	if (ImGui::Button("##ShaderBtn", ImVec2(18.f, 18.f)))
	{
		ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
		pListUI->SetName("GraphicShader");
		vector<string> vecMeshNames;
		CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::GRAPHIC_SHADER, vecMeshNames);
		pListUI->AddList(vecMeshNames);
		pListUI->AddDelegate(this, (DELEGATE_1)&MaterialUI::SelectShader);
		pListUI->SetActive(true);
	}

	// Shader Parameter
	ShaderParameter();
}

void MaterialUI::ShaderParameter()
{
	Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();
	Ptr<CGraphicShader> pShader = pMtrl->GetShader();

	if (nullptr == pShader)
		return;

	// Shader �� �䱸�ϴ� �Ķ���� ����� �����´�.
	const vector<tScalarParam>& vecScalarParam = pShader->GetScalarParam();
	const vector<tTexParam>& vecTexParam = pShader->GetTexParam();

	vecScalarParam;

	vecTexParam;
}

void MaterialUI::SelectShader(DWORD_PTR _ListUI)
{
	Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();

	ListUI* pListUI = (ListUI*)_ListUI;
	string strName = pListUI->GetSelectName();

	if ("None" == strName)
	{
		pMtrl->SetShader(nullptr);
		return;
	}

	wstring strAssetName = wstring(strName.begin(), strName.end());

	Ptr<CGraphicShader> pShader = CAssetMgr::GetInst()->FindAsset<CGraphicShader>(strAssetName);

	assert(pShader.Get());

	pMtrl->SetShader(pShader);
}