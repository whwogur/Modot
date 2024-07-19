#include "pch.h"
#include "SpriteEditor.h"
#include "CSprite.h"
#include "CPathMgr.h"
#include "TreeUI.h"
#include "CAssetMgr.h"
#include "CTimeMgr.h"
SpriteEditor::SpriteEditor()
	: m_AtlasTex(nullptr)
	, m_UVStart{-1, -1}
	, m_ImagePos{}
{
}

SpriteEditor::~SpriteEditor()
{
}

void SpriteEditor::Update()
{

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
		if (payload)
		{
			TreeNode** ppNode = (TreeNode**)payload->Data;
			TreeNode* pNode = *ppNode;

			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (ASSET_TYPE::TEXTURE == pAsset->GetAssetType())
			{
				m_AtlasTex = (CTexture*)pAsset.Get();
			}
		}

		ImGui::EndDragDropTarget();
	}

	if (m_AtlasTex != nullptr)
	{
		wstring key = m_AtlasTex->GetKey();
		string imgTitle = string(key.begin(), key.end());
		float imgWidth = (float)m_AtlasTex->GetDesc().Width;
		float imgHeight = (float)m_AtlasTex->GetDesc().Height;
		ImVec2 winSize = ImGui::GetWindowSize();

		ImGui::SetWindowFontScale(1.18f);
		ImGui::TextColored({ 0.4f, 0.77f, 0.95f, 1.0f }, u8"��Ʋ�� :");
		ImGui::SameLine(140);
		ImGui::Text(imgTitle.c_str());

		ImGui::TextColored((m_SpriteSizeX > 0) ? ImVec4(0.4f, 0.77f, 0.95f, 1.0f) : ImVec4(1.0f, 0.0f, 0.0f, 1.0f), u8"��������Ʈ ũ�� :");
		ImGui::SameLine(140);
		ImGui::SetNextItemWidth(150);
		ImGui::InputInt("X##SpriteSizeX", &m_SpriteSizeX, 8, 16, ImGuiInputTextFlags_AutoSelectAll);
		ImGui::SameLine(350);
		ImGui::SetNextItemWidth(150);
		ImGui::InputInt("Y##SpriteSizeY", &m_SpriteSizeY, 8, 16, ImGuiInputTextFlags_AutoSelectAll);
		ImGui::SameLine(800);
		
		static char spriteRelPath[50] = {};
		ImGui::SetNextItemWidth(150);
		ImGui::InputText("##SpriteRelPath", spriteRelPath, sizeof(spriteRelPath), ImGuiInputTextFlags_AutoSelectAll);
		ImGui::SetItemTooltip(u8"��������Ʈ�� �����մϴ�.\n�̸��� �Է����ּ���.");

		ImGui::SameLine(1000);
		if (m_SpriteSizeX > 0 && m_SpriteSizeY > 0)
		{
			if (ImGui::Button(ICON_FA_FLOPPY_O, { 40, 40 }))
			{
				if (m_UVStart.x >= 0)
				{
					Ptr<CSprite> pSprite = new CSprite;
					pSprite->Create(m_AtlasTex, Vec2(m_UVStart.x * m_SpriteSizeX, m_UVStart.y * m_SpriteSizeY), Vec2(m_SpriteSizeX, m_SpriteSizeY));
					pSprite->SetBackground(Vec2(m_SpriteSizeX, m_SpriteSizeY));

					string strRelPath(spriteRelPath);
					wstring wstrRelPath(strRelPath.begin(), strRelPath.end());
					wstrRelPath += L".sprite";

					pSprite->Save(L"sprite\\" + wstrRelPath);

					CAssetMgr::GetInst()->Load<CSprite>(wstring(strRelPath.begin(), strRelPath.end()), L"sprite\\" + wstrRelPath);
					Refresh();
				}
			}
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.222f, 0.222f, 0.222f, 0.7f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.222f, 0.222f, 0.222f, 0.7f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.222f, 0.222f, 0.222f, 0.7f });
			ImGui::Button(ICON_FA_FLOPPY_O, { 40, 40 });
			ImGui::PopStyleColor(3);
			ImGui::SetItemTooltip(u8"��������Ʈ ũ�⸦ �������ּ���");
		}
			
		if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_A))
		{
			m_SpriteSizeX = m_AtlasTex->GetDesc().Width;
			m_SpriteSizeY = m_AtlasTex->GetDesc().Height;
		}

		if (imgWidth > winSize.x)
		{
			ImGui::SetScrollX(ImGui::GetScrollX() - ImGui::GetMouseDragDelta(ImGuiMouseButton_Middle).x * EngineDT);
		}

		if (imgWidth > winSize.y)
		{
			ImGui::SetScrollY(ImGui::GetScrollY() - ImGui::GetMouseDragDelta(ImGuiMouseButton_Middle).y * EngineDT);
		}

		m_ImagePos = ImGui::GetCursorPos();
		ImGui::Image(m_AtlasTex->GetSRV().Get(), { imgWidth, imgHeight }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 }, { 0.4f, 0.6f, 0.7f, 1.0f });

		if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
		{
			ImVec2 curMousePos = ImGui::GetMousePos();
			if (m_UVStart.x < 0 && m_SpriteSizeX > 0)
			{
				m_MouseStart = curMousePos;
				auto [winPosX, winPosY] = ImGui::GetWindowPos();
				m_UVStart = ImVec2(ImGui::GetMousePos().x - winPosX - m_ImagePos.x, ImGui::GetMousePos().y - winPosY - m_ImagePos.y);
				m_UVStart.x += ImGui::GetScrollX();
				m_UVStart.y += ImGui::GetScrollY();

				m_UVStart.x = std::round(m_UVStart.x / m_SpriteSizeX);
				m_UVStart.y = std::round(m_UVStart.y / m_SpriteSizeY);
			}
			ImDrawList* drawList = ImGui::GetWindowDrawList(); // �巡�� - ���簢�� �׸���
			drawList->AddRect(m_MouseStart, ImGui::GetMousePos(), IM_COL32(0, 255, 0, 255), 0.0f);
			
			float rectSizeX = curMousePos.x - m_MouseStart.x;
			float rectSizeY = curMousePos.y - m_MouseStart.y;
			string rectX = std::to_string(rectSizeX);
			string rectY = std::to_string(rectSizeY);
			string toolTip = rectX + "," + rectY;
			ImGui::SetTooltip(toolTip.c_str());
		}

		if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_Escape))
		{
			Refresh();
		}
	}
	else
	{
		ImGui::SetWindowFontScale(1.18f);
		ImGui::TextColored({ 0.4f, 0.77f, 0.95f, 1.0f }, u8"��Ʋ�� :");
		ImGui::SameLine(140);
		ImGui::Text("Not Selected");

		ImGui::TextColored({ 0.4f, 0.77f, 0.95f, 1.0f }, u8"��������Ʈ ũ�� :");
		ImGui::SameLine(140);
		ImGui::Text("???");
		ImGui::SameLine(800);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.222f, 0.222f, 0.222f, 0.7f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.222f, 0.222f, 0.222f, 0.7f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.222f, 0.222f, 0.222f, 0.7f });
		ImGui::SetNextItemWidth(150);
		ImGui::TextColored({1.0f, 0.0f, 0.0f, 1.0f}, u8"��Ʋ�󽺸� �������ּ���!!");

		ImGui::SameLine(1000);
		ImGui::Button(ICON_FA_FLOPPY_O, { 40, 40 });
		ImGui::PopStyleColor(3);
	}
}

void SpriteEditor::Refresh()
{
	m_UVStart = {-1, -1};
	m_ImagePos = {};
	m_MouseStart = {};
}
