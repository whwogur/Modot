#include "pch.h"
#include "SE_Detail.h"
#include "CEditorMgr.h"
#include "ListUI.h"
#include "TreeUI.h"

#include "SE_AtlasView.h"
#include "CAssetMgr.h"
#include <Engine/CAssetMgr.h>

SE_Detail::SE_Detail()
{
}

SE_Detail::~SE_Detail()
{
}

void SE_Detail::Init()
{
}

void SE_Detail::Update()
{
	ImGui::TextColored(HEADER_1, u8"아틀라스 정보");
	Atlas();
	AtlasInfo();
	
	static char buf[64] = {};
	static int BGSize[2] = { m_BGSizeX, m_BGSizeY };
	ImGui::SetNextItemWidth(200);
	if (ImGui::InputInt2(ICON_FA_OBJECT_GROUP" BGSize", BGSize, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		m_BGSizeX = BGSize[0];
		m_BGSizeY = BGSize[1];
	}

	if (m_BGSizeX > 0 && m_BGSizeY > 0)
	{
		ImGui::NewLine();
		ImGui::TextColored(HEADER_1, u8"선택 정보");
		ImGui::InputText("##spriteTInput", buf, sizeof(buf));
		ImGui::SameLine();
		if (ImGui::Button(ICON_FA_FLOPPY_O, { 25, 25 }))
		{
			Ptr<CSprite> pSprite = new CSprite;
			pSprite->Create(m_AtlasTex, m_LeftTop, m_Slice);
			pSprite->SetBackground({ m_BGSizeX, m_BGSizeY });

			string strRelPath(buf);
			wstring wstrRelPath(strRelPath.begin(), strRelPath.end());
			wstrRelPath += L".sprite";

			pSprite->Save(L"sprite\\" + wstrRelPath);
		}
		ImGui::SetItemTooltip(u8"스프라이트를 저장합니다\n이름을 다시 한번 확인해주세요\n(반드시 이름만작성)");

		float vLT[2] = { m_LeftTop.x, m_LeftTop.y };
		float vRB[2] = { m_Slice.x, m_Slice.y };

		ImGui::InputFloat2("LT##leftTop", vLT, "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::InputFloat2("RB##Slice", vRB, "%.3f", ImGuiInputTextFlags_ReadOnly);
	}
	else
	{
		ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, u8"BG Size 설정 필요");
	}
}

void SE_Detail::Atlas()
{
	string TexName;

	if (nullptr != m_AtlasTex)
		TexName = string(m_AtlasTex->GetKey().begin(), m_AtlasTex->GetKey().end());

	ImGui::Text("Atlas Texture");
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(150.f);
	const map<wstring, Ptr<CAsset>>& mapTex = CAssetMgr::GetInst()->GetAssets(ASSET_TYPE::TEXTURE);

	if (ImGui::BeginCombo(ICON_FA_PICTURE_O "##AtlasTexCombo", TexName.c_str()))
	{
		for (const auto& texture : mapTex)
		{
			string strName(texture.first.begin(), texture.first.end());
			if (ImGui::Selectable(strName.c_str()))
			{
				SetAtlasTex((CTexture*)texture.second.Get());
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
		if (payload)
		{
			TreeNode** ppNode = (TreeNode**)payload->Data;
			TreeNode* pNode = *ppNode;

			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (ASSET_TYPE::TEXTURE == pAsset->GetAssetType())
			{
				SetAtlasTex((CTexture*)pAsset.Get());
			}
		}

		ImGui::EndDragDropTarget();
	}
}

void SE_Detail::AtlasInfo()
{
	UINT width = 0;
	UINT height = 0;

	if (nullptr != m_AtlasTex)
	{
		width = m_AtlasTex->Width();
		height = m_AtlasTex->Height();
	}

	char buff[50] = {};
	sprintf_s(buff, "%d", width);

	ImGui::Text("Width");
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(150);
	ImGui::InputText("##TextureWidth", buff, 50, ImGuiInputTextFlags_ReadOnly);

	sprintf_s(buff, "%d", height);
	ImGui::Text("Height");
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(150);
	ImGui::InputText("##TextureHeight", buff, 50, ImGuiInputTextFlags_ReadOnly);
}

void SE_Detail::SetAtlasTex(Ptr<CTexture> _Tex)
{
	m_AtlasTex = _Tex;
	GetAtlasView()->SetAtlasTex(m_AtlasTex);
}

void SE_Detail::SelectTexture(DWORD_PTR _ListUI)
{
	ListUI* pListUI = (ListUI*)_ListUI;
	string strName = pListUI->GetSelectName();

	if (strName == "None")
	{
		m_AtlasTex = nullptr;
		return;
	}

	wstring strAssetName = wstring(strName.begin(), strName.end());

	Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(strAssetName);

	assert(pTex.Get());

	SetAtlasTex(pTex);
}