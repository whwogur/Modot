#include "pch.h"
#include "TexturePreview.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "ListUI.h"
#include "CEditorMgr.h"

#include <Engine/CAssetMgr.h>

void TexturePreview::Update()
{
	ImGui::SetNextItemWidth(100.f);
	ImGui::DragFloat(u8"너비", &m_TexSizeX, 5.f, 50.f, 500.f, "%.1f", ImGuiSliderFlags_AlwaysClamp);

	ImGui::SameLine();
	ImGui::SetNextItemWidth(100.f);
	ImGui::DragFloat(u8"높이", &m_TexSizeY, 5.f, 50.f, 200.f, "%.1f", ImGuiSliderFlags_AlwaysClamp);

	if (m_Tex1 != nullptr)
	{
		if (ImGui::ImageButton(m_Tex1->GetSRV().Get(), { m_TexSizeX, m_TexSizeY }, { 0, 0 }, { 1, 1 }))
		{
			m_SelectedContext = &m_Tex1;

			ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
			pListUI->SetName("Texture");
			std::vector<string> vecTexNames;
			CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecTexNames);
			pListUI->AddList(vecTexNames);
			pListUI->AddDelegate(this, (DELEGATE_1)&TexturePreview::ChangeTexture);
			pListUI->SetActive(true);
		}
	}
	else
	{
		if (ImGui::Button(ICON_FA_PLUS, { m_TexSizeX, m_TexSizeY }))
		{
			m_SelectedContext = &m_Tex1;

			ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
			pListUI->SetName("Texture");
			std::vector<string> vecTexNames;
			CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecTexNames);
			pListUI->AddList(vecTexNames);
			pListUI->AddDelegate(this, (DELEGATE_1)&TexturePreview::ChangeTexture);
			pListUI->SetActive(true);
		}
	}

	ImGui::SameLine();
	if (m_Tex2 != nullptr)
	{
		if (ImGui::ImageButton(m_Tex2->GetSRV().Get(), { m_TexSizeX, m_TexSizeY }, { 0, 0 }, { 1, 1 }))
		{
			m_SelectedContext = &m_Tex2;

			ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
			pListUI->SetName("Texture");
			std::vector<string> vecTexNames;
			CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecTexNames);
			pListUI->AddList(vecTexNames);
			pListUI->AddDelegate(this, (DELEGATE_1)&TexturePreview::ChangeTexture);
			pListUI->SetActive(true);
		}
	}
	else
	{
		if (ImGui::Button(ICON_FA_PLUS, { m_TexSizeX, m_TexSizeY }))
		{
			m_SelectedContext = &m_Tex2;

			ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
			pListUI->SetName("Texture");
			std::vector<string> vecTexNames;
			CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecTexNames);
			pListUI->AddList(vecTexNames);
			pListUI->AddDelegate(this, (DELEGATE_1)&TexturePreview::ChangeTexture);
			pListUI->SetActive(true);
		}
	}

	ImGui::SameLine();
	if (m_Tex3 != nullptr)
	{
		if (ImGui::ImageButton(m_Tex3->GetSRV().Get(), { m_TexSizeX, m_TexSizeY }, { 0, 0 }, { 1, 1 }))
		{
			m_SelectedContext = &m_Tex3;

			ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
			pListUI->SetName("Texture");
			std::vector<string> vecTexNames;
			CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecTexNames);
			pListUI->AddList(vecTexNames);
			pListUI->AddDelegate(this, (DELEGATE_1)&TexturePreview::ChangeTexture);
			pListUI->SetActive(true);
		}
	}
	else
	{
		if (ImGui::Button(ICON_FA_PLUS, { m_TexSizeX, m_TexSizeY }))
		{
			m_SelectedContext = &m_Tex3;

			ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
			pListUI->SetName("Texture");
			std::vector<string> vecTexNames;
			CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecTexNames);
			pListUI->AddList(vecTexNames);
			pListUI->AddDelegate(this, (DELEGATE_1)&TexturePreview::ChangeTexture);
			pListUI->SetActive(true);
		}
	}

	ImGui::SameLine();
	if (m_Tex4 != nullptr)
	{
		if (ImGui::ImageButton(m_Tex4->GetSRV().Get(), { m_TexSizeX, m_TexSizeY }, { 0, 0 }, { 1, 1 }))
		{
			m_SelectedContext = &m_Tex4;

			ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
			pListUI->SetName("Texture");
			std::vector<string> vecTexNames;
			CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecTexNames);
			pListUI->AddList(vecTexNames);
			pListUI->AddDelegate(this, (DELEGATE_1)&TexturePreview::ChangeTexture);
			pListUI->SetActive(true);
		}
	}
	else
	{
		if (ImGui::Button(ICON_FA_PLUS, { m_TexSizeX, m_TexSizeY }))
		{
			m_SelectedContext = &m_Tex4;

			ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
			pListUI->SetName("Texture");
			std::vector<string> vecTexNames;
			CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecTexNames);
			pListUI->AddList(vecTexNames);
			pListUI->AddDelegate(this, (DELEGATE_1)&TexturePreview::ChangeTexture);
			pListUI->SetActive(true);
		}
	}

}

void TexturePreview::ChangeTexture()
{
	// 마지막으로 선택한 항목이 무엇인지 ListUI 를 통해서 알아냄
	ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
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