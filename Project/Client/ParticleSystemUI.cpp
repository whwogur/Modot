#include "pch.h"
#include "ParticleSystemUI.h"
#include <Engine/CParticleSystem.h>
#include <Engine/CAssetMgr.h>
#include <Engine/CParticleTickCS.h>
#include "TreeUI.h"
#include <ModotHelpers.h>
static string CSTypeToString[2] = { "Default", "FallingLeaves" };

ParticleSystemUI::ParticleSystemUI()
	: ComponentUI(COMPONENT_TYPE::PARTICLESYSTEM)
{
}

void ParticleSystemUI::Update()
{
	Title();
	if (!Collapsed())
	{
		CGameObject* curObj = GetTargetObject();
		if (curObj != nullptr)
		{
			CParticleSystem* curParticleSys = curObj->ParticleSystem();
			if (curParticleSys != nullptr)
			{
				Ptr<CTexture> curTex = curParticleSys->GetParticleTex();

				if (curTex == nullptr)
					curTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"Checkerboard");

				ImGui::SameLine(80);
				ImGui::Image((ImTextureID)curTex.Get()->GetSRV().Get(), { 150, 150 });
				ImGui::SetItemTooltip(u8"공유 텍스처");
				/*if (ImGui::BeginDragDropTarget())
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
				}*/

				tParticleModule& mod = curParticleSys->GetParticleModuleRef();

				ImGui::TextColored(HEADER_1, u8" 모듈 정보");
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.00f, 0.55f, 0.42f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.00f, 0.66f, 0.55f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.00f, 0.44f, 0.36f, 1.0f });
				ImGui::SameLine(290); if (ImGui::Button(ICON_FA_PRINT" Build")) curParticleSys->SetParticleModule(mod);
				ImGui::PopStyleColor(3);

				ImGui::SeparatorText(u8"Spawn 모듈");
				ModotHelpers::ToggleButton("##sharedSpawnModule", &mod.Module[(UINT)PARTICLE_MODULE::SPAWN]);
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
				ImGui::SetItemTooltip(u8"0: World 1: Local");


				ImGui::SeparatorText(u8"Burst 모듈");
				ModotHelpers::ToggleButton("##sharedBurstModule", &mod.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST]);
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

				ImGui::SeparatorText(u8"Scale 모듈");
				ModotHelpers::ToggleButton("##sharedScaleModule", &mod.Module[(UINT)PARTICLE_MODULE::SCALE]);
				ImGui::InputFloat("Start Scale", &mod.StartScale);
				ImGui::InputFloat("End Scale", &mod.EndScale);

				ImGui::SeparatorText(u8"Velocity 모듈");
				ModotHelpers::ToggleButton("##sharedVelocityModule", &mod.Module[(UINT)PARTICLE_MODULE::ADD_VELOCITY]);
				static int velType = static_cast<int>(mod.AddVelocityType);
				static bool velAlign = mod.VelocityAlignment == 0 ? false : true;
				if (ImGui::InputInt("Velocity Type", &velType, 0))
				{
					mod.AddVelocityType = static_cast<UINT>(velType);
				}
				ImGui::SetItemTooltip("0 : Random, 1 : From Center, 2 : To Center,\n3 : X, 4 : Fixed");
				ImGui::InputFloat3("Dir", mod.AddVelocityFixedDir);
				ImGui::InputFloat("Min Speed", &mod.AddMinSpeed);
				ImGui::InputFloat("Max Speed", &mod.AddMaxSpeed);
				if (ImGui::Checkbox("Velocity Align", &velAlign))
				{
					velAlign ? mod.VelocityAlignment = 1 : mod.VelocityAlignment = 0;
				}

				ImGui::SeparatorText(u8"Drag 모듈");
				ModotHelpers::ToggleButton("##sharedDragModule", &mod.Module[(UINT)PARTICLE_MODULE::DRAG]);
				ImGui::InputFloat("Dest Norm. Age", &mod.DestNormalizedAge);
				ImGui::InputFloat("Limit Speed", &mod.LimitSpeed);


				ImGui::SeparatorText(u8"Noise 모듈");
				ModotHelpers::ToggleButton("##sharedNoiseModule", &mod.Module[(UINT)PARTICLE_MODULE::NOISE_FORCE]);
				ImGui::InputFloat("Noise Freq", &mod.NoiseForceTerm);
				ImGui::InputFloat("Noise Scale", &mod.NoiseForceScale);

				ImGui::SeparatorText(u8"Render 모듈");
				ModotHelpers::ToggleButton("##sharedRenderModule", &mod.Module[(UINT)PARTICLE_MODULE::RENDER]);
				ImGui::ColorEdit4("End Color", mod.EndColor, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_PickerHueWheel);
				static bool fadeOut = mod.FadeOut == 0 ? false : true;
				if (ImGui::Checkbox("Fadeout", &fadeOut))
				{
					mod.FadeOut = fadeOut ? 1 : 0;
				}
				ImGui::InputFloat("Fade Start Ratio", &mod.FadeOutStartRatio);

				ImGui::SeparatorText(u8"Orbit 모듈");
				ModotHelpers::ToggleButton("##sharedRotModule", &mod.Module[(UINT)PARTICLE_MODULE::ORBIT]);
				ImGui::InputFloat("Max Rotation", &mod.MaxRotationSpeed);
				ImGui::SeparatorText(u8"Gyrate");
				ModotHelpers::ToggleButton("##sharedGyrate", &mod.Gyrate);
				ImGui::InputFloat("Gyrate Speed", &mod.GyrateSpeed);
			}
		}
	}
}