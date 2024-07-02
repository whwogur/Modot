#pragma once

class CLevel;
class CGameObject;

class CLevelMgr
	: public CSingleton<CLevelMgr>
{
	SINGLE(CLevelMgr);
public:
	CLevel* GetCurrentLevel() { return m_CurLevel; }
	CGameObject* FindObjectByName(const wstring& _Name);

public:
	void Init();
	void Run();

private:
	CLevel* m_CurLevel;
};