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

	bool IsDirty() { return m_Dirty; }
public:
	void SetLevelDirty();
	void Tick();

private:
	bool ChangeLevel(CLevel* _NextLevel);

private:
	friend class CTaskMgr;
	CLevel* m_CurLevel;
	bool	m_Dirty;
};