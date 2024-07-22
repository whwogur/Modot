#pragma once
#include "CAsset.h"
class CGameObject;

class CPrefab :
    public CAsset
{
public:
    CPrefab();
    ~CPrefab() = default;

public:
    void SetOriginalObject(CGameObject* _Object) { m_OriginalObject = _Object; }
    CGameObject* GetOriginalObject() { return m_OriginalObject; }
    CGameObject* Instantiate();

private:
    virtual int Load(const wstring& _FilePath) override;
    virtual int Save(const wstring& _FilePath) override;

private:
    CGameObject*        m_OriginalObject;
};