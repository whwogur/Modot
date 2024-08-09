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

		const vector<CGameObject*>& vecParents = pLayer->GetParentObjects();

		size_t count = vecParents.size();
		fwrite(&count, sizeof(size_t), 1, File);

		// GameObject ����
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

	// COMPONENT_TYPE::END ����
	fwrite(&i, sizeof(COMPONENT_TYPE), 1, _File);

	const vector<CScript*> vecScripts = _Object->GetScripts();
	size_t ScriptCount = vecScripts.size();
	fwrite(&ScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ScriptCount; ++i)
	{
		wstring ScriptName = CScriptMgr::GetScriptName(vecScripts[i]);
		SaveWString(ScriptName, _File);

		vecScripts[i]->SaveToFile(_File);
	}

	// Child ���� ����
	const vector<CGameObject*>& vecChild = _Object->GetChildren();
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

	// Level ����
	CLevel* pNewLevel = new CLevel;

	// Level �̸� �ҷ�����
	wstring LevelName;
	LoadWString(LevelName, File);
	pNewLevel->SetName(LevelName);

	// Level �ȿ� �ִ� Layer ���� �ҷ�����
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pNewLevel->GetLayer(i);

		// Layer �̸� �ҷ�����
		wstring LayerName;
		LoadWString(LayerName, File);
		pLayer->SetName(LayerName);

		// Layer �� ������ ParentObject ���� �Ҿ����
		size_t count = 0;
		fread(&count, sizeof(size_t), 1, File);

		// GameObject �ҷ��ͼ� Layer �� ����ֱ�
		for (size_t i = 0; i < count; ++i)
		{
			CGameObject* pLoadedObject = LoadGameObject(File);
			pLayer->AddObject(pLoadedObject, false);
		}
	}

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

	// GameObject �� �̸� �ε�
	wstring Name;
	LoadWString(Name, _File);
	pObject->SetName(Name);

	// Component ���� �ε�
	COMPONENT_TYPE Type = COMPONENT_TYPE::END;
	while (true)
	{
		// ����Ǿ��ִ� ������ � ������Ʈ���� Ȯ��
		fread(&Type, sizeof(COMPONENT_TYPE), 1, _File);

		// ���� Ÿ�� ������ END �� ���, ����� ������Ʈ ������ ���̴�.
		if (COMPONENT_TYPE::END == Type)
			break;

		// ����� Ÿ�Կ� �´� ������Ʈ�� ���� ��Ű��, �����Ҷ��� ������ ������ �����͸� �д´�.
		CComponent* pComponent = GetComponent(Type);

		// ������ ������Ʈ�� ������Ʈ�� �־��ش�.
		pObject->AddComponent(pComponent);

		// �������� ������ �о�ͼ� �����Ѵ�.
		pComponent->LoadFromFile(_File);
	}

	// Script ����
	size_t ScriptCount = 0;
	fread(&ScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ScriptCount; ++i)
	{
		// Script �� �̸� �б�
		wstring ScriptName;
		LoadWString(ScriptName, _File);

		// ���� Script �̸����� �̸��� �ش��ϴ� Script ����
		CScript* pScript = CScriptMgr::GetScript(ScriptName);
		pScript->LoadFromFile(_File);

		pObject->AddComponent(pScript);
	}

	// Child ���� �ε�
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
		break;

	case COMPONENT_TYPE::ANIMATOR2D:
		return new CAnimator2D;

	case COMPONENT_TYPE::ANIMATOR3D:
		break;

	case COMPONENT_TYPE::STATE_MACHINE:
		break;

	case COMPONENT_TYPE::RIGIDBODY:
		return new CRigidBody;
		break;

	case COMPONENT_TYPE::CAMERA:
		return  new CCamera;

	case COMPONENT_TYPE::MESHRENDER:
		return  new CMeshRender;

	case COMPONENT_TYPE::TILEMAP:
		return  new CTileMap;

	case COMPONENT_TYPE::PARTICLESYSTEM:
		break;

	case COMPONENT_TYPE::DECAl:
		break;

	case COMPONENT_TYPE::SKYBOX:
		break;

	case COMPONENT_TYPE::LANDSCAPE:
		break;
	}

	return nullptr;
}