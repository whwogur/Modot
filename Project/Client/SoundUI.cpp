#include "pch.h"
#include "SoundUI.h"
#include "CTimeMgr.h"
SoundUI::SoundUI()
	: AssetUI(ASSET_TYPE::SOUND)
	, m_Playing(false)
{
}

void SoundUI::Update()
{
	static float acc = 0.f;
	static const char* hourglass[4] = { ICON_FA_HOURGLASS_O, ICON_FA_HOURGLASS_START, ICON_FA_HOURGLASS_HALF, ICON_FA_HOURGLASS_END };
	Title();
	if (m_Sound != nullptr)
	{
		if (GetAsset().Get() != m_Sound)
		{
			if (m_Playing)
				m_Sound->Stop();
			Activate();
		}
		string strPath = "sound\\" + m_strKey;

		// 전체 길이 (m_SoundLength)를 분과 초로 계산
		UINT min = (m_SoundLength / 1000) / 60;
		UINT sec = (m_SoundLength / 1000) % 60;
		string time = std::to_string(min) + " : " + (sec < 10 ? "0" : "") + std::to_string(sec);

		ImGui::NewLine();
		ImGui::SeparatorText(u8"정보");
		ImGui::NewLine();
		ImGui::SameLine(60);
		ImGui::TextColored(HEADER_1, u8"이름");
		ImGui::SameLine(120);
		ImGui::SetNextItemWidth(150.f);
		ImGui::InputText("##SoundKey", m_SoundKey, sizeof(m_SoundKey), ImGuiInputTextFlags_ReadOnly);

		ImGui::NewLine();
		ImGui::SameLine(60);
		ImGui::TextColored(HEADER_1, u8"길이");
		ImGui::SameLine(125);
		ImGui::Text(time.c_str());

		ImGui::NewLine();
		ImGui::SameLine(60);
		ImGui::TextColored(HEADER_1, u8"경로");
		ImGui::SameLine(125);
		ImGui::Text(strPath.c_str());

		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::Separator();
		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::SameLine(50);
		
		ImGui::PushStyleColor(ImGuiCol_Button, HEADER_2);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.2f, 0.87f, 0.67f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.0f, 0.4f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_SliderGrab, HEADER_2);
		ImGui::PushStyleColor(ImGuiCol_FrameBg, {1.f, 1.f, 1.f, 1.f});
		ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, 10.f);
		if (m_Playing)
		{
			if (ImGui::Button(ICON_FA_STOP, { 30, 30 }))
			{
				m_Sound->Stop();
				m_Playing = false;
				acc = 0.f;
				m_ElapsedTime = 0;
			}
			acc += EngineDT;
			if (acc > 1.f)
			{
				++m_ElapsedTime;
				acc = 0.f;
			}
			ImGui::SameLine();
			ImGui::Text("  Playing...");
			ImGui::SameLine();
			ImGui::Text(hourglass[m_ElapsedTime % 4]);
			ImGui::SameLine();
			ImGui::Text(m_SoundKey);
		}
		else
		{
			if (ImGui::Button(ICON_FA_PLAY, { 30, 30 }))
			{
				m_Sound->Play(1, 0.3f, false);
				m_Playing = true;
			}
		}

		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::SameLine(50);
		ImGui::SliderInt("##SoundPlayer", &m_ElapsedTime, 0, m_SoundLength / 1000, "##%d", ImGuiSliderFlags_NoInput);

		UINT elapsedMin = m_ElapsedTime / 60;
		UINT elapsedSec = m_ElapsedTime % 60;
		string elapsedTimeStr = std::to_string(elapsedMin) + " : " + (elapsedSec < 10 ? "0" : "") + std::to_string(elapsedSec);

		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::SameLine(60);
		string playerTime = elapsedTimeStr + "   |   " + time;
		ImGui::Text(playerTime.c_str());
		ImGui::PopStyleColor(5);
		ImGui::PopStyleVar(1);
	}
}

void SoundUI::Activate()
{
	m_Sound = (CSound*)GetAsset().Get();

	const wstring& soundKey = m_Sound->GetKey();
	m_strKey = string(soundKey.begin(), soundKey.end());
	strcpy_s(m_SoundKey, sizeof(m_SoundKey), m_strKey.c_str());
	m_SoundLength = m_Sound->GetSoundLength();

	m_Playing = false;
}

void SoundUI::Deactivate()
{
	if (m_Sound != nullptr)
	{
		m_Sound->Stop();
		m_Playing = false;
	}
}