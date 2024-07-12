#include "pch.h"
#include "FileBrowser.h"
#include "CAssetMgr.h"

FileBrowser::FileBrowser()
{
	m_CurrentDirectory = m_ContentPath = CPathMgr::GetInst()->GetContentPath();
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
	ImGui::SliderFloat(ICON_FA_SEARCH, &thumbnailSize, 50.0f, 70.0f, "%.1f", ImGuiSliderFlags_NoInput);
	//ImGui::SliderFloat(u8"패딩", &padding, 0, 32);
}

