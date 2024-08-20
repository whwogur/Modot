#include "pch.h"
#include "ParamUI.h"

#include <Engine/CAssetMgr.h>
#include "ImGui/imgui.h"

#include "CEditorMgr.h"
#include "ListUI.h"
#include <Engine/CSprite.h>
#include <ImGui/imgui_internal.h>

UINT ParamUI::g_ID = 0;

bool ParamUI::InputInt(int* _Data, const string& _Desc, const string& _Tooltip)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(150);

	char szID[255] = {};
	sprintf_s(szID, 255, "##Input%d", g_ID++);

	ImGui::SetNextItemWidth(120);
	if (ImGui::InputInt(szID, _Data, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		return true;
	}
	if (!_Tooltip.empty())
		ImGui::SetItemTooltip(_Tooltip.c_str());

	return false;
}

bool ParamUI::DragInt(int* _Data, float _Step, const string& _Desc, const string& _Tooltip)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(150);

	char szID[255] = {};
	sprintf_s(szID, 255, "##Input%d", g_ID++);

	if (ImGui::DragInt(szID, _Data, _Step))
	{
		return true;
	}
	if (!_Tooltip.empty())
		ImGui::SetItemTooltip(_Tooltip.c_str());
	return false;
}

void ParamUI::ToggleBool(bool* _Data, const string& _Desc, const string& _Tooltip)
{
	ImGui::SeparatorText(_Desc.c_str());
	ImGui::NewLine();
	ImGui::SameLine(300);
	ImGui::SetNextItemWidth(150);

	char szID[255] = {};
	sprintf_s(szID, 255, "##Input%d", g_ID++);
	
	ImVec4* colors = ImGui::GetStyle().Colors;
	ImVec2 p = ImGui::GetCursorScreenPos();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	float height = ImGui::GetFrameHeight();
	float width = height * 1.55f;
	float radius = height * 0.50f;

	ImGui::InvisibleButton(szID, ImVec2(width, height));
	if (ImGui::IsItemClicked()) *_Data = !*_Data;
	ImGuiContext& gg = *GImGui;
	float ANIM_SPEED = 0.085f;
	if (gg.LastActiveId == gg.CurrentWindow->GetID(szID))// && g.LastActiveIdTimer < ANIM_SPEED)
		float t_anim = ImSaturate(gg.LastActiveIdTimer / ANIM_SPEED);
	if (ImGui::IsItemHovered())
		draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), ImGui::GetColorU32(*_Data ? ImVec4(0.f, 0.77f, 0.23f, 1.0f) : ImVec4(0.78f, 0.78f, 0.78f, 1.0f)), height * 0.5f);
	else
		draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), ImGui::GetColorU32(*_Data ? ImVec4(0.f, 0.88f, 0.35f, 1.0f) : ImVec4(0.85f, 0.85f, 0.85f, 1.0f)), height * 0.50f);
	draw_list->AddCircleFilled(ImVec2(p.x + radius + (*_Data ? 1 : 0) * (width - radius * 2.0f), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));
	if (!_Tooltip.empty())
		ImGui::SetItemTooltip(_Tooltip.c_str());
}

void ParamUI::CheckboxBool(bool* _Data, const string& _Desc, const string& _Tooltip)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(150);

	char szID[255] = {};
	sprintf_s(szID, 255, "##Input%d", g_ID++);
	ImGui::Checkbox(szID, _Data);
	if (!_Tooltip.empty())
		ImGui::SetItemTooltip(_Tooltip.c_str());
}

bool ParamUI::InputFloat(float* _Data, const string& _Desc, const string& _Tooltip)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(150);

	char szID[255] = {};
	sprintf_s(szID, 255, "##Input%d", g_ID++);

	if (ImGui::InputFloat(szID, _Data, 0))
	{
		return true;
	}
	if (!_Tooltip.empty())
		ImGui::SetItemTooltip(_Tooltip.c_str());
	return false;
}

