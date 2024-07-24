#pragma once
class CLevelSaveLoad
{
public:
	static void SaveLevel(const wstring& _RelativePath, CLevel* _Level);
	static void SaveGameObject(FILE* _File, CGameObject* _Object);

	static class CLevel* LoadLevel(const wstring& _RelativePath);
	static class CGameObject* LoadGameObject(FILE* _File);
	static class CComponent* GetComponent(COMPONENT_TYPE _Type);
};