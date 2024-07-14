#include "pch.h"
#include "FileBrowser.h"
#include "CAssetMgr.h"

FileBrowser::FileBrowser()
{
	wstring modifiedPath;
	modifiedPath = CPathMgr::GetInst()->GetContentPath();
	modifiedPath.erase(modifiedPath.size() - 1, 1); // \ 지우는거..
	m_ContentPath = m_CurrentDirectory = modifiedPath; // 컨텐츠 상위폴더로 갈 수 없도록 하기 위해 - 나중에 다시 와서 보자
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
		m_List.push_back(make_pair(relativePath, isDirectory));
	}
}

void FileBrowser::Search(const string& _File)
{
	for (const auto& entry : std::filesystem::recursive_directory_iterator(m_ContentPath))
	{
		if (entry.is_regular_file())
		{
			const auto& path = entry.path();
			if (path.filename().string().find(_File) != std::string::npos)
			{
				MD_ENGINE_TRACE(path.c_str());
				m_CurrentDirectory = path.parent_path();
				Refresh();
			}
		}
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
	}

	static float padding = 16.0f;
	static float thumbnailSize = 50.0f;
	float cellSize = thumbnailSize + padding;

	float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = (int)(panelWidth / cellSize);
	if (columnCount < 1)
		columnCount = 1;


	ImGui::Columns(columnCount, 0, false);
	for (auto& listNode : m_List)
	{
		Ptr<CTexture> icon = listNode.second ? m_DirectoryIcon : m_FileIcon;

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::ImageButton((ImTextureID)icon->GetSRV().Get(), { thumbnailSize, thumbnailSize }, { 1, 0 }, { 0, 1 });
		ImGui::PopStyleColor();
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
				
				if (extention == L".bmp" || extention == L".png" || extention == L".jpg" || extention == L".jpeg"
					|| extention == L".BMP" || extention == L".PNG" || extention == L".JPG" || extention == L".JPEG")
				{
					CAssetMgr::GetInst()->Load<CTexture>(relPath.stem(), relPath);
				}
				
				else if (extention == L".anim")
				{
					CAssetMgr::GetInst()->Load<CAnimation>(relPath.stem(), relPath);
				}

				else
				{
					MD_ENGINE_ERROR(extention.string());
					MD_ENGINE_ERROR(L"지원하지 않는 형식 error");
				}
			}
		}
		
		std::string filenameString = listNode.first.filename().string();
		ImGui::TextWrapped(filenameString.c_str());

		ImGui::NextColumn();
	}

	ImGui::Columns(1);

	ImGui::SetNextItemWidth(50.f);
	ImGui::SliderFloat(ICON_FA_EXPAND, &thumbnailSize, 50.0f, 70.0f, "%.1f", ImGuiSliderFlags_NoInput);
	ImGui::SameLine(100);
	char searchValue[255] = {};

	ImGui::SetNextItemWidth(100.f);
	if (ImGui::InputText(ICON_FA_SEARCH, searchValue, 255, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		if (strlen(searchValue) != 0)
			Search(searchValue);
	}
}

