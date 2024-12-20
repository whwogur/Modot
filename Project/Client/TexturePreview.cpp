#include "pch.h"
#include "TexturePreview.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "ListUI.h"
#include "CEditorMgr.h"

#include <Engine/CAssetMgr.h>
#include "ClientStatic.h"
void TexturePreview::Update()
{
	ImGui::SetNextItemWidth(100.f);
	ImGui::DragFloat(u8"너비", &m_TexSizeX, 5.f, 50.f, 500.f, "%.1f", ImGuiSliderFlags_AlwaysClamp);

	ImGui::SameLine();
	ImGui::SetNextItemWidth(100.f);
	ImGui::DragFloat(u8"높이", &m_TexSizeY, 5.f, 50.f, 200.f, "%.1f", ImGuiSliderFlags_AlwaysClamp);

	if (m_Tex1 != nullptr)
	{
		const wstring& wstrKey = m_Tex1->GetKey();
		string strKey(wstrKey.begin(), wstrKey.end());
		if (ClientStatic::ImageButton(strKey.c_str(), (ImTextureID)m_Tex1->GetSRV().Get(), { m_TexSizeX, m_TexSizeY }, { 0, 0 }, { 1, 1 }, { 0, 0, 0, 0 }, {1, 1, 1, 1}, 0))
		{
			m_SelectedContext = &m_Tex1;

			ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
			pListUI->SetName("Texture");
			std::vector<string> vecTexNames;
			CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecTexNames);
			pListUI->AddList(vecTexNames);
			pListUI->AddDelegate(this, (TOOLFUNC_1)&TexturePreview::ChangeTexture);
			pListUI->SetActive(true);
		}
	}
	else
	{
		if (ImGui::Button(ICON_FA_PLUS "##Tex1", { m_TexSizeX, m_TexSizeY }))
		{
			m_SelectedContext = &m_Tex1;

			ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
			pListUI->SetName("Texture");
			std::vector<string> vecTexNames;
			CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecTexNames);
			pListUI->AddList(vecTexNames);
			pListUI->AddDelegate(this, (TOOLFUNC_1)&TexturePreview::ChangeTexture);
			pListUI->SetActive(true);
		}
	}

	ImGui::SameLine();
	if (m_Tex2 != nullptr)
	{
		const wstring& wstrKey = m_Tex2->GetKey();
		string strKey(wstrKey.begin(), wstrKey.end());
		if (ClientStatic::ImageButton(strKey.c_str(), (ImTextureID)m_Tex2->GetSRV().Get(), { m_TexSizeX, m_TexSizeY }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, 0))
		{
			m_SelectedContext = &m_Tex2;

			ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
			pListUI->SetName("Texture");
			std::vector<string> vecTexNames;
			CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecTexNames);
			pListUI->AddList(vecTexNames);
			pListUI->AddDelegate(this, (TOOLFUNC_1)&TexturePreview::ChangeTexture);
			pListUI->SetActive(true);
		}
	}
	else
	{
		if (ImGui::Button(ICON_FA_PLUS "##Tex2", { m_TexSizeX, m_TexSizeY }))
		{
			m_SelectedContext = &m_Tex2;

			ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
			pListUI->SetName("Texture");
			std::vector<string> vecTexNames;
			CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecTexNames);
			pListUI->AddList(vecTexNames);
			pListUI->AddDelegate(this, (TOOLFUNC_1)&TexturePreview::ChangeTexture);
			pListUI->SetActive(true);
		}
	}

	ImGui::SameLine();
	if (m_Tex3 != nullptr)
	{
		const wstring& wstrKey = m_Tex3->GetKey();
		string strKey(wstrKey.begin(), wstrKey.end());
		if (ClientStatic::ImageButton(strKey.c_str(), (ImTextureID)m_Tex3->GetSRV().Get(), { m_TexSizeX, m_TexSizeY }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, 0))
		{
			m_SelectedContext = &m_Tex3;

			ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
			pListUI->SetName("Texture");
			std::vector<string> vecTexNames;
			CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecTexNames);
			pListUI->AddList(vecTexNames);
			pListUI->AddDelegate(this, (TOOLFUNC_1)&TexturePreview::ChangeTexture);
			pListUI->SetActive(true);
		}
	}
	else
	{
		if (ImGui::Button(ICON_FA_PLUS "##Tex3", { m_TexSizeX, m_TexSizeY }))
		{
			m_SelectedContext = &m_Tex3;

			ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
			pListUI->SetName("Texture");
			std::vector<string> vecTexNames;
			CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecTexNames);
			pListUI->AddList(vecTexNames);
			pListUI->AddDelegate(this, (TOOLFUNC_1)&TexturePreview::ChangeTexture);
			pListUI->SetActive(true);
		}
	}

	ImGui::SameLine();
	if (m_Tex4 != nullptr)
	{
		const wstring& wstrKey = m_Tex4->GetKey();
		string strKey(wstrKey.begin(), wstrKey.end());
		if (ClientStatic::ImageButton(strKey.c_str(), (ImTextureID)m_Tex4->GetSRV().Get(), { m_TexSizeX, m_TexSizeY }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, 0))
		{
			m_SelectedContext = &m_Tex4;

			ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
			pListUI->SetName("Texture");
			std::vector<string> vecTexNames;
			CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecTexNames);
			pListUI->AddList(vecTexNames);
			pListUI->AddDelegate(this, (TOOLFUNC_1)&TexturePreview::ChangeTexture);
			pListUI->SetActive(true);
		}
	}
	else
	{
		if (ImGui::Button(ICON_FA_PLUS "##Tex4", { m_TexSizeX, m_TexSizeY }))
		{
			m_SelectedContext = &m_Tex4;

			ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
			pListUI->SetName("Texture");
			std::vector<string> vecTexNames;
			CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecTexNames);
			pListUI->AddList(vecTexNames);
			pListUI->AddDelegate(this, (TOOLFUNC_1)&TexturePreview::ChangeTexture);
			pListUI->SetActive(true);
		}
	}

	ImGui::NewLine();
	// Name Output
	if (m_Tex1 != nullptr)
	{
		ImGui::SameLine(m_TexSizeX / 3.f);
		const wstring& wstrName = m_Tex1->GetKey();
		string strName(wstrName.begin(), wstrName.end());
		ImGui::TextColored(HEADER_2, strName.c_str());
	}

	if (m_Tex2 != nullptr)
	{
		ImGui::SameLine((m_TexSizeX / 2.f) * 3);
		const wstring& wstrName = m_Tex2->GetKey();
		string strName(wstrName.begin(), wstrName.end());
		ImGui::TextColored(HEADER_2, strName.c_str());
	}

	if (m_Tex3 != nullptr)
	{
		ImGui::SameLine((m_TexSizeX / 2.f) * 5);
		const wstring& wstrName = m_Tex3->GetKey();
		string strName(wstrName.begin(), wstrName.end());
		ImGui::TextColored(HEADER_2, strName.c_str());
	}

	if (m_Tex4 != nullptr)
	{
		ImGui::SameLine((m_TexSizeX / 2.f) * 7);
		const wstring& wstrName = m_Tex4->GetKey();
		string strName(wstrName.begin(), wstrName.end());
		ImGui::TextColored(HEADER_2, strName.c_str());
	}
}

void TexturePreview::ChangeTexture(DWORD_PTR _ListUI)
{
	// 마지막으로 선택한 항목이 무엇인지 ListUI 를 통해서 알아냄
	ListUI* pListUI = (ListUI*)_ListUI;
	string strName = pListUI->GetSelectName();

	if ("None" == strName)
	{
		*m_SelectedContext = nullptr;
		return;
	}

	wstring strAssetName(strName.begin(), strName.end());

	Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(strAssetName);
	*m_SelectedContext = pTex;

	MD_ENGINE_ASSERT(*m_SelectedContext != nullptr, L"TexturePreview Texture 설정 오류");

}