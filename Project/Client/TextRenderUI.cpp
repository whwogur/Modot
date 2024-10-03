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
            static std::string previousText = ToString(pTextRender->GetText());
            char buffer[256];
            strcpy_s(buffer, previousText.c_str());

            // ImGui::InputText 사용
            if (ImGui::InputText(u8"텍스트", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                // buffer가 변경되었으면 std::string으로 변환 후 업데이트
                if (previousText != buffer)
                {
                    previousText = buffer;
                    pTextRender->SetText(ToWstring(previousText));
                }
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
        }
    }
}
