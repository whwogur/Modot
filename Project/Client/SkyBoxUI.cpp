#include "pch.h"
#include "SkyBoxUI.h"
#include <Engine/CSkyBox.h>

SkyBoxUI::SkyBoxUI()
	: ComponentUI(COMPONENT_TYPE::SKYBOX)
{
}

void SkyBoxUI::Update()
{
	Title();
	if (!Collapsed())
	{
		CSkyBox* pSkyBox = GetTargetObject()->SkyBox();

		if (pSkyBox != nullptr)
		{
			Ptr<CTexture> pSkyboxTex = pSkyBox->GetSharedMtrl()->GetTexParam(TEX_PARAM::TEX_0);
			if (pSkyboxTex.Get() != nullptr)
			{
				ImGui::Image(pSkyboxTex->GetSRV().Get(), { 100, 100 });
			}
		}
	}
}
