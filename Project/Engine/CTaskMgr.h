#pragma once


class CTaskMgr
	: public CSingleton<CTaskMgr>
{
	SINGLE(CTaskMgr)
public:
	void Tick();
	void AddTask(const tTask& _Task);
private:
	void ClearGC();
	void ExecuteTask();	

private:
	std::vector<tTask>			m_vecTask;
	std::vector<CGameObject*>	m_GC;
};

