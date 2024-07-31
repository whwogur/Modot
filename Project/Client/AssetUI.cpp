#include "pch.h"
#include "AssetUI.h"
#include "CAssetMgr.h"
#include <ImGui/imgui_internal.h>
AssetUI::AssetUI(ASSET_TYPE _Type)
	: m_Type(_Type)
{
	m_AssetIcons = CAssetMgr::GetInst()->Load<CTexture>(L"AssetIcons", L"texture\\AssetIcons.png");
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
	float vUV_0 = (1 / (float)ASSET_TYPE::END) * (UINT)m_Type;
	float vUV_1 = (1 / (float)ASSET_TYPE::END) * ((UINT)m_Type + 1);
	ImGui::Image((void*)m_AssetIcons.Get()->GetSRV().Get(), { 32, 32 }, { vUV_0, 0 }, { vUV_1, 1 });
	ImGui::SameLine();
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImGui::TextColored({ 0.4f, 0.7f, 0.8f, 1.0f }, ToString(m_Type));

	ImGui::PopStyleVar();
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