#include "pch.h"
#include "SE_AtlasView.h"
#include "SE_Detail.h"
#include <Engine/CKeyMgr.h>
SE_AtlasView::SE_AtlasView()
	: m_WidthSize(200)
	, m_WheelScale(1.f)
{
}

SE_AtlasView::~SE_AtlasView()
{
}

void SE_AtlasView::Init()
{
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

	ImGui::Image(m_AtlasTex->GetSRV().Get(), ImVec2((m_WidthSize * m_WheelScale), m_AtlasTex->Height() * m_Ratio)
		, uv_min, uv_max, tint_col, border_col);

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
			int rbx = std::floor(m_MouseRB.x / 64) * 64;
			int rby = std::floor(m_MouseRB.y / 64) * 64;
			int ltx = std::floor(m_MouseLT.x / 64) * 64;
			int lty = std::floor(m_MouseLT.y / 64) * 64;

			MD_ENGINE_TRACE("{0},{1} / {2},{3}", ltx, lty, rbx, rby);
			GetDetail()->SetLeftTop({ ltx, lty });
			GetDetail()->SetSlice({ rbx, rby });
		}
	}
}

void SE_AtlasView::DrawSelectRect()
{
	ImVec2 MouseLTPos = ImVec2(m_MouseLT.x * m_Ratio + ImageRectMin.x, m_MouseLT.y * m_Ratio + ImageRectMin.y);
	ImVec2 MouseRBPos = ImVec2(m_MouseRB.x * m_Ratio + ImageRectMin.x, m_MouseRB.y * m_Ratio + ImageRectMin.y);

	ImGui::GetWindowDrawList()->AddRect(MouseLTPos, MouseRBPos
		, ImGui::GetColorU32(ImVec4(1.f, 1.f, 0.f, 1.f)), 0.f, 0.f, 1.f);
}