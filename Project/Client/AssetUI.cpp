#include "pch.h"
#include "AssetUI.h"
#include "CAssetMgr.h"
#include <ImGui/imgui_internal.h>
AssetUI::AssetUI(ASSET_TYPE _Type)
	: m_Type(_Type)
{
	m_AssetIcons = CAssetMgr::GetInst()->FindAsset<CTexture>(L"AssetIcons");
	m_LogoTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"Modot_Logo");
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
	float vUV_0 = (1 / (float)ASSET_TYPE::END) * (m_Type == ASSET_TYPE::MESH ? 0 : (UINT)m_Type - 1);
	float vUV_1 = (1 / (float)ASSET_TYPE::END) * ((UINT)m_Type);
	ImGui::Image((void*)m_AssetIcons.Get()->GetSRV().Get(), { 28, 28 }, { vUV_0, 0 }, { vUV_1, 1 });
	ImGui::SameLine();
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImGui::TextColored({ 0.4f, 0.7f, 0.8f, 1.0f }, ToString(m_Type));
	if (m_Asset->IsEngineAsset())
	{
		ImGui::SameLine();
		ImGui::Image(m_LogoTex->GetSRV().Get(), { 20 ,20 });
	}
	ImGui::PopStyleVar();
	ImGui::PopID();
}

void AssetUI::OutputAssetName()
{
	Ptr<CAsset> pAsset = GetAsset();

	string Name = string(pAsset->GetKey().begin(), pAsset->GetKey().end());

	ImGui::NewLine();
	ImGui::SeparatorText(u8"정보");
	ImGui::NewLine();
	ImGui::SameLine(60);

	ImGui::TextColored(HEADER_1, u8"이름");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(150.f);
	ImGui::InputText("##AssetName", (char*)Name.c_str(), Name.length(), ImGuiInputTextFlags_ReadOnly);
}