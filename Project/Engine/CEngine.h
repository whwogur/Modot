#pragma once

typedef void(*OBJECT_SAVE)(FILE*, class CGameObject*);
typedef CGameObject* (*OBJECT_LOAD)(FILE*);

class CEngine
	: public CSingleton<CEngine>
{	
	SINGLE(CEngine);
public:
	HWND GetMainWnd() { return m_hWnd; }
	FMOD::System* GetFMODSystem() { return m_FMODSystem; }
public:
	int Init(HWND _wnd, POINT _ptResolution, OBJECT_SAVE _SaveFunc, OBJECT_LOAD _Func);
	void Tick();
	void ChangeWindowScale(UINT _Width, UINT _Height);

private:
	HWND			m_hWnd;
	POINT			m_ptResolution;
	FMOD::System*	m_FMODSystem;
};

