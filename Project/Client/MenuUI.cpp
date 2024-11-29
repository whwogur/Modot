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
#include <Engine/CRenderMgr.h>

#include "CEditorMgr.h"
#include "Inspector.h"
#include "SpriteEditor.h"
#include "Animation2DEditor.h"
#include "Animation3DEditor.h"
#include "TilemapEditor.h"
#include "CLevelSaveLoad.h"
#include "MaterialUI.h"
#include "CollisionCheck.h"
#include "CPlayerManager.h"
#include "CEditorCameraScript.h"
void MenuUI::Tick()
{
	if (!IsActive())
		return;

	if (ImGui::BeginMainMenuBar())
	{
		File();
		Tools();
		Assets();
		External();
		EditorSettings();

		OutputInfo();

		ImGui::EndMainMenuBar();
	}
}


void MenuUI::Update()
{
}

void MenuUI::File()
{
	if (ImGui::BeginMenu(ICON_FA_FILE " File"))
	{
		if (ImGui::MenuItem(u8"�� ����", " Ctrl + N"))
		{
			Inspector* inspector = reinterpret_cast<Inspector*>(CEditorMgr::GetInst()->FindEditorUI("Inspector"));
			inspector->SetTargetObject(nullptr);
			inspector->SetTargetAsset(nullptr);

			CLevel* pLevel = CLevelSaveLoad::LoadLevel(L"level\\NewLevel.lv");
			ChangeLevel(pLevel, LEVEL_STATE::STOP);
		}

		if (ImGui::MenuItem(u8"���� �ҷ�����", " Ctrl + O"))
		{
			Inspector* inspector = reinterpret_cast<Inspector*>(CEditorMgr::GetInst()->FindEditorUI("Inspector"));
			inspector->SetTargetObject(nullptr);
			inspector->SetTargetAsset(nullptr);
			LoadLevel();
		}

		if (ImGui::MenuItem(u8"���� ����", " Ctrl + S"))
		{
			CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			const wstring& levelName = pLevel->GetName();
			if (!levelName.empty())
			{
				if (levelName == L"NewLevel")
				{
					SaveLevelAs();
				}
				else
				{
					wstring strLevelPath = L"level\\" + levelName + L".lv";
					CLevelSaveLoad::SaveLevel(strLevelPath, pLevel);
				}
			}
		}
		ImGui::SetItemTooltip(u8"���� ��������\n������ �����մϴ�");

		if (ImGui::MenuItem(u8"�ٸ� �̸����� ����", " Ctrl + Shift + S"))
		{
			SaveLevelAs();
		}

		if (ImGui::MenuItem(u8"���� Ž����", "Ctrl + E"))
		{
			CEditorMgr::GetInst()->FindEditorUI("FileBrowser")->Toggle();
		}

		if (ImGui::MenuItem(ICON_FA_POWER_OFF , "CTRL + F4"))
		{
			PostQuitMessage(0);
		}
		ImGui::EndMenu();
	}
}

void MenuUI::Tools()
{
	if (ImGui::BeginMenu(ICON_FA_WRENCH " Tools"))
	{
		if (ImGui::MenuItem(u8"Ÿ�ϸ� ������", " Alt + T"))
		{
			TilemapEditor* editor = static_cast<TilemapEditor*>(CEditorMgr::GetInst()->FindEditorUI("TilemapEditor"));
			editor->SetActive(true);
			editor->Activate();
		}
		if (ImGui::MenuItem(u8"��������Ʈ ������", " Alt + S"))
		{
			SpriteEditor* spriteEditor = static_cast<SpriteEditor*>(CEditorMgr::GetInst()->FindEditorUI("SpriteEditor"));
			spriteEditor->Toggle();
		}

		if (ImGui::BeginMenu(u8"�ִϸ��̼� ������", " Alt + A"))
		{
			if (ImGui::MenuItem(u8"2D�ִϸ��̼�"))
			{
				Animation2DEditor* animEditor = static_cast<Animation2DEditor*>(CEditorMgr::GetInst()->FindEditorUI("Animation2DEditor"));
				animEditor->SetActive(true);
			}

			if (ImGui::MenuItem(u8"3D�ִϸ��̼�"))
			{
				Animation3DEditor* animEditor = static_cast<Animation3DEditor*>(CEditorMgr::GetInst()->FindEditorUI("Animation3DEditor"));
				animEditor->SetActive(true);
			}

			ImGui::EndMenu();
		}
		

		if (ImGui::MenuItem(u8"���̾� ����", " Alt + C"))
		{
			CollisionCheck* animEditor = static_cast<CollisionCheck*>(CEditorMgr::GetInst()->FindEditorUI(ICON_FA_CHECK_SQUARE_O" CollisionCheck"));
			animEditor->SetActive(true);
		}

		if (ImGui::MenuItem(u8"�ؽ�ó ��", " Alt + P"))
		{
			CEditorMgr::GetInst()->FindEditorUI(ICON_FA_PICTURE_O" Texture Preview")->Toggle();
		}

		ImGui::EndMenu();
	}
}

