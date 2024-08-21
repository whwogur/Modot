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
			
			ImGui::TextColored(HEADER_1, u8"텍스처");
			ImGui::SameLine(80);
			ImGui::Image(curTex.Get()->GetSRV().Get(), { 150, 150 });
			ImGui::SetItemTooltip(u8"텍스처");
			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
				if (payload)
				{
					TreeNode** ppNode = (TreeNode**)payload->Data;
					TreeNode* pNode = *ppNode;

					Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
					if (pAsset->GetAssetType() == ASSET_TYPE::TEXTURE)
					{
						Ptr<CTexture> pTex = reinterpret_cast<CTexture*>(pAsset.Get());
						curParticleSys->SetParticleTexture(pTex);
					}
				}
				ImGui::EndDragDropTarget();
			}
			
			tParticleModule& mod = curParticleSys->GetParticleModuleRef();

			ImGui::SeparatorText(u8"모듈 정보");
			ImGui::NewLine();
			ImGui::NewLine();
			ImGui::SameLine(300); if (ImGui::Button(ICON_FA_PRINT" Build")) curParticleSys->SetParticleModule(mod);
			// 파티클 기능(모듈) 정보 세팅
			//// Spawn Module
			//m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN] = true;
			//m_Module.SpawnRate = 20;
			//m_Module.vSpawnColor = Vec4(0.24f, 0.67f, 0.87f, 1.f);
			//m_Module.MinLife = 1.f;
			//m_Module.MaxLife = 5.f;
			//m_Module.vSpawnMinScale = Vec3(50.f, 5.f, 1.f);
			//m_Module.vSpawnMaxScale = Vec3(500.f, 50.f, 1.f);
			ImGui::TextColored(HEADER_1, "Spawn Module"); ImGui::SameLine();
			ToggleButton("##sharedSpawnModule", &mod.Module[(UINT)PARTICLE_MODULE::SPAWN]);
			static int spawnRate = static_cast<int>(mod.SpawnRate);
			if (ImGui::InputInt("SpawnRate", &spawnRate))
			{
				mod.SpawnRate = static_cast<UINT>(spawnRate);
			}
			ImGui::ColorEdit4("SpawnColor", mod.vSpawnColor, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_PickerHueWheel);
			ImGui::InputFloat("MinLife", &mod.MinLife);
			ImGui::InputFloat("MaxLife", &mod.MaxLife);
			ImGui::InputFloat4("MinScale", mod.vSpawnMinScale);
			ImGui::InputFloat4("MaxScale", mod.vSpawnMaxScale);
			
			//m_Module.SpawnShape = 1;
			//m_Module.SpawnShapeScale.x = 500.f;
			//m_Module.BlockSpawnShape = 1;
			//m_Module.BlockSpawnShapeScale.x = 0.f;
			//m_Module.SpaceType = 1; // Local Space 
			static int spawnShape = static_cast<int>(mod.SpawnShape);
			static int blockShape = static_cast<int>(mod.BlockSpawnShape);
			static int spaceType = static_cast<int>(mod.SpaceType);
			ImGui::NewLine();
			if (ImGui::InputInt("Spawn Shape", &spawnShape, 0))
			{
				mod.SpawnShape = static_cast<UINT>(spawnShape);
			}
			ImGui::SetItemTooltip(u8"0:박스 1:구");
			ImGui::InputFloat("Spawn Rad", &mod.SpawnShapeScale.x);
			ImGui::SetItemTooltip(u8"반지름");
			if (ImGui::InputInt("Block Shape", &spawnShape, 0))
			{
				mod.BlockSpawnShape = static_cast<UINT>(spawnShape);
			}
			ImGui::SetItemTooltip(u8"0:박스 1:구");
			ImGui::InputFloat("Block Rad", &mod.BlockSpawnShapeScale.x);
			ImGui::SetItemTooltip(u8"반지름");
			if (ImGui::InputInt("Space Type", &spaceType, 0))
			{
				mod.SpaceType = static_cast<UINT>(spaceType);
			}
			

			//// Spawn Burst Module
			//m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] = true;
			//m_Module.SpawnBurstRepeat = true;
			//m_Module.SpawnBurstCount = 100;
			//m_Module.SpawnBurstRepeatTime = 3.f;
			ImGui::TextColored(HEADER_1, "Burst Module"); ImGui::SameLine();
			ToggleButton("##sharedBurstModule", &mod.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST]);
			static bool burstRepeat = mod.SpawnBurstRepeat == 0 ? false : true;
			static int burstCount = static_cast<int>(mod.SpawnBurstCount);
			if (ImGui::Checkbox("Burst Repeat", &burstRepeat))
			{
				burstRepeat ? mod.SpawnBurstRepeat = 1 : mod.SpawnBurstRepeat = 0;
			}
			if (ImGui::InputInt("Burst Count", &burstCount, 0))
			{
				mod.SpawnBurstCount = static_cast<UINT>(burstCount);
			}
			ImGui::InputFloat("Burst Interval", &mod.SpawnBurstRepeatTime);
			//// Scale Module
			//m_Module.Module[(UINT)PARTICLE_MODULE::SCALE] = true;
			//m_Module.StartScale = 1.f;
			//m_Module.EndScale = 1.f;
			ImGui::TextColored(HEADER_1, "Scale Module"); ImGui::SameLine();
			ToggleButton("##sharedScaleModule", &mod.Module[(UINT)PARTICLE_MODULE::SCALE]);
			ImGui::InputFloat("Start Scale", &mod.StartScale);
			ImGui::InputFloat("End Scale", &mod.EndScale);
			//// AddVelocity Module
			//m_Module.Module[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true;
			//m_Module.AddVelocityType = 1;
			//m_Module.AddVelocityFixedDir = Vec3(0.f, 1.f, 0.f);
			//m_Module.AddMinSpeed = 100.f;
			//m_Module.AddMaxSpeed = 500.f;
			ImGui::TextColored(HEADER_1, "Velocity Module"); ImGui::SameLine();
			ToggleButton("##sharedVelocityModule", &mod.Module[(UINT)PARTICLE_MODULE::ADD_VELOCITY]);
			static int velType = static_cast<int>(mod.AddVelocityType);
			if (ImGui::InputInt("Velocity Type", &velType, 0))
			{
				mod.AddVelocityType = static_cast<UINT>(velType);
			}
			ImGui::InputFloat3("Dir", mod.AddVelocityFixedDir);
			ImGui::InputFloat("Min Speed", &mod.AddMinSpeed);
			ImGui::InputFloat("Max Speed", &mod.AddMaxSpeed);
			//// Drag Module (감속)
			//m_Module.Module[(UINT)PARTICLE_MODULE::DRAG] = false;
			//m_Module.DestNormalizedAge = 1.f;
			//m_Module.LimitSpeed = 0.f;
			ImGui::TextColored(HEADER_1, "Drag Module"); ImGui::SameLine();
			ToggleButton("##sharedDragModule", &mod.Module[(UINT)PARTICLE_MODULE::DRAG]);
			ImGui::InputFloat("Dest Norm. Age", &mod.DestNormalizedAge);
			ImGui::InputFloat("Limit Speed", &mod.LimitSpeed);

			//// Noise Force Module
			//m_Module.Module[(UINT)PARTICLE_MODULE::NOISE_FORCE] = false;
			//m_Module.NoiseForceTerm = 0.3f;
			//m_Module.NoiseForceScale = 100.f;
			ImGui::TextColored(HEADER_1, "Noise Module"); ImGui::SameLine();
			ToggleButton("##sharedNoiseModule", &mod.Module[(UINT)PARTICLE_MODULE::NOISE_FORCE]);
			ImGui::InputFloat("Noise Freq", &mod.NoiseForceTerm);
			ImGui::InputFloat("Noise Scale", &mod.NoiseForceScale);
			//// Render Module
			//m_Module.Module[(UINT)PARTICLE_MODULE::RENDER] = true;
			//m_Module.EndColor = Vec3(1.f, 0.f, 0.f);
			//m_Module.FadeOut = true;
			//m_Module.FadeOutStartRatio = 0.9f;
			//m_Module.VelocityAlignment = true;
			ImGui::TextColored(HEADER_1, "Render Module"); ImGui::SameLine();
			ToggleButton("##sharedRenderModule", &mod.Module[(UINT)PARTICLE_MODULE::RENDER]);
			ImGui::ColorEdit4("End Color", mod.EndColor, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_PickerHueWheel);
			static int fadeOut = static_cast<int>(mod.FadeOut);
			static int velAlign = static_cast<int>(mod.VelocityAlignment);
			if (ImGui::InputInt("Fadeout", &fadeOut, 0))
			{
				mod.FadeOut = static_cast<UINT>(fadeOut);
			}
			ImGui::InputFloat("Fade Start Ratio", &mod.FadeOutStartRatio);
			if (ImGui::InputInt("Align with Vel", &velAlign, 0))
			{
				mod.VelocityAlignment = static_cast<UINT>(velAlign);
			}
		}
	}
}