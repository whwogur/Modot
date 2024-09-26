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
#include "AnimationEditor.h"
#include "TilemapEditor.h"
#include "CLevelSaveLoad.h"
#include "MaterialUI.h"
#include "CollisionCheck.h"
MenuUI::MenuUI()
{
}

void MenuUI::Tick()
{
	static bool ModeToggle = false;
	if (!IsActive())
		return;

	if (ImGui::BeginMainMenuBar())
	{
		Ptr<CTexture> LogoTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"Modot_Logo");
		ImGui::Image(LogoTex->GetSRV().Get(), { 25 ,25 });

		Update();

		float contentRegionAvailable = ImGui::GetContentRegionAvail().x + 250.0f;
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
			whichCamera = ICON_FA_CAMERA " MainCam";

			ImGui::SameLine(contentRegionAvailable / 2 + 32);
			if (ImGui::Button(ICON_FA_PAUSE, { 22, 22 }))
			{
				ChangeLevelState(LEVEL_STATE::PAUSE);
				EDITOR_TRACE("Paused");
			}
			ImGui::SameLine(contentRegionAvailable / 2 + 70);
			if (ImGui::Button(ICON_FA_STOP, { 22, 22 }))
			{
				const wstring& lvName = CLevelMgr::GetInst()->GetCurrentLevel()->GetName();
				CLevel* pLoadedLevel = CLevelSaveLoad::LoadLevel(L"level\\" + lvName + L".lv");
				ChangeLevel(pLoadedLevel, LEVEL_STATE::STOP);

				// Inspector Clear 하기 (이전 오브젝트 정보를 보여주고 있을 수가 있기 때문에)				
				Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
				pInspector->SetTargetObject(nullptr);
				pInspector->SetTargetAsset(nullptr);
				EDITOR_TRACE("Stopped");
			}

			color = { 0.45f, 0.55f, 0.88f, 1.0f };
		}
		else if (state == LEVEL_STATE::PAUSE)
		{
			whichCamera = ICON_FA_CAMERA " EditorCam";
			ImGui::SameLine(contentRegionAvailable / 2 + 32);
			if (ImGui::Button(ICON_FA_PLAY, { 22, 22 }))
			{
				ChangeLevelState(LEVEL_STATE::PLAY);
				EDITOR_TRACE("Play");
			}
			ImGui::SameLine(contentRegionAvailable / 2 + 70);
			if (ImGui::Button(ICON_FA_STOP, { 22, 22 }))
			{
				const wstring& lvName = CLevelMgr::GetInst()->GetCurrentLevel()->GetName();
				CLevel* pLoadedLevel = CLevelSaveLoad::LoadLevel(L"level\\" + lvName + L".lv");
				ChangeLevel(pLoadedLevel, LEVEL_STATE::STOP);

				// Inspector Clear 하기 (이전 오브젝트 정보를 보여주고 있을 수가 있기 때문에)				
				Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
				pInspector->SetTargetObject(nullptr);
				pInspector->SetTargetAsset(nullptr);
				EDITOR_TRACE("Stopped");
			}

			color = { 1, 1, 1, 1 };
		}
		else
		{
			whichCamera = ICON_FA_CAMERA " EditorCam";
			ImGui::SameLine(contentRegionAvailable / 2 + 32);
			if (ImGui::Button(ICON_FA_PLAY, { 22, 22 }))
			{
				ChangeLevelState(LEVEL_STATE::PLAY);
				EDITOR_TRACE("Play");
			}

			color = { 1, 1, 1, 1 };
		}

		ImGui::SameLine(1000);
		bool& bDebug = CRenderMgr::GetInst()->GetDebugRenderRef();
		ToggleButton("##DebugToggle", &bDebug);
		ImGui::SetItemTooltip(u8"디버그 ON/OFF");

		ImGui::TextColored(color, whichCamera.c_str());
		ImGui::SameLine();
		ImGui::TextColored(color, buffer);

		ImGui::EndMainMenuBar();
	}
}


