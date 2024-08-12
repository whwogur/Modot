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
		const wstring& wstrKey = pCS->GetKey();
		const wstring& wstrPath = pCS->GetRelativePath();
		string strKey(wstrKey.begin(), wstrKey.end());
		string strPath(wstrPath.begin(), wstrPath.end());
		ImGui::TextColored(HEADER_1, "Name");
		ImGui::SameLine();
		ImGui::Text(strKey.c_str());
		ImGui::TextColored(HEADER_1, "Path");
		ImGui::SameLine();
		ImGui::Text(strPath.empty()? "No Path" : strPath.c_str());
	}
}
