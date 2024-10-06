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
            std::string previousText = ToString(pTextRender->GetText());
            static char buffer[256];
            strcpy_s(buffer, previousText.c_str());

            if (ImGui::IsWindowFocused())
            {
                if (ImGui::InputTextMultiline(u8"�ؽ�Ʈ", buffer, sizeof(buffer), {0, 200}, ImGuiInputTextFlags_CtrlEnterForNewLine))
                {
                    // buffer�� ����Ǿ����� std::string���� ��ȯ �� ������Ʈ
                    if (previousText != buffer)
                    {
                        previousText = buffer;
                        pTextRender->SetText(ToWstring(previousText));
                    }
                }
            }
            else
            {
                ImGui::InputText(u8"�ؽ�Ʈ", buffer, sizeof(buffer), ImGuiInputTextFlags_ReadOnly);
            }

            float size = pTextRender->GetTextSize();
            if (ImGui::DragFloat(u8"ũ��", &size))
                pTextRender->SetTextSize(size);

            Vec4 color = pTextRender->GetTextColor();
            color /= 255.f;
            if (ImGui::ColorEdit4(u8"����", &color.x, ImGuiColorEditFlags_DisplayRGB))
            {
                color *= 255.f;
                pTextRender->SetTextColor(color);
            }

            int camIdx = pTextRender->GetCamIdx();

            if (ImGui::InputInt(u8"ī�޶�", &camIdx, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                pTextRender->SetCamIdx(camIdx);
            }
        }
    }
}
