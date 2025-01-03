#include "pch.h"
#include "Inspector.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>

#include "TransformUI.h"
#include "Collider2DUI.h"
#include "Light2DUI.h"
#include "Light3DUI.h"
#include "CameraUI.h"
#include "MeshRenderUI.h"
#include "Animator2DUI.h"
#include "Animator3DUI.h"
#include "TileMapUI.h"
#include "ParticleSystemUI.h"
#include "NavigationUI.h"
#include "TextRenderUI.h"
#include "SkyBoxUI.h"
#include "DecalUI.h"
#include "LandscapeUI.h"

#include "MeshUI.h"
#include "MeshDataUI.h"
#include "TextureUI.h"
#include "PrefabUI.h"
#include "TextureUI.h"
#include "MaterialUI.h"
#include "SpriteUI.h"
#include "AnimationUI.h"
#include "SoundUI.h"
#include "GraphicShaderUI.h"
#include "ComputeShaderUI.h"

#include "ScriptUI.h"

void Inspector::Init()
{
	CreateComponentUI();

	CreateAssetUI();


	if (nullptr == m_TargetObject)
	{
		SetTargetObject(CLevelMgr::GetInst()->FindObjectByName(L"EditorCamera"));
		return;
	}
}

void Inspector::CreateComponentUI()
{
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM] = new TransformUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM]->SetName("TransformUI");
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM]);

	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D] = new Collider2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D]->SetName("Collider2DUI");
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D]);

	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D] = new Light2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D]->SetName("Light2DUI");
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D]);

	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT3D] = new Light3DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT3D]->SetName("Light3DUI");
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT3D]);

	m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA] = new CameraUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA]->SetName("CameraUI");
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA]);

	m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D] = new Animator2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D]->SetName("Animator2DUI");
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D]);

	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER] = new MeshRenderUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER]->SetName("MeshRenderUI");
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER]);

	m_arrComUI[(UINT)COMPONENT_TYPE::TILEMAP] = new TileMapUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::TILEMAP]->SetName("TileMapUI");
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::TILEMAP]);

	m_arrComUI[(UINT)COMPONENT_TYPE::PARTICLESYSTEM] = new ParticleSystemUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::PARTICLESYSTEM]->SetName("ParticleSystemUI");
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::PARTICLESYSTEM]);

	m_arrComUI[(UINT)COMPONENT_TYPE::NAVIGATION] = new NavigationUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::NAVIGATION]->SetName("NavigationUI");
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::NAVIGATION]);

	m_arrComUI[(UINT)COMPONENT_TYPE::TEXTRENDER] = new TextRenderUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::TEXTRENDER]->SetName("TextRenderUI");
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::TEXTRENDER]);

	m_arrComUI[(UINT)COMPONENT_TYPE::SKYBOX] = new SkyBoxUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::SKYBOX]->SetName("SkyBoxUI");
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::SKYBOX]);

	m_arrComUI[(UINT)COMPONENT_TYPE::DECAL] = new DecalUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::DECAL]->SetName("DecalUI");
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::DECAL]);

	m_arrComUI[(UINT)COMPONENT_TYPE::LANDSCAPE] = new LandscapeUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::LANDSCAPE]->Init();
	m_arrComUI[(UINT)COMPONENT_TYPE::LANDSCAPE]->SetName("LandscapeUI");
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::LANDSCAPE]);

	m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR3D] = new Animator3DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR3D]->SetName("Animator3DUI");
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR3D]);
}

void Inspector::CreateScriptUI(UINT _Count)
{
	for (UINT i = 0; i < _Count; ++i)
	{
		ScriptUI* pScriptUI = new ScriptUI;

		char szScriptUIName[255] = {};
		sprintf_s(szScriptUIName, 255, "ScriptUI##%d", (int)m_vecScriptUI.size());
		pScriptUI->SetName(szScriptUIName);

		AddChild(pScriptUI);
		m_vecScriptUI.push_back(pScriptUI);
	}
}

void Inspector::CreateAssetUI()
{
	AssetUI* UI = nullptr;

	UI = new MeshUI;
	UI->SetName("MeshUI");
	AddChild(UI);
	m_arrAssetUI[(UINT)ASSET_TYPE::MESH] = UI;

	UI = new MeshDataUI;
	UI->SetName("MeshDataUI");
	AddChild(UI);
	m_arrAssetUI[(UINT)ASSET_TYPE::MESH_DATA] = UI;

	UI = new MaterialUI;
	UI->SetName("MaterialUI");
	AddChild(UI);
	m_arrAssetUI[(UINT)ASSET_TYPE::MATERIAL] = UI;

	UI = new PrefabUI;
	UI->SetName("PrefabUI");
	AddChild(UI);
	m_arrAssetUI[(UINT)ASSET_TYPE::PREFAB] = UI;

	UI = new TextureUI;
	UI->SetName("TextureUI");
	AddChild(UI);
	m_arrAssetUI[(UINT)ASSET_TYPE::TEXTURE] = UI;

	UI = new SpriteUI;
	UI->SetName("SpriteUI");
	AddChild(UI);
	m_arrAssetUI[(UINT)ASSET_TYPE::SPRITE] = UI;

	UI = new AnimationUI;
	UI->SetName("AnimationUI");
	AddChild(UI);
	m_arrAssetUI[(UINT)ASSET_TYPE::ANIMATION] = UI;

	UI = new SoundUI;
	UI->SetName("SoundUI");
	AddChild(UI);
	m_arrAssetUI[(UINT)ASSET_TYPE::SOUND] = UI;

	UI = new GraphicShaderUI;
	UI->SetName("GraphicShaderUI");
	AddChild(UI);
	m_arrAssetUI[(UINT)ASSET_TYPE::GRAPHIC_SHADER] = UI;

	UI = new ComputeShaderUI;
	UI->SetName("ComputeShaderUI");
	AddChild(UI);
	m_arrAssetUI[(UINT)ASSET_TYPE::COMPUTE_SHADER] = UI;
}