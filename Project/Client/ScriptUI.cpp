#include "pch.h"
#include "ScriptUI.h"

#include <Scripts/CScriptMgr.h>
#include <Engine/CScript.h>
#include <Engine/CAssetMgr.h>
#include "ParamUI.h"
#include "ListUI.h"
ScriptUI::ScriptUI()
	: ComponentUI(COMPONENT_TYPE::SCRIPT)
	, m_Script(nullptr)
{
	m_IconTexture = CAssetMgr::GetInst()->FindAsset<CTexture>(L"ScriptIcon");
}

void ScriptUI::Update()
{
	ImGui::Image((void*)m_IconTexture.Get()->GetSRV().Get(), { ICON_SIZE, ICON_SIZE });
	ImGui::SameLine(ICON_SIZE + 3);
	wstring strScriptName = CScriptMgr::GetScriptName(m_Script);

	ImGui::TextColored(HEADER_3, string(strScriptName.begin(), strScriptName.end()).c_str());
	// Script ���� �����ų �����͸� �����ش�.
	const std::vector<tScriptParam>& vecParam = m_Script->GetScriptParam();

	for (size_t i = 0; i < vecParam.size(); ++i)
	{
		switch (vecParam[i].Type)
		{
		case SCRIPT_PARAM::INT:
			ParamUI::InputInt((int*)vecParam[i].pData, vecParam[i].Desc, vecParam[i].Tooltip);
			break;
		case SCRIPT_PARAM::FLOAT:
			ParamUI::InputFloat((float*)vecParam[i].pData, vecParam[i].Desc, vecParam[i].Tooltip);
			break;
		case SCRIPT_PARAM::VEC2:
			ParamUI::InputFloat((float*)vecParam[i].pData, vecParam[i].Desc, vecParam[i].Tooltip);
			break;
		case SCRIPT_PARAM::VEC3:
		case SCRIPT_PARAM::VEC4:
			ParamUI::InputVec4((Vec4*)vecParam[i].pData, vecParam[i].Desc, vecParam[i].Tooltip);
			break;
		case SCRIPT_PARAM::TEXTURE:
		{
			Ptr<CTexture>& pTex = *((Ptr<CTexture>*)vecParam[i].pData);
			ParamUI::InputTexture(pTex, vecParam[i].Desc);
			break;
		}
		case SCRIPT_PARAM::PREFAB:
		{
			Ptr<CPrefab>& pPrefab = *((Ptr<CPrefab>*)vecParam[i].pData);

			if (ParamUI::InputPrefab(pPrefab, vecParam[i].Desc, this, (DELEGATE_1)&ScriptUI::SelectPrefab))
			{
				m_SelectedPrefab = &pPrefab;
			}
			break;
		}
		case SCRIPT_PARAM::SPRITE:
		{
			Ptr<CSprite>& pSprite = *((Ptr<CSprite>*)vecParam[i].pData);
			ParamUI::InputSprite(pSprite, vecParam[i].Desc);
			break;
		}

		case SCRIPT_PARAM::BOOLEAN_TOGGLE:
		{
			ParamUI::ToggleBool((bool*)vecParam[i].pData, vecParam[i].Desc, vecParam[i].Tooltip);
			break;
		}

		case SCRIPT_PARAM::BOOLEAN_CHECKBOX:
		{
			ParamUI::CheckboxBool((bool*)vecParam[i].pData, vecParam[i].Desc, vecParam[i].Tooltip);
			break;
		}
		case SCRIPT_PARAM::COLOR:
		{
			ParamUI::ColorVec4((Vec4*)vecParam[i].pData, vecParam[i].Desc, vecParam[i].Tooltip);
			break;
		}
		case SCRIPT_PARAM::CHAR:
		{	
			ImGui::TextColored(HEADER_2, vecParam[i].Desc.c_str());
			ImGui::InputText("##CharParam", (char*)vecParam[i].pData, 255, ImGuiInputTextFlags_EnterReturnsTrue);
			if (!vecParam[i].Tooltip.empty())
				ImGui::SetItemTooltip(vecParam[i].Tooltip.c_str());
			break;
		}
		case SCRIPT_PARAM::WSTRING:
		{
			std::string previousText = ToString(*(wstring*)vecParam[i].pData);
			// ���� ������ ���� ���� ũ���� �迭�� ���
			static char buffer[1024];

			strcpy_s(buffer, previousText.c_str());

			// ImGui::InputText ���
			if (ImGui::InputText(u8"�ؽ�Ʈ", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
			{
				std::string newText(buffer);
				// buffer�� ����Ǿ����� ������Ʈ
				if (previousText != newText)
				{
					if (newText.size() > sizeof(buffer) - 1)
					{
						MessageBox(nullptr, L"�Է� ũ�� ����", L"�ʹ� ���� �Է�����", MB_OK);
						assert(nullptr);
					}
					else
					{
						previousText = newText;
						(*(wstring*)vecParam[i].pData) = ToWstring(previousText);
					}
				}
			}
			break;
		}
		}
	}

}

void ScriptUI::SetTargetScript(CScript* _Script)
{
	m_Script = _Script;

	if (nullptr != m_Script)
		SetActive(true);
	else
		SetActive(false);
}


void ScriptUI::SelectPrefab(DWORD_PTR _ListUI)
{
	ListUI* pListUI = (ListUI*)_ListUI;
	string strName = pListUI->GetSelectName();

	if ("None" == strName)
	{
		*m_SelectedPrefab = nullptr;
		return;
	}

	wstring strAssetName = wstring(strName.begin(), strName.end());
	Ptr<CPrefab> pPrefab = CAssetMgr::GetInst()->FindAsset<CPrefab>(strAssetName);

	assert(pPrefab.Get());

	*m_SelectedPrefab = pPrefab;
}