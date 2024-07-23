#include "pch.h"
#include "CLevelSaveLoad.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>

#include <Scripts/CScriptMgr.h>

void CLevelSaveLoad::SaveLevel(const wstring& _FilePath, CLevel* _Level)
{
	MD_ENGINE_ASSERT(_Level, L"CLevelSaveLoad - 레벨 없음");

	FILE* File = nullptr;
	_wfopen_s(&File, _FilePath.c_str(), L"wb");

	SaveWString(_Level->GetName(), File);

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = _Level->GetLayer(i);

		SaveWString(pLayer->GetName(), File);

		const vector<CGameObject*>& vecParents = pLayer->GetParentObjects();

		size_t count = vecParents.size();
		fwrite(&count, sizeof(size_t), 1, File);

		// GameObject 저장
		for (size_t i = 0; i < vecParents.size(); ++i)
		{
			SaveGameObject(File, vecParents[i]);
		}
	}

	fclose(File);
}

void CLevelSaveLoad::SaveGameObject(FILE* _File, CGameObject* _Object)
{
	SaveWString(_Object->GetName(), _File);

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		CComponent* pComponent = _Object->GetComponent((COMPONENT_TYPE)i);

		if (nullptr == pComponent)
			continue;

		COMPONENT_TYPE Type = pComponent->GetComponentType();
		fwrite(&Type, sizeof(COMPONENT_TYPE), 1, _File);

		pComponent->SaveToFile(_File);
	}

	const vector<CScript*> vecScripts = _Object->GetScripts();
	size_t ScriptCount = vecScripts.size();
	fwrite(&ScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ScriptCount; ++i)
	{
		wstring ScriptName = CScriptMgr::GetScriptName(vecScripts[i]);
		SaveWString(ScriptName, _File);

		vecScripts[i]->SaveToFile(_File);
	}

	// Child 정보 저장

}








CLevel* CLevelSaveLoad::LoadLevel(const wstring& _FilePath)
{
	FILE* File = nullptr;
	_wfopen_s(&File, _FilePath.c_str(), L"rb");

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


	return pObject;
}