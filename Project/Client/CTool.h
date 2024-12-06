#pragma once
#include <ImGui/imgui.h>

class CTool
{
public:
	CTool() = default;
	virtual ~CTool() = default;

public:
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Activate() {};
	virtual void Deactivate() {};

public:
	void SetName(string _Name) { m_Name = _Name; }
	const string& GetName() { return m_Name; }

private:
	bool		m_Active = false;
	string		m_Name = {};
};

typedef void(CTool::* TOOLFUNC_0)(void);
typedef void(CTool::* TOOLFUNC_1)(DWORD_PTR);
typedef void(CTool::* TOOLFUNC_2)(DWORD_PTR, DWORD_PTR);
typedef void(CTool::* TOOLFUNC_3)(DWORD_PTR, DWORD_PTR, DWORD_PTR);