bool ParamUI::DragFloat(float* _Data, float _Step, const string& _Desc, const string& _Tooltip)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(150);

	char szID[255] = {};
	sprintf_s(szID, 255, "##Input%d", g_ID++);

	if (ImGui::InputFloat(szID, _Data, _Step))
	{
		return true;
	}
	if (!_Tooltip.empty())
		ImGui::SetItemTooltip(_Tooltip.c_str());
	return false;
}

bool ParamUI::InputVec2(Vec2* _Data, const string& _Desc, const string& _Tooltip)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine(120);

	char szID[255] = {};
	sprintf_s(szID, 255, "##Input%d", g_ID++);

	if (ImGui::InputFloat2(szID, *_Data, 0))
	{
		return true;
	}
	if (!_Tooltip.empty())
		ImGui::SetItemTooltip(_Tooltip.c_str());
	return false;
}

bool ParamUI::DragVec2(Vec2* _Data, float _Step, const string& _Desc, const string& _Tooltip)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine(120);

	char szID[255] = {};
	sprintf_s(szID, 255, "##Input%d", g_ID++);

	if (ImGui::DragFloat2(szID, *_Data, _Step))
	{
		return true;
	}
	if (!_Tooltip.empty())
		ImGui::SetItemTooltip(_Tooltip.c_str());
	return false;
}

bool ParamUI::InputVec4(Vec4* _Data, const string& _Desc, const string& _Tooltip)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine(120);

	char szID[255] = {};
	sprintf_s(szID, 255, "##Input%d", g_ID++);

	if (ImGui::InputFloat4(szID, *_Data, 0))
	{
		return true;
	}
	if (!_Tooltip.empty())
		ImGui::SetItemTooltip(_Tooltip.c_str());
	return false;
}

bool ParamUI::DragVec4(Vec4* _Data, float _Step, const string& _Desc, const string& _Tooltip)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine(120);

	char szID[255] = {};
	sprintf_s(szID, 255, "##Input%d", g_ID++);

	if (ImGui::DragFloat4(szID, *_Data, _Step))
	{
		return true;
	}
	if (!_Tooltip.empty())
		ImGui::SetItemTooltip(_Tooltip.c_str());
	return false;
}

bool ParamUI::ColorVec4(Vec4* _Data, const string& _Desc, const string& _Tooltip)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine(120);

	char szID[255] = {};
	sprintf_s(szID, 255, "##Input%d", g_ID++);

	if (ImGui::ColorEdit4(szID, *_Data, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_PickerHueWheel))
	{
		return true;
	}
	if (!_Tooltip.empty())
		ImGui::SetItemTooltip(_Tooltip.c_str());
	return false;
}

#include "TreeUI.h"
bool ParamUI::InputTexture(Ptr<CTexture>& _CurTex, const string& _Desc
	, EditorUI* _Inst, DELEGATE_1 _MemFunc)
{
	Ptr<CTexture> CurTex = _CurTex;

	ImGui::TextColored({0.2f, 0.55f, 0.78f, 1.0f}, _Desc.c_str());
	ImGui::SameLine(120);

	// 이미지
	ImVec2 uv_min = ImVec2(0.0f, 0.0f);
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);

	ImTextureID TexID = nullptr;
	if (nullptr != CurTex)
		TexID = CurTex->GetSRV().Get();

	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 border_col = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
	ImGui::Image(TexID, ImVec2(150, 150), uv_min, uv_max, tint_col, border_col);

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
				_CurTex = ((CTexture*)pAsset.Get());
			}
		}

		ImGui::EndDragDropTarget();
	}

	// DragDrop 으로 원본 텍스쳐가 바뀐경우
	if (CurTex != _CurTex)
		return true;


	// List Button
	if (nullptr == _Inst && nullptr == _MemFunc)
	{
		return false;
	}

	char szID[255] = {};
	sprintf_s(szID, 255, ICON_FA_PENCIL_SQUARE_O "##InputBtn%d", g_ID++);

	ImGui::SameLine();
	if (ImGui::Button(szID, ImVec2(27, 27)))
	{
		ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
		pListUI->SetName("Texture");
		vector<string> vecTexNames;
		CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecTexNames);
		pListUI->AddList(vecTexNames);
		pListUI->AddDelegate(_Inst, (DELEGATE_1)_MemFunc);
		pListUI->SetActive(true);

		return true;
	}

	return false;
}

