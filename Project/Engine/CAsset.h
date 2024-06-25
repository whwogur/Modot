#pragma once
#include "CEntity.h"

class CAsset :
    public CEntity
{
public:
    CAsset(ASSET_TYPE _Type);
    ~CAsset();

    friend class CAssetMgr;
public:
    const wstring& GetKey() { return m_Key; }
    const wstring& GetRelativePath() { return m_RelativePath; }
    ASSET_TYPE GetAssetType() { return m_Type; }
public:
    virtual void Binding() = 0;
    virtual CAsset* Clone() { return nullptr; }

private:
    void SetKey(const wstring& _Key) { m_Key = _Key; }
    void SetRelativePath(const wstring& _path) { m_RelativePath = _path; }
private:
    wstring             m_Key;
    wstring             m_RelativePath;
    const ASSET_TYPE    m_Type;
};