void MenuUI::Assets()
{
	if (ImGui::BeginMenu(ICON_FA_SUITCASE " Assets"))
	{
		if (ImGui::MenuItem(ICON_FA_FLOPPY_O " Save Material"))
		{
			Inspector* pInspector = static_cast<Inspector*>(CEditorMgr::GetInst()->FindEditorUI("Inspector"));
			Ptr<CMaterial> curMat = dynamic_cast<CMaterial*>(pInspector->GetTargetAsset().Get());
			if (curMat != nullptr)
			{
				const wstring& wstrKey = curMat->GetKey();
				const wstring& wstrPath = CPathMgr::GetInst()->GetContentPath();
				curMat->Save(L"material\\" + wstrKey + L".mtrl");
			}
		}
		ImGui::SetItemTooltip(u8"������ ���Ϸ� �����մϴ�.");

		AddScript();


		ImGui::EndMenu();
	}
}

void MenuUI::External()
{
	if (ImGui::BeginMenu(ICON_FA_GLOBE " External"))
	{
		if (ImGui::MenuItem(ICON_FA_GITHUB_SQUARE " Github"))
		{
			ShellExecute(0, 0, L"https://github.com/whwogur", 0, 0, SW_SHOW);
		}

		if (ImGui::MenuItem(ICON_FA_YOUTUBE_PLAY " Youtube"))
		{
			ShellExecute(0, 0, L"https://www.youtube.com/@user-vy8dx1rr4q", 0, 0, SW_SHOW);
		}

		if (ImGui::MenuItem(ICON_FA_TWITCH " Aesprite"))
		{

		}

		if (ImGui::MenuItem(ICON_FA_TERMINAL" Powershell"))
		{
			ShellExecute(NULL, L"open", L"cmd.exe", NULL, NULL, SW_SHOWNORMAL);
		}

		ImGui::EndMenu();
	}
}

void MenuUI::OutputInfo()
{
	// Debug Toggle
	bool& bDebug = CRenderMgr::GetInst()->GetDebugRenderRef();
	ToggleButton("##DebugToggle", &bDebug);
	ImGui::SameLine();
	ImGui::Text(bDebug ? "Debug ON" : "Debug OFF");

	// Info
	float contentRegionAvailable = ImGui::GetContentRegionAvail().x;
	LEVEL_STATE state = CLevelMgr::GetInst()->GetCurrentLevel()->GetState();
	string whichCamera;
	ImVec4 color;
	UINT FPS = CTimeMgr::GetInst()->GetFPSRecord();
	char buffer[30];
	const char* iconFun = FPS > 500 ? ICON_FA_FIGHTER_JET :
		(FPS > 100 ? ICON_FA_BICYCLE : ICON_FA_WHEELCHAIR);

	sprintf_s(buffer, "%s FPS: %d", iconFun, FPS);

	if (state == LEVEL_STATE::PLAY)
	{
		whichCamera = ICON_FA_CAMERA_RETRO " MAINCAM";

		ImGui::SameLine(contentRegionAvailable / 2);
		if (ImGui::Button(ICON_FA_PAUSE, { 22, 22 }))
		{
			ChangeLevelState(LEVEL_STATE::PAUSE);
			EDITOR_TRACE("Paused");
		}
		ImGui::SameLine(contentRegionAvailable / 2 + 30);
		if (ImGui::Button(ICON_FA_STOP, { 22, 22 }))
		{
			const wstring& lvName = CLevelMgr::GetInst()->GetCurrentLevel()->GetName();
			CLevel* pLoadedLevel = CLevelSaveLoad::LoadLevel(L"level\\" + lvName + L".lv");
			ChangeLevel(pLoadedLevel, LEVEL_STATE::STOP);

			// Inspector Clear �ϱ� (���� ������Ʈ ������ �����ְ� ���� ���� �ֱ� ������)				
			Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
			pInspector->SetTargetObject(nullptr);
			pInspector->SetTargetAsset(nullptr);
		}

		color = { 0.45f, 0.55f, 0.88f, 1.0f };
	}
	else if (state == LEVEL_STATE::PAUSE)
	{
		whichCamera = ICON_FA_PAUSE" PAUSED";
		ImGui::SameLine(contentRegionAvailable / 2);
		if (ImGui::Button(ICON_FA_PLAY, { 22, 22 }))
		{
			ChangeLevelState(LEVEL_STATE::PLAY);
		}
		ImGui::SameLine(contentRegionAvailable / 2 + 30);
		if (ImGui::Button(ICON_FA_STOP, { 22, 22 }))
		{
			const wstring& lvName = CLevelMgr::GetInst()->GetCurrentLevel()->GetName();
			CLevel* pLoadedLevel = CLevelSaveLoad::LoadLevel(L"level\\" + lvName + L".lv");
			ChangeLevel(pLoadedLevel, LEVEL_STATE::STOP);

			// Inspector Clear �ϱ� (���� ������Ʈ ������ �����ְ� ���� ���� �ֱ� ������)				
			Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
			pInspector->SetTargetObject(nullptr);
			pInspector->SetTargetAsset(nullptr);
		}

		color = { 1, 0, 0, 1 };
	}
	else
	{
		whichCamera = ICON_FA_CAMERA_RETRO " EDITORCAM";
		ImGui::SameLine(contentRegionAvailable / 2);
		if (ImGui::Button(ICON_FA_PLAY, { 22, 22 }))
		{
			ChangeLevelState(LEVEL_STATE::PLAY);
			EDITOR_TRACE("Play");
		}

		color = { 1, 1, 1, 1 };

	}

	ImGui::SameLine(contentRegionAvailable);

	ImGui::TextColored(color, whichCamera.c_str());
	ImGui::SameLine();
	ImGui::TextColored(color, buffer);
}

