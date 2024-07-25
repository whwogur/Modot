#include "pch.h"
#include "SE_AtlasView.h"
#include "SE_Detail.h"

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

	WheelCheck();

	// �̹���	
	ImVec2 uv_min = ImVec2(0.0f, 0.0f);
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);

	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 border_col = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);

	// ���� �ػ� ��� ��� Image �� ����
	m_Ratio = (m_WidthSize * m_WheelScale) / m_AtlasTex->Width();

	ImGui::Image(m_AtlasTex->GetSRV().Get(), ImVec2((m_WidthSize * m_WheelScale), m_AtlasTex->Height() * m_Ratio)
		, uv_min, uv_max, tint_col, border_col);

	// SelectCheck
	SelectCheck();

	// ������ ������ �簢�� �׸���
	DrawSelectRect();
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
	// Image ���� �»�� ��ǥ
	ImageRectMin = ImGui::GetItemRectMin();
	float ArrImageMin[] = { ImageRectMin.x, ImageRectMin.y };
	ImGui::InputFloat2("ImageMin", ArrImageMin);

	// ���� ���콺 ��ġ
	m_MousePos = ImGui::GetMousePos();
	float arrMousePos[] = { m_MousePos.x, m_MousePos.y };
	ImGui::InputFloat2("MousePos", arrMousePos);

	ImVec2 vDiff = ImVec2(m_MousePos.x - ImageRectMin.x, m_MousePos.y - ImageRectMin.y);
	vDiff = ImVec2(vDiff.x / m_Ratio, vDiff.y / m_Ratio);

	// ���콺 ��ġ�� ��Ʋ�� �ȼ���ǥ	
	float PixelPos[] = { vDiff.x, vDiff.y };
	ImGui::InputFloat2("PixelPos", PixelPos);

	// ���콺 ����Ŭ�� üũ
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
	}
}

void SE_AtlasView::DrawSelectRect()
{
	ImVec2 MouseLTPos = ImVec2(m_MouseLT.x * m_Ratio + ImageRectMin.x, m_MouseLT.y * m_Ratio + ImageRectMin.y);
	ImVec2 MouseRBPos = ImVec2(m_MouseRB.x * m_Ratio + ImageRectMin.x, m_MouseRB.y * m_Ratio + ImageRectMin.y);

	ImGui::GetWindowDrawList()->AddRect(MouseLTPos, MouseRBPos
		, ImGui::GetColorU32(ImVec4(1.f, 1.f, 0.f, 1.f)), 0.f, 0.f, 1.f);
}