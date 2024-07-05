#pragma once


class CTimeMgr
	: public CSingleton<CTimeMgr>
{
	SINGLE(CTimeMgr)
public:
	void Init();
	void Tick();

public:
	float GetDeltaTime() { return m_DeltaTime; }
	float GetTime() { return m_Time; }
	UINT GetFPS() { return m_FPS; }
	UINT GetFPSRecord() { return m_FPSRecord; }
	float GetEngineDeltaTime() { return m_E_DeltaTime; }
	float GetEngineTime() { return m_E_Time; }
	
private:
	LARGE_INTEGER	m_llCurCount;
	LARGE_INTEGER	m_llPrevCount;
	LARGE_INTEGER	m_llFrequency;

	UINT			m_FPS;
	UINT			m_FPSRecord = 0;

	float			m_DeltaTime;
	float			m_Time;
	float			m_E_DeltaTime;
	float			m_E_Time;
};

