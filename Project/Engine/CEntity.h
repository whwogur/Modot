#pragma once

class CEntity
{
public:
	void SetName(const wstring& _Name) { m_Name = _Name; }
	const wstring& GetName() { return m_Name; }
	UINT GetID() { return m_ID; }

public:
	CEntity();
	CEntity(const CEntity& _Other);
	virtual ~CEntity();

private:
	static UINT g_ID;

private:
	wstring		m_Name;
	const UINT	m_ID;
};
