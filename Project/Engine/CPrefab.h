#pragma once
#include "CAsset.h"

class CPrefab :
    public CAsset
{
public:
    CPrefab();
    ~CPrefab();

public:
    void SetProtoObject(CGameObject* _Object) { m_ProtoObject = _Object; }
    CGameObject* Instantiate();

private:
    virtual int Load(const wstring& _FilePath) override;
    virtual int Save(const wstring& _FilePath) override;

private:
    class CGameObject* m_ProtoObject;
};