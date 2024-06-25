#pragma once


class CEngine
	: public CSingleton<CEngine>
{	
	SINGLE(CEngine);
public:
	HWND GetMainWnd() { return m_hWnd; }
public:
	int Init(HWND _wnd, POINT _ptResolution);
	void Run();
	void ChangeWindowScale(UINT _Width, UINT _Height);

private:
	HWND			m_hWnd;
	POINT			m_ptResolution;
};

