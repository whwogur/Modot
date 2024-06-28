#pragma once

class CLevel;

class CLevelMgr
	: public CSingleton<CLevelMgr>
{
	SINGLE(CLevelMgr);
public:
	CLevel* GetCurrentLevel() { return m_CurLevel; }

public:
	void Init();
	void Run();

private:
	CLevel* m_CurLevel;
};