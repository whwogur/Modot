#include "pch.h"
#include "ComputeShaderUI.h"
#include <Engine/CComputeShader.h>
ComputeShaderUI::ComputeShaderUI()
	: AssetUI(ASSET_TYPE::COMPUTE_SHADER)
{
}

void ComputeShaderUI::Update()
{
	Title();

	Ptr<CComputeShader> pCS = (CComputeShader*)GetAsset().Get();
	if (pCS != nullptr)
	{
		ImGui::NewLine();
		ImGui::SeparatorText(u8"정보");
		ImGui::NewLine();
		ImGui::SameLine(60);

		const wstring& wstrKey = pCS->GetKey();
		const wstring& wstrPath = pCS->GetRelativePath();
		string strKey(wstrKey.begin(), wstrKey.end());
		string strPath(wstrPath.begin(), wstrPath.end());
		ImGui::TextColored(HEADER_1, u8"이름");
		ImGui::SameLine(100);
		ImGui::Text(strKey.c_str());

		ImGui::NewLine();
		ImGui::SameLine(60);
		ImGui::TextColored(HEADER_1, u8"경로");
		ImGui::SameLine(100);
		ImGui::Text(strPath.empty()? "NULL" : strPath.c_str());
	}
}
