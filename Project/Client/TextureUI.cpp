#include "pch.h"
#include "TextureUI.h"

#include <Engine/CTexture.h>

TextureUI::TextureUI()
	: AssetUI(ASSET_TYPE::TEXTURE)
{
}

TextureUI::~TextureUI()
{
}

void TextureUI::Update()
{
	Title();

	Ptr<CTexture> pTexture = (CTexture*)GetAsset().Get();

	// �̹���	
	ImVec2 uv_min = ImVec2(0.0f, 0.0f);
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);

	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 border_col = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
	ImGui::Image(pTexture->GetSRV().Get(), ImVec2(150, 150), uv_min, uv_max, tint_col, border_col);

	// �ؽ��� �̸�
	OutputAssetName();

	// �ػ�
	UINT width = pTexture->Width();
	UINT height = pTexture->Height();

	char buff[50] = {};
	sprintf_s(buff, "%d", width);

	ImGui::NewLine();
	ImGui::SameLine(60);
	ImGui::TextColored(HEADER_1, u8"�ʺ�");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(150.f);
	ImGui::InputText("##TextureWidth", buff, 50, ImGuiInputTextFlags_ReadOnly);

	ImGui::NewLine();
	ImGui::SameLine(60);
	sprintf_s(buff, "%d", height);
	ImGui::TextColored(HEADER_1, u8"����");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(150.f);
	ImGui::InputText("##TextureHeight", buff, 50, ImGuiInputTextFlags_ReadOnly);

	// �迭 ������
}