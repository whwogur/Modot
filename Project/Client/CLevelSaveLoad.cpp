#include "pch.h"
#include "CLevelSaveLoad.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>
#include <Engine/CCollisionMgr.h>
#include <Scripts/CScriptMgr.h>

void CLevelSaveLoad::SaveLevel(const wstring& _RelativePath, CLevel* _Level)
{
	MD_ENGINE_ASSERT(_Level && _Level->GetState() == LEVEL_STATE::STOP, L"CLevelSaveLoad");

	const wstring& contentPath = CPathMgr::GetInst()->GetContentPath();
	wstring fullPath = contentPath + _RelativePath;

	FILE* File = nullptr;
	_wfopen_s(&File, fullPath.c_str(), L"wb");

	SaveWString(_Level->GetName(), File);

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = _Level->GetLayer(i);

		SaveWString(pLayer->GetName(), File);

		const std::vector<CGameObject*>& vecParents = pLayer->GetParentObjects();

		size_t count = vecParents.size();
		fwrite(&count, sizeof(size_t), 1, File);

		// GameObject 저장
		for (size_t i = 0; i < vecParents.size(); ++i)
		{
			SaveGameObject(File, vecParents[i]);
		}
	}

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		UINT collInfo = CCollisionMgr::GetInst()->GetCollision(i);
		fwrite(&collInfo, sizeof(UINT), 1, File);
	}

	fclose(File);
}

void CLevelSaveLoad::SaveGameObject(FILE* _File, CGameObject* _Object)
{
	SaveWString(_Object->GetName(), _File);

	UINT i = 0;
	for (; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		CComponent* pComponent = _Object->GetComponent((COMPONENT_TYPE)i);

		if (nullptr == pComponent)
			continue;

		COMPONENT_TYPE Type = pComponent->GetComponentType();
		fwrite(&Type, sizeof(COMPONENT_TYPE), 1, _File);

		pComponent->SaveToFile(_File);
	}

	// COMPONENT_TYPE::END 저장
	fwrite(&i, sizeof(COMPONENT_TYPE), 1, _File);

	const std::vector<CScript*> vecScripts = _Object->GetScripts();
	size_t ScriptCount = vecScripts.size();
	fwrite(&ScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ScriptCount; ++i)
	{
		wstring ScriptName = CScriptMgr::GetScriptName(vecScripts[i]);
		SaveWString(ScriptName, _File);

		vecScripts[i]->SaveToFile(_File);
	}

	// Child 정보 저장
	const std::vector<CGameObject*>& vecChild = _Object->GetChildren();
	size_t ChildCount = vecChild.size();
	fwrite(&ChildCount, sizeof(size_t), 1, _File);
	for (auto childObject : vecChild)
	{
		SaveGameObject(_File, childObject);
	}
}

CLevel* CLevelSaveLoad::LoadLevel(const wstring& _RelativePath)
{
	const wstring& contentPath = CPathMgr::GetInst()->GetContentPath();
	wstring fullPath = contentPath + _RelativePath;

	FILE* File = nullptr;
	_wfopen_s(&File, fullPath.c_str(), L"rb");

	// Level 생성
	CLevel* pNewLevel = new CLevel;

	// Level 이름 불러오기
	wstring LevelName;
	LoadWString(LevelName, File);
	pNewLevel->SetName(LevelName);

	// Level 안에 있는 Layer 정보 불러오기
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pNewLevel->GetLayer(i);

		// Layer 이름 불러오기
		wstring LayerName;
		LoadWString(LayerName, File);
		pLayer->SetName(LayerName);

		// Layer 가 보유한 ParentObject 개수 불어오기
		size_t count = 0;
		fread(&count, sizeof(size_t), 1, File);

		// GameObject 불러와서 Layer 에 집어넣기
		for (size_t i = 0; i < count; ++i)
		{
			CGameObject* pLoadedObject = LoadGameObject(File);
			pLayer->AddObject(pLoadedObject, false);
		}
	}

	CCollisionMgr::GetInst()->CollisionCheckClear();
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		UINT collInfo = 0;
		fread(&collInfo, sizeof(UINT), 1, File);
		CCollisionMgr::GetInst()->LoadCollisionInfo(i, collInfo);
	}

	fclose(File);

	return pNewLevel;
}

CGameObject* CLevelSaveLoad::LoadGameObject(FILE* _File)
{
	CGameObject* pObject = new CGameObject;

	// GameObject 의 이름 로드
	wstring Name;
	LoadWString(Name, _File);
	pObject->SetName(Name);

	// Component 정보 로드
	COMPONENT_TYPE Type = COMPONENT_TYPE::END;
	while (true)
	{
		// 저장되어있는 정보가 어떤 컴포넌트인지 확인
		fread(&Type, sizeof(COMPONENT_TYPE), 1, _File);

		// 읽은 타입 정보가 END 인 경우, 저장된 컴포넌트 정보의 끝이다.
		if (COMPONENT_TYPE::END == Type)
			break;

		// 저장된 타입에 맞는 컴포넌트를 생성 시키고, 저장할때랑 동일한 순서로 데이터를 읽는다.
		CComponent* pComponent = GetComponent(Type);

		// 생성된 컴포넌트를 오브젝트에 넣어준다.
		pObject->AddComponent(pComponent);

		// 저장당시의 정보를 읽어와서 복수한다.
		pComponent->LoadFromFile(_File);
	}

	// Script 정보
	size_t ScriptCount = 0;
	fread(&ScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ScriptCount; ++i)
	{
		// Script 의 이름 읽기
		wstring ScriptName;
		LoadWString(ScriptName, _File);

		// 읽은 Script 이름으로 이름에 해당하는 Script 생성
		CScript* pScript = CScriptMgr::GetScript(ScriptName);
		pScript->LoadFromFile(_File);

		pObject->AddComponent(pScript);
	}

	// Child 정보 로드
	size_t ChildCount = 0;
	fread(&ChildCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ChildCount; ++i)
	{
		CGameObject* pChildObject = LoadGameObject(_File);
		pObject->AddChild(pChildObject);
	}

	return pObject;
}

CComponent* CLevelSaveLoad::GetComponent(COMPONENT_TYPE _Type)
{
	switch (_Type)
	{
	case COMPONENT_TYPE::TRANSFORM:
		return new CTransform;

	case COMPONENT_TYPE::COLLIDER2D:
		return new CCollider2D;

	case COMPONENT_TYPE::COLLIDER3D:

		break;
	case COMPONENT_TYPE::LIGHT2D:
		return  new CLight2D;

	case COMPONENT_TYPE::LIGHT3D:
		return  new CLight3D;

	case COMPONENT_TYPE::ANIMATOR2D:
		return new CAnimator2D;

	case COMPONENT_TYPE::ANIMATOR3D:
		return new CAnimator3D;

	case COMPONENT_TYPE::TEXTRENDER:
		return new CTextRender;

	case COMPONENT_TYPE::NAVIGATION:
		return new CNavigation;

	case COMPONENT_TYPE::CAMERA:
		return  new CCamera;

	case COMPONENT_TYPE::MESHRENDER:
		return  new CMeshRender;

	case COMPONENT_TYPE::TILEMAP:
		return  new CTileMap;

	case COMPONENT_TYPE::PARTICLESYSTEM:
		return new CParticleSystem;

	case COMPONENT_TYPE::DECAL:
		return new CDecal;

	case COMPONENT_TYPE::SKYBOX:
		return new CSkyBox;

	case COMPONENT_TYPE::LANDSCAPE:
		return new CLandscape;
	}

	return nullptr;
}