#pragma once


class CTaskMgr
	: public CSingleton<CTaskMgr>
{
	SINGLE(CTaskMgr)
private:
	vector<tTask>	m_vecTask;
	vector<CObj*>	m_GC;

public:
	void tick();
	void AddTask(const tTask& _Task) { m_vecTask.push_back(_Task); }

private:
	void ClearGC();
	void ExecuteTask();	
};

