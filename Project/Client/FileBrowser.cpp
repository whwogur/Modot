#include "pch.h"
#include "FileBrowser.h"
#include "CAssetMgr.h"
#include "CLevelSaveLoad.h"
#include "Inspector.h"
#include "CEditorMgr.h"

FileBrowser::FileBrowser()
{
	wstring modifiedPath;
	modifiedPath = CPathMgr::GetInst()->GetContentPath();
	modifiedPath.erase(modifiedPath.size() - 1, 1);
	m_ContentPath = m_CurrentDirectory = modifiedPath;
}

void FileBrowser::Init()
{
	m_DirectoryIcon = CAssetMgr::GetInst()->Load<CTexture>(L"DirectoryIcon", L"texture\\DirectoryIcon.png");
	m_FileIcon = CAssetMgr::GetInst()->Load<CTexture>(L"FileIcon", L"texture\\FileIcon.png");
	Refresh();
}

void FileBrowser::Refresh()
{
	m_List.clear();
	for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
	{
		const auto& path = directoryEntry.path();
		auto relativePath = std::filesystem::relative(path, m_ContentPath);

		bool isDirectory = directoryEntry.is_directory() ? true : false;
		m_List.emplace_back(relativePath, isDirectory);
	}
}

void FileBrowser::Search(const string& _File)
{
	try
	{
		for (const auto& entry : std::filesystem::recursive_directory_iterator(m_ContentPath))
		{
			if (entry.is_regular_file())
			{
				const auto& path = entry.path();
				if (path.filename().string().find(_File) != std::string::npos)
				{
					EDITOR_TRACE(path.string());
					m_CurrentDirectory = path.parent_path();
					Refresh();
					break;
				}
			}
		}
	}
	catch (const std::filesystem::filesystem_error& e)
	{
		MD_ENGINE_ERROR("File search error: {}", e.what());
	}
}

void FileBrowser::Update()
{
	if (m_CurrentDirectory != m_ContentPath)
	{
		if (ImGui::Button(ICON_FA_REPLY))
		{
			m_CurrentDirectory = m_CurrentDirectory.parent_path();
			Refresh();
		}

		ImGui::SameLine();
	}

	static float padding = 16.0f;
	static float thumbnailSize = 50.0f;
	float cellSize = thumbnailSize + padding;

	float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = (int)(panelWidth / cellSize);
	if (columnCount < 1)
		columnCount = 1;

	ImGui::SetNextItemWidth(50.f);
	ImGui::SliderFloat(ICON_FA_SEARCH_PLUS, &thumbnailSize, 50.0f, 70.0f, "%.1f", ImGuiSliderFlags_NoInput);
	ImGui::SetItemTooltip(u8"아이콘 크기 조절");
	ImGui::SameLine(100);

	static char searchValue[255] = {};

	ImGui::SetNextItemWidth(100.f);
	if (ImGui::InputText(ICON_FA_STACK_OVERFLOW, searchValue, 255, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		if (strlen(searchValue) != 0)
			Search(searchValue);
	}

	ImGui::SetItemTooltip(u8"파일 검색");

	ImGui::Columns(columnCount, 0, false);
	for (auto& listNode : m_List)
	{
		Ptr<CTexture> icon = listNode.second ? m_DirectoryIcon : 
			listNode.first.extension() == ".png" || listNode.first.extension() == ".tga" ?
			CAssetMgr::GetInst()->FindAsset<CTexture>(listNode.first.stem()) : m_FileIcon;

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::ImageButton((ImTextureID)icon->GetSRV().Get(), { thumbnailSize, thumbnailSize }, { 1, 0 }, { 0, 1 });
		ImGui::PopStyleColor();

		if (ImGui::BeginPopupContextWindow(listNode.first.string().c_str(), ImGuiPopupFlags_MouseButtonRight))
		{
			if (ImGui::MenuItem(u8"삭제"))
			{
				const wstring& wstrPath = CPathMgr::GetInst()->GetContentPath();
				string toBeErased(wstrPath.begin(), wstrPath.end());
				toBeErased += listNode.first.string();
				
				std::filesystem::remove(toBeErased);
				Refresh();
			}

			ImGui::EndPopup();
		}

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			if (listNode.second)
			{
				m_CurrentDirectory /= listNode.first.filename();
				Refresh();
				return;
			}
			else
			{
				//const string ext = listNode.first.extension().string();
				path relPath = listNode.first;
				path extention = listNode.first.extension();

				//MD_ENGINE_TRACE(ext);
				if (extention == L".lv")
				{
					//MD_ENGINE_TRACE(relPath.c_str());
					//MD_ENGINE_TRACE(extention.c_str());
					CLevel* pLoadedLevel = CLevelSaveLoad::LoadLevel(relPath.wstring());
					ChangeLevel(pLoadedLevel, LEVEL_STATE::PLAY);		
					Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
					pInspector->SetTargetObject(nullptr);
					pInspector->SetTargetAsset(nullptr);
				}
				else if (extention == L".wav") // TODO
				{
					CAssetMgr::GetInst()->Load<CSound>(relPath.stem(), relPath);
				}

				else
				{
					EDITOR_WARN(extention.string());
					EDITOR_WARN("extension not supported !");
					EDITOR_WARN("Features not supported besides loading lv & .wav files (for now)");
				}
			}
		}

		if (ImGui::BeginDragDropSource())
		{
			if (!listNode.second && listNode.first.extension() == ".png")
			{
				Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(listNode.first.stem().wstring());

				if (pTex != nullptr)
				{
					auto* pElem = &listNode;

					ImGui::SetDragDropPayload("FileBrowser", pElem, sizeof(std::pair<std::filesystem::path, bool>), ImGuiCond_Once);
					ImGui::Image(pTex->GetSRV().Get(), { 100, 100 });
					ImGui::EndDragDropSource();
				}
			}
		}
		
		std::string filenameString = listNode.first.filename().string();
		ImGui::TextWrapped(filenameString.c_str());

		ImGui::NextColumn();
	}

	ImGui::Columns(1);
}

