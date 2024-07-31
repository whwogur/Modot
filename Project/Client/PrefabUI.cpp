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
		ImGui::NewLine();
		ImGui::SameLine(200); 
		if (ImGui::Button(ICON_FA_FLOPPY_O, { 25, 25 }))
		{
			wstring objName = pPrefab->GetOriginalObject()->GetName();
			pPrefab->Save(L"prefab\\" + objName + L".prefab");
		}
		ImGui::SetItemTooltip(u8"프리팹을 저장합니다.\n원본 객체 이름 확인");
		ImGui::NewLine();
		ImGui::TextColored(HEADER_1, u8"원본 :");
		ImGui::SameLine(100);
		ImGui::SetNextItemWidth(100);
		ImGui::InputText("##OriginalObjName", (char*)strName.c_str(), strName.length(), ImGuiInputTextFlags_ReadOnly);
	}
}