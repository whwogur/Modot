#include "pch.h"
#include "SpriteEditor.h"
#include "CSprite.h"
#include "CPathMgr.h"
#include "TreeUI.h"
#include "CAssetMgr.h"
SpriteEditor::SpriteEditor()
	: m_AtlasTex(nullptr)
	, m_UVStart{-1, -1}
	, m_UVEnd{-1, -1}
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
		ImGui::TextColored({ 0.4f, 0.77f, 0.95f, 1.0f }, u8"아틀라스 :");
		ImGui::SameLine(140);
		ImGui::Text(imgTitle.c_str());

		ImGui::TextColored({ 0.4f, 0.77f, 0.95f, 1.0f }, u8"선택된 uv :");
		ImGui::SameLine(140);
		ImGui::Text("< (%.4f, %.4f) / (%.4f, %.4f) >", m_UVStart.x, m_UVStart.y, m_UVEnd.x, m_UVEnd.y);
		ImGui::SameLine(800);
		
		static char spriteRelPath[50] = {};
		ImGui::SetNextItemWidth(150);
		ImGui::InputText("##SpriteRelPath", spriteRelPath, sizeof(spriteRelPath), ImGuiInputTextFlags_AutoSelectAll);
		ImGui::SetItemTooltip(u8"스프라이트를 저장합니다.\n스프라이트 이름을 입력해주세요.");

		ImGui::SameLine(1000);
		if (ImGui::Button(ICON_FA_FLOPPY_O, { 40, 40 }))
		{
			if (m_UVStart.x > 0 && m_UVEnd.x > 0)
			{
				Ptr<CSprite> pSprite = new CSprite;
				pSprite->Create(m_AtlasTex, Vec2(m_UVStart.x, m_UVStart.y), Vec2(m_UVEnd.x - m_UVStart.x, m_UVEnd.y - m_UVStart.y));

				string strRelPath(spriteRelPath);
				wstring wstrRelPath(strRelPath.begin(), strRelPath.end());
				wstrRelPath += L".sprite";

				pSprite->Save(L"sprite\\" + wstrRelPath);
				
				CAssetMgr::GetInst()->Load<CSprite>(wstring(strRelPath.begin(), strRelPath.end()), L"sprite\\" + wstrRelPath);
				Refresh();
			}
		}

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
			ImVec2 curMousePos = ImGui::GetMousePos();
			if (m_UVStart.x < 0)
			{
				m_MouseStart = curMousePos;
				auto [winPosX, winPosY] = ImGui::GetWindowPos();
				m_UVStart = ImVec2(ImGui::GetMousePos().x - winPosX - m_ImagePos.x, ImGui::GetMousePos().y - winPosY - m_ImagePos.y);
			}
			ImDrawList* drawList = ImGui::GetWindowDrawList(); // 드래그 - 직사각형 그리기
			drawList->AddRect(m_MouseStart, ImGui::GetMousePos(), IM_COL32(0, 255, 0, 255), 0.0f);
			
			float rectSizeX = curMousePos.x - m_MouseStart.x;
			float rectSizeY = curMousePos.y - m_MouseStart.y;
			string rectX = std::to_string(rectSizeX);
			string rectY = std::to_string(rectSizeY);
			string toolTip = rectX + "," + rectY;
			ImGui::SetTooltip(toolTip.c_str());
		}

		if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
		{
			if (m_UVStart.x >= 0)
			{
				auto [winPosX, winPosY] = ImGui::GetWindowPos();
				m_UVEnd = ImVec2(ImGui::GetMousePos().x - winPosX - m_ImagePos.x, ImGui::GetMousePos().y - winPosY - m_ImagePos.y);

				MD_ENGINE_INFO("PixelPair : start(x: {0}, y: {1}) end(x: {2}, y: {3})",
					m_UVStart.x, m_UVStart.y, m_UVEnd.x, m_UVEnd.y);
			}
		}

		if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_Escape))
		{
			Refresh();
		}
	}
	else
	{
		ImGui::SetWindowFontScale(1.18f);
		ImGui::TextColored({ 0.4f, 0.77f, 0.95f, 1.0f }, u8"아틀라스 :");
		ImGui::SameLine(140);
		ImGui::Text("Not Selected");

		ImGui::TextColored({ 0.4f, 0.77f, 0.95f, 1.0f }, u8"선택된 uv :");
		ImGui::SameLine(140);
		ImGui::Text("< TopL(%.4f, %.4f), BottomR(%.4f, %.4f) >", m_UVStart.x, m_UVStart.y, m_UVEnd.x, m_UVEnd.y);
		ImGui::SameLine(800);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.222f, 0.222f, 0.222f, 0.7f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.222f, 0.222f, 0.222f, 0.7f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.222f, 0.222f, 0.222f, 0.7f });
		ImGui::SetNextItemWidth(150);
		ImGui::TextColored({1.0f, 0.0f, 0.0f, 1.0f}, u8"아틀라스를 선택해주세요!!");

		ImGui::SameLine(1000);
		if (ImGui::Button(ICON_FA_FLOPPY_O, { 40, 40 }))
		{
			
		}
		ImGui::PopStyleColor(3);
	}
}

void SpriteEditor::Refresh()
{
	m_UVStart = {-1, -1};
	m_UVEnd = {-1, -1};
	m_ImagePos = {};
	m_MouseStart = {};
}
