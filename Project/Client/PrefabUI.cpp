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

		ImGui::NewLine();
		ImGui::SeparatorText(u8"프리팹 정보");
		ImGui::NewLine();

		ImGui::SameLine(60);
		ImGui::TextColored(HEADER_1, u8"경로 :");
		const wstring& wstrRelPath = pPrefab->GetRelativePath();
		if (!wstrRelPath.empty())
		{
			string strRelPath(wstrRelPath.begin(), wstrRelPath.end());
			ImGui::SameLine(100);
			ImGui::SetNextItemWidth(200);
			ImGui::InputText("##PrefabPath", (char*)strRelPath.c_str(), strRelPath.length(), ImGuiInputTextFlags_ReadOnly);
		}
		else
		{
			ImGui::SameLine(100);
			ImGui::TextColored({1.0f, 0.0f, 0.0f, 1.0f}, u8"저장되지 않은 프리팹");
			ImGui::SameLine();
			if (ImGui::Button(ICON_FA_FLOPPY_O, { 25, 25 }))
			{
				wstring objName = pPrefab->GetOriginalObject()->GetName();
				pPrefab->Save(L"prefab\\" + objName + L".prefab");
			}
			ImGui::SetItemTooltip(u8"프리팹을 저장합니다.\n원본 객체 이름 확인");
		}
	}
}