void MenuUI::EditorSettings()
{
	if (ImGui::BeginMenu(ICON_FA_COG " Editor"))
	{
		if (ImGui::BeginMenu(u8"�׸� ����"))
		{
			if (ImGui::MenuItem("Microsoft"))
			{
				CEditorMgr::GetInst()->SetThemeMicrosoft();
			}

			if (ImGui::MenuItem("Unreal"))
			{
				CEditorMgr::GetInst()->SetThemeUnrealEngine();
			}

			if (ImGui::MenuItem("Moonlight"))
			{
				CEditorMgr::GetInst()->SetThemeMoonlight();
			}

			if (ImGui::MenuItem("Dark"))
			{
				CEditorMgr::GetInst()->SetThemeFutureDark();
			}

			ImGui::EndMenu();
		}


		ImGui::EndMenu();
	}
}

void MenuUI::LoadLevel()
{
	// open a file name
	OPENFILENAME ofn = {};

	wstring wstrPath = CPathMgr::GetInst()->GetContentPath();
	wstrPath += L"level\\";

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
	ofn.lpstrInitialDir = wstrPath.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (false == GetOpenFileName(&ofn))
		return;

	path relPath = szFilePath;
	wstring rel = relPath.filename();
	CLevel* pLevel = CLevelSaveLoad::LoadLevel(L"level\\" + rel);
	ChangeLevel(pLevel, LEVEL_STATE::STOP);
}

void MenuUI::SaveLevelAs()
{
	// open a file name
	OPENFILENAME ofn = {};

	wstring wstrPath = CPathMgr::GetInst()->GetContentPath();
	wstrPath += L"level\\";


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
	ofn.lpstrInitialDir = wstrPath.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (false == GetSaveFileName(&ofn))
		return;

	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	path relPath = szFilePath;
	wstring rel = relPath.filename();
	pLevel->SetName(rel);
	rel += L".lv";
	CLevelSaveLoad::SaveLevel(L"level\\" + rel, pLevel);
}

void MenuUI::AddScript()
{
	if (ImGui::BeginMenu(ICON_FA_CODE " Add Script"))
	{
		std::vector<wstring> vecScriptsName;
		CScriptMgr::GetScriptInfo(vecScriptsName);

		for (size_t i = 0; i < vecScriptsName.size(); ++i)
		{
			if (ImGui::MenuItem(string(vecScriptsName[i].begin(), vecScriptsName[i].end()).c_str()))
			{
				Inspector* pInspector = static_cast<Inspector*>(CEditorMgr::GetInst()->FindEditorUI("Inspector"));
				CGameObject* pObject = pInspector->GetTargetObject();
				
				if (nullptr != pObject)
				{
					// ������Ʈ��, ������ ��ũ��Ʈ�� �߰�����
					CScript* pScript = CScriptMgr::GetScript(vecScriptsName[i]);
					pObject->AddComponent(pScript);
					pInspector->SetTargetObject(pObject);
				}
			}
		}

		ImGui::EndMenu();
	}
}