void MenuUI::Update()
{
	File();

	Tools();

	Assets();

	if (ImGui::BeginMenu(ICON_FA_GLOBE " External"))
	{
		if (ImGui::MenuItem(ICON_FA_GITHUB " Github"))
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

		if (ImGui::MenuItem(ICON_FA_TERMINAL" CMD"))
		{
			ShellExecute(NULL, L"open", L"cmd.exe", NULL, NULL, SW_SHOWNORMAL);
		}

		ImGui::EndMenu();
	}
}

void MenuUI::File()
{
	ImFont* iconFont = CEditorMgr::GetInst()->GetIconFont();
	ImGui::PushFont(iconFont);

	if (ImGui::BeginMenu(ICON_FA_FILE " File"))
	{
		if (ImGui::MenuItem(u8"새 레벨", " Ctrl + N"))
		{
			Inspector* inspector = reinterpret_cast<Inspector*>(CEditorMgr::GetInst()->FindEditorUI("Inspector"));
			inspector->SetTargetObject(nullptr);
			inspector->SetTargetAsset(nullptr);

			CLevel* pLevel = CLevelSaveLoad::LoadLevel(L"level\\NewLevel.lv");
			ChangeLevel(pLevel, LEVEL_STATE::STOP);
		}

		if (ImGui::MenuItem(u8"레벨 불러오기", " Ctrl + O"))
		{
			Inspector* inspector = reinterpret_cast<Inspector*>(CEditorMgr::GetInst()->FindEditorUI("Inspector"));
			inspector->SetTargetObject(nullptr);
			inspector->SetTargetAsset(nullptr);
			LoadLevel();
		}

		if (ImGui::MenuItem(u8"레벨 저장", " Ctrl + S"))
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
		ImGui::SetItemTooltip(u8"현재 편집중인\n레벨을 저장합니다");

		if (ImGui::MenuItem(u8"다른 이름으로 저장", " Ctrl + Shift + S"))
		{
			SaveLevelAs();
		}

		if (ImGui::MenuItem(u8"파일 탐색기", "Ctrl + E"))
		{
			CEditorMgr::GetInst()->FindEditorUI("FileBrowser")->Toggle();
		}

		if (ImGui::MenuItem(ICON_FA_POWER_OFF , "CTRL + F4"))
		{
			PostQuitMessage(0);
		}
		ImGui::EndMenu();
	}
	ImGui::PopFont();
}

void MenuUI::Tools()
{
	if (ImGui::BeginMenu(ICON_FA_WRENCH " Tools"))
	{
		if (ImGui::MenuItem(u8"타일맵 에디터", " Alt + T"))
		{
			/*TilemapEditor* editor = static_cast<TilemapEditor*>(CEditorMgr::GetInst()->FindEditorUI("TilemapEditor"));
			editor->SetTilemap(nullptr);
			editor->Toggle();*/
		}
		if (ImGui::MenuItem(u8"스프라이트 에디터", " Alt + S"))
		{
			SpriteEditor* spriteEditor = static_cast<SpriteEditor*>(CEditorMgr::GetInst()->FindEditorUI("SpriteEditor"));
			spriteEditor->Toggle();
		}

		if (ImGui::MenuItem(u8"애니메이션 에디터", " Alt + A"))
		{
			AnimationEditor* animEditor = static_cast<AnimationEditor*>(CEditorMgr::GetInst()->FindEditorUI("AnimationEditor"));
			animEditor->SetActive(true);
		}

		if (ImGui::MenuItem(u8"충돌 매트릭스", " Alt + C"))
		{
			CollisionCheck* animEditor = static_cast<CollisionCheck*>(CEditorMgr::GetInst()->FindEditorUI(ICON_FA_CHECK_SQUARE_O" CollisionCheck"));
			animEditor->SetActive(true);
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
		ImGui::SetItemTooltip(u8"재질을 파일로 저장합니다.");

		AddScript();


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