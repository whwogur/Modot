#include "pch.h"
#include "SpriteEditor.h"
#include "CSprite.h"
#include "CPathMgr.h"

SpriteEditor::SpriteEditor()
	: m_AtlasTex(nullptr)
	, m_UVStart{-1, -1}
	, m_UVEnd{-1, -1}
	, m_ImagePos{}
	, m_UVpair{}
{
}

SpriteEditor::~SpriteEditor()
{
}

void SpriteEditor::Update()
{
	MD_ENGINE_ASSERT(m_AtlasTex != nullptr, L"Atlas Texture is Missing");
	wstring key = m_AtlasTex->GetKey();
	string imgTitle = string(key.begin(), key.end());
	float imgWidth = (float)m_AtlasTex->GetDesc().Width;
	float imgHeight = (float)m_AtlasTex->GetDesc().Height;
	ImVec2 winSize = ImGui::GetWindowSize();

	ImGui::SetWindowFontScale(1.18f);
	ImGui::TextColored({ 0.4f, 0.77f, 0.95f, 1.0f }, u8"��Ʋ�� :");
	ImGui::SameLine(140);
	ImGui::Text(imgTitle.c_str());

	ImGui::TextColored({ 0.4f, 0.77f, 0.95f, 1.0f }, u8"���õ� uv :");
	ImGui::SameLine(140);
	ImGui::Text("< TopL(%.4f, %.4f), BottomR(%.4f, %.4f) >", m_UVpair.first.x, m_UVpair.first.y, m_UVpair.second.x, m_UVpair.second.y);
	ImGui::SameLine(800);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.341f, 0.0f, 1.0f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.44f, 0.2f, 1.0f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.0f, 0.8f, 1.0f });
	static char spriteRelPath[50] = {};
	ImGui::SetNextItemWidth(150);
	ImGui::InputText("##SpriteRelPath", spriteRelPath, sizeof(spriteRelPath), ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_AlwaysOverwrite);

	ImGui::SameLine(1000);
	if (ImGui::Button(ICON_FA_FLOPPY_O, { 40, 40 }))
	{
		if (m_UVpair.first.x > 0 && m_UVpair.second.x > 0)
		{
			Ptr<CSprite> pSprite = new CSprite;
			
			const wstring& contentPath = CPathMgr::GetInst()->GetContentPath();
			string strRelPath(spriteRelPath);
			wstring wstrRelPath(strRelPath.begin(), strRelPath.end());
			Vec2 UVTopLeft(m_UVpair.first.x, m_UVpair.first.y);
			Vec2 UVBottomRight(m_UVpair.second.x, m_UVpair.second.y);

			pSprite->Create(m_AtlasTex, UVTopLeft, UVBottomRight);
			pSprite->Save(contentPath + wstrRelPath);
		}
	}
	ImGui::PopStyleColor(3);
	

	if (imgWidth > winSize.x)
	{
		ImGui::SetScrollX(ImGui::GetScrollX() - ImGui::GetMouseDragDelta(ImGuiMouseButton_Middle).x);
	}

	if (imgWidth > winSize.y)
	{
		ImGui::SetScrollY(ImGui::GetScrollY() - ImGui::GetMouseDragDelta(ImGuiMouseButton_Middle).y);
	}

	m_ImagePos = ImGui::GetCursorPos();
	ImGui::Image(m_AtlasTex->GetSRV().Get(), { imgWidth, imgHeight }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 }, { 0.4f, 0.6f, 0.7f, 1.0f });

	if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
	{
		if (m_UVStart.x < 0)
		{
			m_MouseStart = ImGui::GetMousePos();
			auto [winPosX, winPosY] = ImGui::GetWindowPos();
			m_UVStart = ImVec2(ImGui::GetMousePos().x - winPosX, ImGui::GetMousePos().y - winPosY);
		}
		ImDrawList* drawList = ImGui::GetWindowDrawList(); // �巡�� - ���簢�� �׸���
		drawList->AddRect(m_MouseStart, ImGui::GetMousePos(), IM_COL32(0, 255, 0, 255), 0.0f);
	}

	if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
	{
		if (m_UVStart.x >= 0)
		{
			auto [winPosX, winPosY] = ImGui::GetWindowPos();
			m_UVEnd = ImVec2(ImGui::GetMousePos().x - winPosX, ImGui::GetMousePos().y - winPosY);

			float UVStartX = (m_UVStart.x - m_ImagePos.x) / imgWidth;
			float UVStartY = (m_UVStart.y - m_ImagePos.y) / imgHeight;
			float UVEndX = (m_UVEnd.x - m_ImagePos.x) / imgWidth;
			float UVEndY = (m_UVEnd.y - m_ImagePos.y) / imgHeight;

			m_UVpair = std::make_pair(ImVec2(UVStartX, UVStartY), ImVec2(UVEndX, UVEndY));

			m_UVStart = m_UVEnd = ImVec2(-1, -1);

			MD_ENGINE_INFO("UVPair : start(x: {0}, y: {1}) end(x: {2}, y: {3})",
				m_UVpair.first.x, m_UVpair.first.y, m_UVpair.second.x, m_UVpair.second.y);
		}
	}

	if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_Escape))
	{
		m_UVStart = m_UVEnd = ImVec2(-1, -1);

		MD_ENGINE_INFO("Reset : start(x: {0}, y: {1}) end(x: {2}, y: {3})",
			m_UVStart.x, m_UVStart.y, m_UVEnd.x, m_UVEnd.y);
	}
}
