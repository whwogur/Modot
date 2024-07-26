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
#include "MaterialUI.h"
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
		ImGui::Image(LogoTex->GetSRV().Get(), { 20 ,20 });

		Update();

		float contentRegionAvailable = ImGui::GetContentRegionAvail().x + 250.0f;
		LEVEL_STATE state = CLevelMgr::GetInst()->GetCurrentLevel()->GetState();
		string whichCamera;
		ImVec4 color;
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
			if (ImGui::Button(ICON_FA_PAUSE, { 32, 25 }))
			{
				ChangeLevelState(LEVEL_STATE::PAUSE);
			}
			ImGui::SameLine(contentRegionAvailable / 2 + 70);
			if (ImGui::Button(ICON_FA_STOP, { 32, 25 }))
			{
				CLevel* pLoadedLevel = CLevelSaveLoad::LoadLevel(L"level\\TestLevel.lv");
				ChangeLevel(pLoadedLevel, LEVEL_STATE::STOP);

				// Inspector Clear �ϱ� (���� ������Ʈ ������ �����ְ� ���� ���� �ֱ� ������)				
				Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
				pInspector->SetTargetObject(nullptr);
				pInspector->SetTargetAsset(nullptr);
			}
			ImGui::PopStyleColor(3);

			color = { 0.45f, 0.55f, 0.88f, 1.0f };
		}
		else if (state == LEVEL_STATE::PAUSE)
		{
			whichCamera = ICON_FA_CAMERA " EditorCamera";
			ImGui::SameLine(contentRegionAvailable / 2 + 32);
			if (ImGui::Button(ICON_FA_PLAY, { 32, 25 }))
			{
				ChangeLevelState(LEVEL_STATE::PLAY);
			}
			ImGui::SameLine(contentRegionAvailable / 2 + 70);
			if (ImGui::Button(ICON_FA_STOP, { 32, 25 }))
			{
				CLevel* pLoadedLevel = CLevelSaveLoad::LoadLevel(L"level\\TestLevel.lv");
				ChangeLevel(pLoadedLevel, LEVEL_STATE::STOP);

				// Inspector Clear �ϱ� (���� ������Ʈ ������ �����ְ� ���� ���� �ֱ� ������)				
				Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
				pInspector->SetTargetObject(nullptr);
				pInspector->SetTargetAsset(nullptr);
			}

			color = { 1, 1, 1, 1 };
		}
		else
		{
			whichCamera = ICON_FA_CAMERA " EditorCamera";
			ImGui::SameLine(contentRegionAvailable / 2 + 32);
			if (ImGui::Button(ICON_FA_PLAY, { 32, 25 }))
			{
				ChangeLevelState(LEVEL_STATE::PLAY);
			}

			color = { 1, 1, 1, 1 };
		}

		ImGui::SameLine(contentRegionAvailable - 200);
		ImGui::TextColored(color, whichCamera.c_str());
		ImGui::SameLine();
		ImGui::TextColored(color, buffer);

		ImGui::EndMainMenuBar();
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
	
}

void MenuUI::Level()
{
	ImFont* iconFont = CEditorMgr::GetInst()->GetIconFont();
	ImGui::PushFont(iconFont);

	if (ImGui::BeginMenu(ICON_FA_FILE " File"))
	{
		if (ImGui::MenuItem(u8"�� ����", "Ctrl + N"))
		{
		}

		if (ImGui::MenuItem(u8"���� �ҷ�����", "Ctrl + O"))
		{
			LoadLevel();
		}

		if (ImGui::MenuItem(u8"���� ����", "Ctrl + S"))
		{
			CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			const wstring& levelName = pLevel->GetName();
			if (!levelName.empty())
			{
				wstring strLevelPath = L"level\\" + levelName + L".lv";
				CLevelSaveLoad::SaveLevel(strLevelPath, pLevel);
			}
		}
		ImGui::SetItemTooltip(u8"���� ��������\n������ �����մϴ�");

		if (ImGui::MenuItem(u8"�ٸ� �̸����� ����", "Ctrl + Shift + S"))
		{
			SaveLevelAs();
		}

		if (ImGui::MenuItem(u8"���� Ž����", "Ctrl + E"))
		{
			CEditorMgr::GetInst()->FindEditorUI("FileBrowser")->Toggle();
		}

		if (ImGui::MenuItem(u8"����"))
		{
			PostQuitMessage(0);
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
		if (ImGui::MenuItem(u8"���� ����"))
		{
			Inspector* pInspector = static_cast<Inspector*>(CEditorMgr::GetInst()->FindEditorUI("Inspector"));
			Ptr<CMaterial> curMat = dynamic_cast<CMaterial*>(pInspector->GetTargetAsset().Get());
			if (curMat != nullptr)
			{
				const wstring& wstrKey = curMat->GetKey();
				curMat->Save(L"material\\" + wstrKey + L".mtrl");
			}
		}

		if (ImGui::MenuItem(u8"Ÿ�ϸ� ������"))
		{
			TilemapEditor* editor = static_cast<TilemapEditor*>(CEditorMgr::GetInst()->FindEditorUI("TilemapEditor"));
			editor->SetTilemap(nullptr);
			editor->Toggle();
		}
		if (ImGui::MenuItem(u8"��������Ʈ ������"))
		{
			SpriteEditor* spriteEditor = static_cast<SpriteEditor*>(CEditorMgr::GetInst()->FindEditorUI("SpriteEditor"));
			spriteEditor->Toggle();
		}

		if (ImGui::MenuItem(u8"�ִϸ��̼� ������"))
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
	rel += L".lv";
	CLevelSaveLoad::SaveLevel(L"level\\" + rel, pLevel);
}

void MenuUI::AddScript()
{
	if (ImGui::BeginMenu(u8"��ũ��Ʈ �߰�"))
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