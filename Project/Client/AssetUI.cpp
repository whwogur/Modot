﻿#include "pch.h"
#include "AssetUI.h"

AssetUI::AssetUI(ASSET_TYPE _Type)
	: m_Type(_Type)
{
}

AssetUI::~AssetUI()
{
}

void AssetUI::SetAsset(Ptr<CAsset> _Asset)
{
	m_Asset = nullptr;

	if (nullptr == _Asset || m_Type != _Asset->GetAssetType())
	{
		SetActive(false);
	}
	else
	{
		m_Asset = _Asset;
		SetActive(true);
	}
}

void AssetUI::Title()
{
	ImGui::PushID((int)m_Type);

	ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(38, 98, 241, 255));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(38, 98, 241, 255));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(38, 98, 241, 255));

	ImGui::Button(ToString(m_Type));

	ImGui::PopStyleColor(3);
	ImGui::PopID();
}

void AssetUI::OutputAssetName()
{
	Ptr<CAsset> pAsset = GetAsset();

	string Name = string(pAsset->GetKey().begin(), pAsset->GetKey().end());

	ImGui::Text("Name");
	ImGui::SameLine(100);
	ImGui::InputText("##AssetName", (char*)Name.c_str(), Name.length(), ImGuiInputTextFlags_ReadOnly);
}