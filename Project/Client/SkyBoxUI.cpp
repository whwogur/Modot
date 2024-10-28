#include "pch.h"
#include "SkyBoxUI.h"
#include <Engine/CSkyBox.h>
#include <CAssetMgr.h>
#include "ParamUI.h"
SkyBoxUI::SkyBoxUI()
	: ComponentUI(COMPONENT_TYPE::SKYBOX)
{
	m_CubemapTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"Cubemap");
	m_NullTexture = CAssetMgr::GetInst()->FindAsset<CTexture>(L"ImageNotFound");
}

void SkyBoxUI::Update()
{
	Title();
	if (!Collapsed())
	{
		CSkyBox* pSkyBox = GetTargetObject()->SkyBox();

		if (pSkyBox != nullptr)
		{
			// Type
			SKYBOX_TYPE eType = pSkyBox->GetType();
			if (ImGui::BeginCombo("##SkyboxType", eType == SPHERE ? "Sphere" : "Cube"))
			{
				if (ImGui::Selectable("Sphere", eType == SPHERE ? true : false, ImGuiSelectableFlags_Disabled && eType == SPHERE))
				{
					pSkyBox->SetSkyBoxType(SKYBOX_TYPE::SPHERE);
				}

				if (ImGui::Selectable("Cube", eType == CUBE ? true : false, ImGuiSelectableFlags_Disabled && eType == CUBE))
				{
					pSkyBox->SetSkyBoxType(SKYBOX_TYPE::CUBE);
				}

				ImGui::EndCombo();
			}

			// Texture
			Ptr<CTexture> pCurTex = pSkyBox->GetSkyBoxTex();
			if (ParamUI::InputTexture(pCurTex, u8"ÅØ½ºÃ³"))
			{
				bool isCube = pCurTex->IsCubeMap();
				pSkyBox->SetSkyBoxType(isCube ? CUBE : SPHERE);
				pSkyBox->SetSkyBoxTexture(pCurTex);
			}
		}
		else
			ImGui::Image(m_NullTexture->GetSRV().Get(), { 100, 100 });
	}
}
