#pragma once
#include "CAsset.h"
#include "CEngine.h"

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

public:
    virtual int Load(const wstring& _RelativePath) override;
    virtual int Save(const wstring& _RelativePath) override;

private:
    static OBJECT_SAVE  g_ObjectSaveFunc;
    static OBJECT_LOAD  g_ObjectLoadFunc;

private:
    friend class CEngine;
    CGameObject*        m_OriginalObject;
};