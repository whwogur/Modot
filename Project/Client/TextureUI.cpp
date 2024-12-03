#include "pch.h"
#include "TextureUI.h"

#include <Engine/CTexture.h>

TextureUI::TextureUI()
	: AssetUI(ASSET_TYPE::TEXTURE)
{
}

void TextureUI::Update()
{
	Title();

	Ptr<CTexture> pTexture = (CTexture*)GetAsset().Get();

	// 이미지	
	static float thumbnailSize = 100.0f;
	ImGui::SetNextItemWidth(100.f);
	ImGui::SliderFloat(ICON_FA_SEARCH_PLUS, &thumbnailSize, 64.0f, 512.0f, "%.1f", ImGuiSliderFlags_NoInput);
	ImGui::Image((ImTextureID)pTexture->GetSRV().Get(), { thumbnailSize ,thumbnailSize }, {0, 0}, {1, 1}, {1, 1, 1, 1}, HEADER_2);

	// 텍스쳐 이름
	OutputAssetName();

	// 해상도
	UINT width = pTexture->Width();
	UINT height = pTexture->Height();

	char buff[50] = {};
	sprintf_s(buff, "%d", width);

	ImGui::NewLine();
	ImGui::SameLine(60);
	ImGui::TextColored(HEADER_1, u8"너비");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(150.f);
	ImGui::InputText("##TextureWidth", buff, 50, ImGuiInputTextFlags_ReadOnly);

	ImGui::NewLine();
	ImGui::SameLine(60);
	sprintf_s(buff, "%d", height);
	ImGui::TextColored(HEADER_1, u8"높이");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(150.f);
	ImGui::InputText("##TextureHeight", buff, 50, ImGuiInputTextFlags_ReadOnly);

	// 배열 사이즈
}