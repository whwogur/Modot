#include "pch.h"
#include "SE_AtlasView.h"
#include "SE_Detail.h"
#include <Engine/CKeyMgr.h>
#include "TreeUI.h"
#include "CEditorMgr.h"
SE_AtlasView::SE_AtlasView()
	: m_WidthSize(200)
	, m_WheelScale(1.f)
{
}

void SE_AtlasView::Init()
{
	m_AtlasTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"Checkerboard");
}

void SE_AtlasView::Update()
{
	if (nullptr == m_AtlasTex)
		return;

	// 이미지	
	ImVec2 uv_min = ImVec2(0.0f, 0.0f);
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);

	ImVec4 tint_col(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 border_col(0.7f, 0.7f, 0.7f, 1.0f);

	// 실제 해상도 대비 출력 Image 의 비율
	m_Ratio = (m_WidthSize * m_WheelScale) / m_AtlasTex->Width();

	ImGui::Image((ImTextureID)m_AtlasTex->GetSRV().Get(), ImVec2((m_WidthSize * m_WheelScale), m_AtlasTex->Height() * m_Ratio)
		, uv_min, uv_max, tint_col, border_col);
	ImageRectMin = ImGui::GetItemRectMin();
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
				GetDetail()->SetAtlasTex(m_AtlasTex);
			}
		}

		ImGui::EndDragDropTarget();
	}

	if (ImGui::IsWindowFocused() && ImGui::IsMousePosValid())
	{
		WheelCheck();
		SelectCheck();
		DrawSelectRect();
	}
}


void SE_AtlasView::SetAtlasTex(Ptr<CTexture> _Tex)
{
	if (m_AtlasTex == _Tex)
		return;

	m_AtlasTex = _Tex;

	m_WidthSize = m_AtlasTex->Width();
}

void SE_AtlasView::WheelCheck()
{
	if (0 < ImGui::GetIO().MouseWheel)
	{
		m_WheelScale += 0.05f;
	}

	if (0 > ImGui::GetIO().MouseWheel)
	{
		m_WheelScale -= 0.05f;
	}

	if (3.f < m_WheelScale)
		m_WheelScale = 3.f;
	if (m_WheelScale < 0.1f)
		m_WheelScale = 0.1f;
}

void SE_AtlasView::SelectCheck()
{
	const bool& PrecisionMode = GetOwner()->GetPrecisionRef();
	if (PrecisionMode)
	{
		// Image 위젯 좌상단 좌표
		ImageRectMin = ImGui::GetItemRectMin();
		float ArrImageMin[] = { ImageRectMin.x, ImageRectMin.y };
		ImGui::InputFloat2("ImageMin", ArrImageMin);

		// 현재 마우스 위치
		m_MousePos = ImGui::GetMousePos();
		float arrMousePos[] = { m_MousePos.x, m_MousePos.y };
		ImGui::InputFloat2("MousePos", arrMousePos);

		ImVec2 vDiff = ImVec2(m_MousePos.x - ImageRectMin.x, m_MousePos.y - ImageRectMin.y);
		vDiff = ImVec2(vDiff.x / m_Ratio, vDiff.y / m_Ratio);

		// 마우스 위치의 아틀라스 픽셀좌표	
		float PixelPos[] = { vDiff.x, vDiff.y };
		ImGui::InputFloat2("PixelPos", PixelPos);

		// 마우스 왼쪽 Tap 체크
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			m_MouseLT = ImGui::GetMousePos();
			ImVec2 vDiff = ImVec2(m_MouseLT.x - ImageRectMin.x, m_MouseLT.y - ImageRectMin.y);
			m_MouseLT = ImVec2(vDiff.x / m_Ratio, vDiff.y / m_Ratio);
			string log = std::to_string(m_MouseLT.x) + " " + std::to_string(m_MouseLT.y);
			EDITOR_TRACE(log.c_str());
			GetDetail()->SetLeftTop({ m_MouseLT.x, m_MouseLT.y });
		}

		if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
		{
			m_MouseRB = ImGui::GetMousePos();
			ImVec2 vDiff = ImVec2(m_MouseRB.x - ImageRectMin.x, m_MouseRB.y - ImageRectMin.y);
			m_MouseRB = ImVec2(vDiff.x / m_Ratio, vDiff.y / m_Ratio);
		}

		if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		{
			string log = std::to_string(m_MouseRB.x - m_MouseLT.x) + " " + std::to_string(m_MouseRB.y - m_MouseLT.y);
			EDITOR_TRACE(log.c_str());

			GetDetail()->SetSlice({ m_MouseRB.x - m_MouseLT.x, m_MouseRB.y - m_MouseLT.y });
		}
	}
	else
	{
		// 마우스 왼쪽클릭 체크
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			m_MouseLT = ImGui::GetMousePos();
			ImVec2 vDiff = ImVec2(m_MouseLT.x - ImageRectMin.x, m_MouseLT.y - ImageRectMin.y);
			m_MouseLT = ImVec2(vDiff.x / m_Ratio, vDiff.y / m_Ratio);
		}

		if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
		{
			m_MouseRB = ImGui::GetMousePos();
			ImVec2 vDiff = ImVec2(m_MouseRB.x - ImageRectMin.x, m_MouseRB.y - ImageRectMin.y);
			m_MouseRB = ImVec2(vDiff.x / m_Ratio, vDiff.y / m_Ratio);
			string size = std::to_string(m_MouseRB.x - m_MouseLT.x) + ", " + std::to_string(m_MouseRB.y - m_MouseLT.y);
			ImGui::SetItemTooltip(size.c_str());
		}

		if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		{
			Vec2 vPixelPos = Vec2(ImGui::GetMousePos().x, ImGui::GetMousePos().y);
			ImVec2 vDiff = ImVec2(vPixelPos.x - ImageRectMin.x, vPixelPos.y - ImageRectMin.y);
			vPixelPos = Vec2(vDiff.x / m_Ratio, vDiff.y / m_Ratio);

			if (0.f <= vPixelPos.x && vPixelPos.x < m_AtlasTex->Width()
				&& 0.f <= vPixelPos.y && vPixelPos.y < m_AtlasTex->Height())
			{
				m_MouseRB = ImVec2(vPixelPos.x, vPixelPos.y);
				int ltx = (int)std::floor(m_MouseLT.x / m_BGSize.x) * m_BGSize.x;
				int lty = (int)std::floor(m_MouseLT.y / m_BGSize.y) * m_BGSize.y;

				GetDetail()->SetLeftTop({ ltx, lty });
			}
		}
	}
}

void SE_AtlasView::DrawSelectRect()
{
	ImVec2 MouseLTPos = ImVec2(m_MouseLT.x * m_Ratio + ImageRectMin.x, m_MouseLT.y * m_Ratio + ImageRectMin.y);
	ImVec2 MouseRBPos = ImVec2(m_MouseRB.x * m_Ratio + ImageRectMin.x, m_MouseRB.y * m_Ratio + ImageRectMin.y);

	ImGui::GetWindowDrawList()->AddRect(MouseLTPos, MouseRBPos
		, ImGui::GetColorU32(HEADER_2), 0.1f, 0, 1.f);
}