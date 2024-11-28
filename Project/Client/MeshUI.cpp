#include "pch.h"
#include "MeshUI.h"

MeshUI::MeshUI()
	: AssetUI(ASSET_TYPE::MESH)
{
}

MeshUI::~MeshUI()
{
}

void MeshUI::Update()
{
	Title();
	// 재질 이름
	OutputAssetName();

	Ptr<CMesh> pMesh = (CMesh*)GetAsset().Get();

	if (pMesh != nullptr)
	{

		if (pMesh->IsAnimMesh())
		{
			const std::vector<tMTAnimClip>* vpAnimClip = pMesh->GetAnimClip();
			string temp((*vpAnimClip)[0].strAnimName.begin(), (*vpAnimClip)[0].strAnimName.end());
			ImGui::Text(temp.c_str());
		}
	}
}