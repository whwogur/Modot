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
}