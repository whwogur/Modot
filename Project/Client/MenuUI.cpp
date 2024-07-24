#include "pch.h"
#include "MenuUI.h"
#include "CEditorMgr.h"
#include "CTimeMgr.h"
#include "CLevelMgr.h"
#include <Engine/CAssetMgr.h>
#include <Engine/assets.h>
#include <Scripts/CScriptMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CScript.h>
#include <Engine/CLevel.h>

#include "CEditorMgr.h"
#include "Inspector.h"
#include "SpriteEditor.h"
#include "AnimationEditor.h"
#include "TilemapEditor.h"
#include "CLevelSaveLoad.h"
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
		LEVEL_STATE state = CLevelMgr::GetInst()->GetCurrentLevel()->GetState();
		string whichCamera;
		UINT FPS = CTimeMgr::GetInst()->GetFPSRecord();
		char buffer[30];
		sprintf_s(buffer, ICON_FA_BAR_CHART " FPS: %d", FPS);

		if (state == LEVEL_STATE::PLAY)
		{
			whichCamera = ICON_FA_CAMERA " MainCamera";
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.22f, 0.23f, 0.77f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.35f, 0.87f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.15f, 0.2f, 0.7f, 1.0f });
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
			ImGui::PopStyleColor(3);

			ImGui::SameLine(contentRegionAvailable - 150);
			ImGui::TextColored({ 0.45f, 0.55f, 0.88f, 1.0f }, whichCamera.c_str());
			ImGui::SameLine();
			ImGui::TextColored({ 0.45f, 0.55f, 0.88f, 1.0f }, buffer);
			ImGui::EndMainMenuBar();
		}
		else
		{
			whichCamera = ICON_FA_CAMERA " EditorCamera";
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

			ImGui::SameLine(contentRegionAvailable - 150);
			ImGui::Text(whichCamera.c_str());
			ImGui::SameLine();
			ImGui::Text(buffer);
			ImGui::EndMainMenuBar();
		}
		
	}

}


void MenuUI::Update()
{
	File();

	Level();

	AddScripts();

	Assets();
}

void MenuUI::File()
{
	ImFont* iconFont = CEditorMgr::GetInst()->GetIconFont();
	ImGui::PushFont(iconFont);
	
	if (ImGui::BeginMenu(ICON_FA_FILE " File"))
	{
		if (ImGui::MenuItem(u8"새 레벨", "Ctrl + N"))
		{
		}

		if (ImGui::MenuItem(u8"레벨 불러오기", "Ctrl + O"))
		{
			LoadLevel();
		}

		if (ImGui::MenuItem(u8"레벨 저장", "Ctrl + S"))
		{
			CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			const wstring& levelName = pLevel->GetName();
			if (!levelName.empty())
			{
				wstring strLevelPath = L"level\\" + levelName + L".lv";
				CLevelSaveLoad::SaveLevel(strLevelPath, pLevel);
			}
		}
		ImGui::SetItemTooltip(u8"현재 편집중인\n레벨을 저장합니다");

		if (ImGui::MenuItem(u8"다른 이름으로 저장", "Ctrl + Shift + S"))
		{
			SaveLevelAs();
		}

		if (ImGui::MenuItem(u8"파일 탐색기", "Ctrl + E"))
		{
			CEditorMgr::GetInst()->FindEditorUI("FileBrowser")->Toggle();
		}

		if (ImGui::MenuItem(u8"종료"))
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
		if (ImGui::MenuItem("Tilemap Editor"))
		{
			TilemapEditor* editor = static_cast<TilemapEditor*>(CEditorMgr::GetInst()->FindEditorUI("TilemapEditor"));
			editor->SetTilemap(nullptr);
			editor->Toggle();
		}

		ImGui::EndMenu();
	}
	ImGui::PopFont();
}

void MenuUI::AddScripts()
{
	ImFont* iconFont = CEditorMgr::GetInst()->GetIconFont();
	ImGui::PushFont(iconFont);

	if (ImGui::BeginMenu(ICON_FA_FILE_TEXT_O " Add Script"))
	{
		AddScript();

		ImGui::EndMenu();
	}
	ImGui::PopFont();
}

void MenuUI::Assets()
{
	if (ImGui::BeginMenu(ICON_FA_SUITCASE " Assets"))
	{
		if (ImGui::MenuItem("Create Empty Material"))
		{
			Ptr<CMaterial> pMtrl = new CMaterial;
			wstring Key = GetAssetKey(ASSET_TYPE::MATERIAL, L"Default Material");
			CAssetMgr::GetInst()->AddAsset<CMaterial>(Key, pMtrl);
			pMtrl->Save(Key);
		}

		if (ImGui::MenuItem("Sprite Editor"))
		{
			SpriteEditor* spriteEditor = static_cast<SpriteEditor*>(CEditorMgr::GetInst()->FindEditorUI("SpriteEditor"));
			spriteEditor->SetActive(true);
		}

		if (ImGui::MenuItem("Animation Editor"))
		{
			AnimationEditor* animEditor = static_cast<AnimationEditor*>(CEditorMgr::GetInst()->FindEditorUI("AnimationEditor"));
			animEditor->SetActive(true);
		}

		ImGui::EndMenu();
	}
}

void MenuUI::LoadLevel()
{
	// open a file name
	OPENFILENAME ofn = {};

	wstring strinitPath = CPathMgr::GetInst()->GetContentPath();
	strinitPath += L"level\\";

	wchar_t szFilePath[256] = {};

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFilePath;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 256;
	ofn.lpstrFilter = L"lv\0*.lv\0ALL\0*.*";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = strinitPath.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (false == GetOpenFileName(&ofn))
		return;

	path relPath = szFilePath;
	wstring rel = relPath.filename();
	CLevel* pLevel = CLevelSaveLoad::LoadLevel(L"level\\" + rel);
	ChangeLevel(pLevel, LEVEL_STATE::PLAY);
}

void MenuUI::SaveLevelAs()
{
	// open a file name
	OPENFILENAME ofn = {};

	wstring strTileFolderPath = CPathMgr::GetInst()->GetContentPath();
	strTileFolderPath += L"level\\";


	wchar_t szFilePath[256] = {};

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFilePath;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 256;
	ofn.lpstrFilter = L"lv\0*.lv\0ALL\0*.*";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = strTileFolderPath.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (false == GetSaveFileName(&ofn))
		return;

	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	path relPath = szFilePath;
	wstring rel = relPath.filename();
	rel += L".lv";
	CLevelSaveLoad::SaveLevel(L"level\\" + rel, pLevel);
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

void MenuUI::AddScript()
{
	if (ImGui::BeginMenu("Add Script"))
	{
		vector<wstring> vecScriptsName;
		CScriptMgr::GetScriptInfo(vecScriptsName);

		for (size_t i = 0; i < vecScriptsName.size(); ++i)
		{
			if (ImGui::MenuItem(string(vecScriptsName[i].begin(), vecScriptsName[i].end()).c_str()))
			{
				Inspector* pInspector = static_cast<Inspector*>(CEditorMgr::GetInst()->FindEditorUI("Inspector"));
				CGameObject* pObject = pInspector->GetTargetObject();
				
				if (nullptr != pObject)
				{
					// 오브젝트에, 선택한 스크립트를 추가해줌
					CScript* pScript = CScriptMgr::GetScript(vecScriptsName[i]);
					pObject->AddComponent(pScript);
					pInspector->SetTargetObject(pObject);
				}
			}
		}

		ImGui::EndMenu();
	}
}