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
            const wstring& Str = pTextRender->GetText();
            char buffer[256] = {};
            memset(buffer, 0, sizeof(buffer));
            strcpy_s(buffer, sizeof(buffer), ToString(Str).c_str());
            if (ImGui::InputText(u8"텍스트", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                pTextRender->SetText(ToWstring(buffer));
            }

            float size = pTextRender->GetTextSize();
            if (ImGui::DragFloat(u8"크기", &size))
                pTextRender->SetTextSize(size);

            Vec4 color = pTextRender->GetTextColor();
            color /= 255.f;
            if (ImGui::ColorEdit4(u8"색깔", &color.x, ImGuiColorEditFlags_DisplayRGB))
            {
                color *= 255.f;
                pTextRender->SetTextColor(color);
            }

            int CamIdx = pTextRender->GetCameraIdx();
            if (ImGui::InputInt(u8"카메라 인덱스", &CamIdx))
                pTextRender->SetCameraIdx(CamIdx);
        }
	}
}
