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
			CalcSpriteSize(vPixelPos);
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

void SE_AtlasView::CalcSpriteSize(Vec2 _PixelPos)
{
	float left = m_AtlasTex->Width() - 1.f;
	float top = m_AtlasTex->Height() - 1.f;
	float right = 0.f;
	float bot = 0.f;


	// 등록아이디 검사용 Set Clear
	m_PixelID.clear();

	// Quene 에 클릭한 최초의 픽셀좌표 입력
	list<Vec2>	queue;
	queue.push_back(_PixelPos);

	while (!queue.empty())
	{
		Vec2 vPixelPos = queue.front();
		queue.pop_front();

		if (vPixelPos.x < left)
			left = vPixelPos.x;
		if (vPixelPos.x > right)
			right = vPixelPos.x;
		if (vPixelPos.y < top)
			top = vPixelPos.y;
		if (vPixelPos.y > bot)
			bot = vPixelPos.y;


		// 주변 픽셀을 queue 에 넣는다.
		Vec2 vUp = vPixelPos + Vec2(0.f, 1.f);	  // 위		
		Vec2 vDown = vPixelPos + Vec2(0.f, -1.f); // 아래		
		Vec2 vLeft = vPixelPos + Vec2(-1.f, 0.f); // 좌		
		Vec2 vRight = vPixelPos + Vec2(1.f, 0.f); // 우

		if (IsPixelOk(vUp))
		{
			queue.push_back(vUp);
			m_PixelID.insert(vUp);
		}

		if (IsPixelOk(vDown))
		{
			queue.push_back(vDown);
			m_PixelID.insert(vDown);
		}

		if (IsPixelOk(vLeft))
		{
			queue.push_back(vLeft);
			m_PixelID.insert(vLeft);
		}

		if (IsPixelOk(vRight))
		{
			queue.push_back(vRight);
			m_PixelID.insert(vRight);
		}
	}

	//m_MouseLT = ImVec2(left, top);
	m_MouseRB = ImVec2(right, bot);
	MD_ENGINE_TRACE("{0},{1} {2},{3}", m_MouseLT.x, m_MouseLT.y, m_MouseRB.x, m_MouseRB.y);
	MD_ENGINE_TRACE(L"크기 : {:.2f},{:.2f}", m_MouseRB.x - m_MouseLT.x, m_MouseRB.y - m_MouseLT.y);
}

bool SE_AtlasView::IsPixelOk(Vec2 _PixelPos)
{
	// 해당 픽셀이 아틀라스 해상도 영역을 벗어난 좌표라면
	if (_PixelPos.x < 0 || m_AtlasTex->Width() <= _PixelPos.x
		|| _PixelPos.y < 0 || m_AtlasTex->Height() <= _PixelPos.y)
	{
		return false;
	}

	// 이미 등록된 적이 있는 Pixel 이라면
	if (m_PixelID.end() != m_PixelID.find(_PixelPos))
		return false;

	// 픽셀의 알파값이 0 이라면
	int PixelIdx = m_AtlasTex->Width() * (int)_PixelPos.y + (int)_PixelPos.x;
	if (0 == m_AtlasTex->GetPixels()[PixelIdx].a)
		return false;

	return true;
}