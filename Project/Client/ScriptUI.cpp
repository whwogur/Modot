#include "pch.h"
#include "ScriptUI.h"

#include <Scripts/CScriptMgr.h>
#include <Engine/CScript.h>
#include <Engine/CAssetMgr.h>

ScriptUI::ScriptUI()
	: ComponentUI(COMPONENT_TYPE::SCRIPT)
	, m_Script(nullptr)
{
	m_IconTexture = CAssetMgr::GetInst()->FindAsset<CTexture>(L"ScriptIcon");
}

ScriptUI::~ScriptUI()
{
}


void ScriptUI::Update()
{
	ImGui::Image((void*)m_IconTexture.Get()->GetSRV().Get(), { ICON_SIZE, ICON_SIZE });
	ImGui::SameLine(ICON_SIZE + 3);
	wstring strScriptName = CScriptMgr::GetScriptName(m_Script);

	ImGui::TextColored({ 0.0f, 0.33f, 0.77f, 1.0f }, string(strScriptName.begin(), strScriptName.end()).c_str());
}

void ScriptUI::SetTargetScript(CScript* _Script)
{
	m_Script = _Script;

	if (nullptr != m_Script)
		SetActive(true);
	else
		SetActive(false);
}