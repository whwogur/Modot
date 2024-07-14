#include "pch.h"
#include "MenuUI.h"
#include "CEditorMgr.h"
#include "CTimeMgr.h"
#include <Engine/CAssetMgr.h>
#include <Engine/assets.h>
MenuUI::MenuUI()
{
}

MenuUI::~MenuUI()
{
}

void MenuUI::Tick()
{
	if (!IsActive())
		return;

	if (ImGui::BeginMainMenuBar())
	{
		Ptr<CTexture> LogoTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"Modot_Logo");
		ImGui::Image(LogoTex->GetSRV().Get(), { 28 ,28 });

		Update();

		float contentRegionAvailable = ImGui::GetContentRegionAvail().x + 250.0f;

		ImGui::SameLine(contentRegionAvailable / 2 + 32);
		if (ImGui::Button(ICON_FA_PLAY, { 32, 25 }))
		{
			ChangeLevelState(LEVEL_STATE::PLAY);
		}
		ImGui::SameLine(contentRegionAvailable / 2 + 70);
		if (ImGui::Button(ICON_FA_PAUSE, { 32, 25 }))
		{
			ChangeLevelState(LEVEL_STATE::PAUSE);
		}

		UINT FPS = CTimeMgr::GetInst()->GetFPSRecord();
		char buffer[30];
		sprintf_s(buffer, ICON_FA_BAR_CHART " FPS: %d", FPS);
		
		ImGui::SameLine(contentRegionAvailable);
		ImGui::TextColored({ 0.57f, 0.95f, 0.88f, 1.0f }, buffer);
		ImGui::EndMainMenuBar();
	}

}


void MenuUI::Update()
{
	File();

	Level();

	GameObject();

	Assets();
}

void MenuUI::File()
{
	ImFont* iconFont = CEditorMgr::GetInst()->GetIconFont();
	ImGui::PushFont(iconFont);
	
	if (ImGui::BeginMenu(ICON_FA_FILE " File"))
	{
		if (ImGui::MenuItem(u8"�� ����", "Ctrl + N"))
		{
		}

		if (ImGui::MenuItem(u8"����", "Ctrl + O"))
		{
		}

		if (ImGui::MenuItem(u8"�ٸ� �̸����� ����", "Ctrl + Shift + S"))
		{
		}

		if (ImGui::MenuItem(u8"���� Ž����", "Ctrl + Shift + S"))
		{
			CEditorMgr::GetInst()->FindEditorUI("FileBrowser")->Toggle();
		}

		if (ImGui::MenuItem(u8"�ݱ�"))
		{
			PostQuitMessage(0);
		}
		ImGui::EndMenu();
	}
	ImGui::PopFont();
}

void MenuUI::Level()
{
	ImFont* iconFont = CEditorMgr::GetInst()->GetIconFont();
	ImGui::PushFont(iconFont);
	if (ImGui::BeginMenu(ICON_FA_FILE_VIDEO_O " Level"))
	{
		if (ImGui::MenuItem("Play"))
		{

		}


		if (ImGui::MenuItem("Pause"))
		{

		}

		if (ImGui::MenuItem("Stop"))
		{

		}

		ImGui::EndMenu();
	}
	ImGui::PopFont();
}

void MenuUI::GameObject()
{
	ImFont* iconFont = CEditorMgr::GetInst()->GetIconFont();
	ImGui::PushFont(iconFont);

	if (ImGui::BeginMenu(ICON_FA_OBJECT_GROUP " GameObject"))
	{
		if (ImGui::MenuItem("Create Empty Object"))
		{

		}

		if (ImGui::BeginMenu("Add Component"))
		{
			ImGui::MenuItem("MeshRender");
			ImGui::MenuItem("Collider2D");
			ImGui::MenuItem("Camera");

			ImGui::EndMenu();
		}

		ImGui::EndMenu();
	}
	ImGui::PopFont();
}

void MenuUI::Assets()
{
	if (ImGui::BeginMenu(ICON_FA_PICTURE_O " Assets"))
	{
		if (ImGui::MenuItem("Create Empty Material"))
		{
			Ptr<CMaterial> pMtrl = new CMaterial;
			wstring Key = GetAssetKey(ASSET_TYPE::MATERIAL, L"Default Material");
			CAssetMgr::GetInst()->AddAsset<CMaterial>(Key, pMtrl);
			pMtrl->Save(Key);
		}

		ImGui::EndMenu();
	}
}

wstring MenuUI::GetAssetKey(ASSET_TYPE _Type, const wstring& _KeyFormat)
{
	wstring Key;

	switch (_Type)
	{
	case ASSET_TYPE::MATERIAL:
	{
		Key = wstring(L"material\\") + _KeyFormat + L"_%d.mtrl";
	}
	break;
	case ASSET_TYPE::PREFAB:
	{
		Key = wstring(L"prefab\\") + _KeyFormat + L"_%d.pref";
	}
	break;
	case ASSET_TYPE::SPRITE:
	{
		Key = wstring(L"sprite\\") + _KeyFormat + L"_%d.sprite";
	}
	break;
	case ASSET_TYPE::ANIMATION:
	{
		Key = wstring(L"animation\\") + _KeyFormat + L"_%d.anim";
	}
	break;
	default:
		assert(nullptr);
		break;
	}

	wchar_t szKey[255] = {};
	wstring FilePath = CPathMgr::GetInst()->GetContentPath();

	for (UINT i = 0; i < 0xffffffff; ++i)
	{
		swprintf_s(szKey, 255, Key.c_str(), i);

		if (false == std::filesystem::exists(FilePath + szKey))
		{
			break;
		}
	}

	return szKey;
}