#include "pch.h"
#include "MaterialUI.h"

#include "ListUI.h"
#include "TreeUI.h"
#include "CEditorMgr.h"
#include "ParamUI.h"

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
	ImGui::NewLine();
	ImGui::SameLine(60);
	ImGui::TextColored(HEADER_1, u8"���̴�");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(150.f);
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
	if (ImGui::Button(ICON_FA_COG "##ShaderBtn", ImVec2(22.f, 22.f)))
	{
		ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
		pListUI->SetName("GraphicShader");
		std::vector<string> vecMeshNames;
		CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::GRAPHIC_SHADER, vecMeshNames);
		pListUI->AddList(vecMeshNames);
		pListUI->AddDelegate(this, (DELEGATE_1)&MaterialUI::SelectShader);
		pListUI->SetActive(true);
	}

	// Shader Parameter
	ImGui::NewLine();
	ImGui::NewLine();
	ImGui::SeparatorText(u8"���̴� �Ķ����");
	ImGui::NewLine();
	ShaderParameter();
}

void MaterialUI::ShaderParameter()
{
	Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();
	Ptr<CGraphicShader> pShader = pMtrl->GetShader();

	if (nullptr == pShader)
		return;

	// Shader �� �䱸�ϴ� �Ķ���� ����� �����´�.
	const std::vector<tScalarParam>& vecScalarParam = pShader->GetScalarParam();
	const std::vector<tTexParam>& vecTexParam = pShader->GetTexParam();

	vecScalarParam;
	for (size_t i = 0; i < vecScalarParam.size(); ++i)
	{
		switch (vecScalarParam[i].ParamType)
		{
		case INT_0:
		case INT_1:
		case INT_2:
		case INT_3:
		{
			int data = *((int*)pMtrl->GetScalarParam(vecScalarParam[i].ParamType));
			if (ParamUI::InputInt(&data, vecScalarParam[i].strDesc))
			{
				pMtrl->SetScalarParam(vecScalarParam[i].ParamType, data);
			}
		}

		break;
		case FLOAT_0:
		case FLOAT_1:
		case FLOAT_2:
		case FLOAT_3:
		{
			float data = *((float*)pMtrl->GetScalarParam(vecScalarParam[i].ParamType));
			if (ParamUI::InputFloat(&data, vecScalarParam[i].strDesc))
			{
				pMtrl->SetScalarParam(vecScalarParam[i].ParamType, data);
			}
		}
		break;
		case VEC2_0:
		case VEC2_1:
		case VEC2_2:
		case VEC2_3:
		{
			Vec2 data = *((Vec2*)pMtrl->GetScalarParam(vecScalarParam[i].ParamType));
			if (ParamUI::InputVec2(&data, vecScalarParam[i].strDesc))
			{
				pMtrl->SetScalarParam(vecScalarParam[i].ParamType, data);
			}
		}
		break;
		case VEC4_0:
		case VEC4_1:
		case VEC4_2:
		{
			Vec4 data = *((Vec4*)pMtrl->GetScalarParam(vecScalarParam[i].ParamType));
			if (ParamUI::InputVec4(&data, vecScalarParam[i].strDesc))
			{
				pMtrl->SetScalarParam(vecScalarParam[i].ParamType, data);
			}
		}
		break;
		case VEC4_3:
		{
			Vec4 data = *((Vec4*)pMtrl->GetScalarParam(vecScalarParam[i].ParamType));
			if (ParamUI::ColorVec4(&data, vecScalarParam[i].strDesc))
			{
				pMtrl->SetScalarParam(vecScalarParam[i].ParamType, data);
			}
		}
		break;
		case MAT_0:
		case MAT_1:
		case MAT_2:
		case MAT_3:
		{
			Matrix data = *((Matrix*)pMtrl->GetScalarParam(vecScalarParam[i].ParamType));
		}
		break;
		}
	}

	// Texture �Ķ����
	for (size_t i = 0; i < vecTexParam.size(); ++i)
	{
		Ptr<CTexture> pCurTex = pMtrl->GetTexParam(vecTexParam[i].ParamType);
		if (ParamUI::InputTexture(pCurTex, vecTexParam[i].strDesc, this, (DELEGATE_1)&MaterialUI::ChangeTexture))
		{
			pMtrl->SetTexParam(vecTexParam[i].ParamType, pCurTex);
			m_SelectTexParam = vecTexParam[i].ParamType;
		}
	}
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

void MaterialUI::ChangeTexture(DWORD_PTR Param)
{
	// �ؽ��� �Ķ���͸� �Է¹��� ����
	Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();

	// ���������� ������ �׸��� �������� ListUI �� ���ؼ� �˾Ƴ�
	ListUI* pListUI = (ListUI*)Param;
	string strName = pListUI->GetSelectName();

	if ("None" == strName)
	{
		pMtrl->SetTexParam(m_SelectTexParam, nullptr);
		return;
	}

	wstring strAssetName(strName.begin(), strName.end());

	Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(strAssetName);

	MD_ENGINE_ASSERT(pMtrl.Get() != nullptr, L"Material ���� ����");

	pMtrl->SetTexParam(m_SelectTexParam, pTex);
}