#include "pch.h"
#include "TextRenderUI.h"

#include <Engine/CTextRender.h>

TextRenderUI::TextRenderUI()
	: ComponentUI(COMPONENT_TYPE::TEXTRENDER)
{
}

void TextRenderUI::Update()
{
	Title();

	if (!Collapsed())
	{
        CTextRender* pTextRender = GetTargetObject()->TextRender();
        if (pTextRender != nullptr)
        {

        }
        wstring Str = pTextRender->GetText();
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strcpy_s(buffer, sizeof(buffer), ToString(Str).c_str());
        if (ImGui::InputText("Text", buffer, sizeof(buffer)))
        {
            pTextRender->SetText(ToWstring(buffer));
        }

        float size = pTextRender->GetTextSize();
        if (ImGui::DragFloat("Size", &size))
            pTextRender->SetTextSize(size);

        Vec4 color = pTextRender->GetTextColor();
        color /= 255.f;
        if (ImGui::ColorEdit4("Color", &color.x))
        {
            color *= 255.f;
            pTextRender->SetTextColor(color);
        }

        int CamIdx = pTextRender->GetCameraIdx();
        if (ImGui::InputInt("Camera Index", &CamIdx))
            pTextRender->SetCameraIdx(CamIdx);
	}
}
