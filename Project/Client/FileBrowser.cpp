#include "pch.h"
#include "FileBrowser.h"
#include "CAssetMgr.h"
#include "CTimeMgr.h"

FileBrowser::FileBrowser()
{
	m_CurrentDirectory = m_ContentPath = CPathMgr::GetInst()->GetContentPath();
}

void FileBrowser::Init()
{
	m_DirectoryIcon = CAssetMgr::GetInst()->Load<CTexture>(L"DirectoryIcon", L"texture\\DirectoryIcon.png");
	m_FileIcon = CAssetMgr::GetInst()->Load<CTexture>(L"FileIcon", L"texture\\FileIcon.png");
}


void FileBrowser::Tick()
{
	EditorUI::Tick();
	m_Acc += EngineDT;

	if (m_Acc > m_RefreshPeriod)
	{
		m_Acc = 0.0f;
		Update();
	}
}


void FileBrowser::Update()
{
	if (m_CurrentDirectory != m_ContentPath)
	{
		if (ImGui::Button(".."))
		{
			m_CurrentDirectory = m_CurrentDirectory.parent_path();
		}
	}

	static float padding = 16.0f;
	static float thumbnailSize = 128.0f;
	float cellSize = thumbnailSize + padding;

	float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = (int)(panelWidth / cellSize);
	if (columnCount < 1)
		columnCount = 1;

	ImGui::Columns(columnCount, 0, false);

	for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
	{
		const auto& path = directoryEntry.path();
		auto relativePath = std::filesystem::relative(path, m_ContentPath);
		std::string filenameString = relativePath.filename().string();

		Ptr<CTexture> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
		ImGui::ImageButton((ImTextureID)icon->GetSRV().Get(), {thumbnailSize, thumbnailSize}, {0, 1}, {1, 0});
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			if (directoryEntry.is_directory())
				m_CurrentDirectory /= path.filename();

		}
		ImGui::TextWrapped(filenameString.c_str());

		ImGui::NextColumn();
	}

	ImGui::Columns(1);

	ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
	ImGui::SliderFloat("Padding", &padding, 0, 32);

}

