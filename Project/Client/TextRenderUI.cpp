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
            const wstring& wstrText = pTextRender->GetText();
            string previousText(wstrText.begin(), wstrText.end());
            static char buffer[256];
            strcpy_s(buffer, previousText.c_str());

            if (ImGui::IsWindowFocused())
            {
                if (ImGui::InputTextMultiline(u8"텍스트", buffer, sizeof(buffer), {0, 200}, ImGuiInputTextFlags_CtrlEnterForNewLine))
                {
                    // buffer가 변경되었으면 std::string으로 변환 후 업데이트
                    if (previousText != buffer)
                    {
                        previousText = buffer;
                        pTextRender->SetText(wstring(previousText.begin(), previousText.end()));
                    }
                }
            }
            else
            {
                ImGui::InputText(u8"텍스트", buffer, sizeof(buffer), ImGuiInputTextFlags_ReadOnly);
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

            int camIdx = pTextRender->GetCamIdx();

            if (ImGui::InputInt(u8"카메라", &camIdx, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                pTextRender->SetCamIdx(camIdx);
            }
        }
    }
}
