#include "pch.h"
#include "MaterialUI.h"

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


	// Shader Parameter

}