#include "pch.h"
#include "ParticleSystemUI.h"
#include <Engine/CParticleSystem.h>
#include <Engine/CAssetMgr.h>
#include <Engine/CParticleTickCS.h>
#include "TreeUI.h"
static string CSTypeToString[2] = { "Default", "FallingLeaves" };

ParticleSystemUI::ParticleSystemUI()
	: ComponentUI(COMPONENT_TYPE::PARTICLESYSTEM)
{
}

void ParticleSystemUI::Update()
{
	Title();

	CGameObject* curObj = GetTargetObject();
	if (curObj != nullptr)
	{
		CParticleSystem* curParticleSys = curObj->ParticleSystem();
		if (curParticleSys != nullptr)
		{
			Ptr<CTexture> curTex = curParticleSys->GetParticleTex();
			
			if (curTex == nullptr)
				curTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"Checkerboard");
			
			ImGui::TextColored(HEADER_1, u8"ÅØ½ºÃ³");
			ImGui::SameLine(80);
			ImGui::Image(curTex.Get()->GetSRV().Get(), {150, 150});

			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
				if (payload)
				{
					TreeNode** ppNode = (TreeNode**)payload->Data;
					TreeNode* pNode = *ppNode;

					Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
					Ptr<CTexture> pTex = dynamic_cast<CTexture*>(pAsset.Get());
					if (pTex != nullptr)
					{
						curParticleSys->SetParticleTexture(pTex);
					}
				}
				ImGui::EndDragDropTarget();
			}

			Ptr<CParticleTickCS> pTickCS = curParticleSys->GetTickCS();
			
			if (pTickCS != nullptr)
			{
				CSType type = pTickCS->GetCSType();
				ImGui::TextColored(HEADER_1, "TickCS"); ImGui::SameLine();
				if (ImGui::BeginCombo("##TickCSSelect", CSTypeToString[(int)type].c_str()))
				{
					for (UINT i = 0; i < (UINT)CSType::END; ++i)
					{
						if (ImGui::Selectable(CSTypeToString[i].c_str()))
						{
							pTickCS->SetCSType((CSType)i);
						}
					}
					ImGui::EndCombo();
				}
			}
			else
			{
				ImGui::Text("None");
			}
			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
				if (payload)
				{
					TreeNode** ppNode = (TreeNode**)payload->Data;
					TreeNode* pNode = *ppNode;

					Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
					Ptr<CParticleTickCS> pParticleTickCS = dynamic_cast<CParticleTickCS*>(pAsset.Get());
					if (pParticleTickCS != nullptr)
					{
						curParticleSys->SetTickCS(pParticleTickCS);
					}
				}
				ImGui::EndDragDropTarget();
			}
		}
	}
}