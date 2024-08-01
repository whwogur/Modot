#include "pch.h"
#include "PrefabUI.h"

#include <Engine/CPrefab.h>
#include <Engine/CGameObject.h>
PrefabUI::PrefabUI()
	: AssetUI(ASSET_TYPE::PREFAB)
{
}

void PrefabUI::Update()
{
	Title();
	
	if (GetAsset() != nullptr)
	{
		Ptr<CPrefab> pPrefab = (CPrefab*)GetAsset().Get();

		CGameObject* originalObj = pPrefab->GetOriginalObject();
		const wstring& originalObjName = originalObj->GetName();
		string strName(originalObjName.begin(), originalObjName.end());
		
		ImGui::TextColored(HEADER_1, u8"���� :");
		ImGui::SameLine(100);
		ImGui::SetNextItemWidth(100);
		ImGui::InputText("##OriginalObjName", (char*)strName.c_str(), strName.length(), ImGuiInputTextFlags_ReadOnly);

		ImGui::TextColored(HEADER_1, u8"��� :");
		wstring wstrRelPath = pPrefab->GetRelativePath();
		if (!wstrRelPath.empty())
		{
			string strRelPath(wstrRelPath.begin(), wstrRelPath.end());
			ImGui::SameLine(100);
			ImGui::SetNextItemWidth(200);
			ImGui::InputText("##PrefabPath", (char*)strRelPath.c_str(), strRelPath.length(), ImGuiInputTextFlags_ReadOnly);
		}
		else
		{
			ImGui::TextColored({1.0f, 0.0f, 0.0f, 1.0f}, u8"������� ���� ������");
			ImGui::SameLine();
			if (ImGui::Button(ICON_FA_FLOPPY_O, { 25, 25 }))
			{
				wstring objName = pPrefab->GetOriginalObject()->GetName();
				pPrefab->Save(L"prefab\\" + objName + L".prefab");
			}
			ImGui::SetItemTooltip(u8"�������� �����մϴ�.\n���� ��ü �̸� Ȯ��");
		}
	}
}