bool ParamUI::InputSprite(Ptr<CSprite>& _CurSprite, const string& _Desc, EditorUI* _Inst, DELEGATE_1 _MemFunc)
{
	Ptr<CSprite> CurSprite = _CurSprite;
	Ptr<CTexture> atlasTex = CurSprite->GetAtlasTexture();

	ImGui::TextColored(HEADER_1, _Desc.c_str());
	ImGui::SameLine(120);

	Vec2 ltUV = CurSprite->GetLeftTopUV();
	Vec2 rbUV = CurSprite->GetLeftTopUV() + CurSprite->GetBackgroundUV();

	ImVec2 uv_min = ImVec2(ltUV.x, ltUV.y);
	ImVec2 uv_max = ImVec2(rbUV.x, rbUV.y);

	ImTextureID TexID = nullptr;
	if (nullptr != atlasTex)
		TexID = atlasTex->GetSRV().Get();

	ImGui::Image(TexID, ImVec2(150, 150), uv_min, uv_max);

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
		if (payload)
		{
			TreeNode** ppNode = (TreeNode**)payload->Data;
			TreeNode* pNode = *ppNode;

			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (ASSET_TYPE::SPRITE == pAsset->GetAssetType())
			{
				_CurSprite = ((CSprite*)pAsset.Get());
			}
		}

		ImGui::EndDragDropTarget();
	}

	// DragDrop 으로 원본 텍스쳐가 바뀐경우
	if (CurSprite != _CurSprite)
		return true;


	// List Button
	if (nullptr == _Inst && nullptr == _MemFunc)
	{
		return false;
	}

	char szID[255] = {};
	sprintf_s(szID, 255, ICON_FA_PENCIL_SQUARE_O "##InputBtn%d", g_ID++);

	ImGui::SameLine();
	if (ImGui::Button(szID, ImVec2(27, 27)))
	{
		ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
		pListUI->SetName("Sprites");
		vector<string> vecTexNames;
		CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::SPRITE, vecTexNames);
		pListUI->AddList(vecTexNames);
		pListUI->AddDelegate(_Inst, (DELEGATE_1)_MemFunc);
		pListUI->SetActive(true);

		return true;
	}

	return false;
}

bool ParamUI::InputPrefab(Ptr<CPrefab>& _CurPrefab, const string& _Desc, EditorUI* _Inst, DELEGATE_1 _MemFunc)
{
	Ptr<CPrefab> CurPrefab = _CurPrefab;

	ImGui::Text(_Desc.c_str());
	ImGui::SameLine(120);

	string PrefabName;

	if (CurPrefab.Get())
		PrefabName = string(CurPrefab->GetKey().begin(), CurPrefab->GetKey().end());

	char szID[255] = {};
	sprintf_s(szID, 255, "##PrefabKey%d", g_ID++);

	ImGui::SetNextItemWidth(150.f);
	ImGui::InputText(szID, (char*)PrefabName.c_str(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
		if (payload)
		{
			TreeNode** ppNode = (TreeNode**)payload->Data;
			TreeNode* pNode = *ppNode;

			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (ASSET_TYPE::PREFAB == pAsset->GetAssetType())
			{
				_CurPrefab = ((CPrefab*)pAsset.Get());
			}
		}

		ImGui::EndDragDropTarget();
	}

	// DragDrop 으로 원본 텍스쳐가 바뀐경우
	if (CurPrefab != _CurPrefab)
		return true;


	// List Button
	if (nullptr == _Inst && nullptr == _MemFunc)
	{
		return false;
	}

	sprintf_s(szID, 255, "##InputBtn%d", g_ID++);

	ImGui::SameLine();
	if (ImGui::Button(szID, ImVec2(18.f, 18.f)))
	{
		ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
		pListUI->SetName("Prefab");
		vector<string> vecPrefabName;
		CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::PREFAB, vecPrefabName);
		pListUI->AddList(vecPrefabName);
		pListUI->AddDelegate(_Inst, (DELEGATE_1)_MemFunc);
		pListUI->SetActive(true);

		return true;
	}

	return false;
}