#include "spch.h"
#include "CText2D.h"
//TODO
CText2D::CText2D()
	: CScript(SCRIPT_TYPE::TEXT2D)
{
	AddScriptParam(SCRIPT_PARAM::WSTRING, u8"내용", &m_Text); // wstring m_Text;
	AddScriptParam(SCRIPT_PARAM::COLOR, u8"주 색상", &m_MainColor);
	AddScriptParam(SCRIPT_PARAM::COLOR, u8"보조 색상", &m_SubColor);
	AddScriptParam(SCRIPT_PARAM::FLOAT, u8"폰트크기", &m_Size);
}

void CText2D::Begin()
{
	TextRender()->SetText(m_Text);
	TextRender()->SetTextColor(m_MainColor);
}

void CText2D::Tick()
{
}

void CText2D::SaveToFile(FILE* _File)
{
}

void CText2D::LoadFromFile(FILE* _File)
{
}
