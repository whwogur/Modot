#pragma once

class CLevel;

class CLevelMgr
	: public CSingleton<CLevelMgr>
{
	SINGLE(CLevelMgr);
public:
	void Init();
	void Progress();
	void Render();

private:
	CLevel* m_CurLevel;
};