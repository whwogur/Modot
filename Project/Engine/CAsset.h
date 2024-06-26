#pragma once
#include "CEntity.h"

class CAsset :
    public CEntity
{
    template<typename T>
    friend class Ptr;
    friend class CAssetMgr;
public:
    CAsset(ASSET_TYPE _Type);
    CAsset(const CAsset& _Other);
    virtual ~CAsset() = default;

public:
    ASSET_TYPE GetAssetType() { return m_Type; }
    const wstring& GetKey() { return m_Key; }
    const wstring& GetRelativePath() { return m_RelativePath; }
public:
    virtual void Binding() = 0;
    virtual CAsset* Clone() { return nullptr; }
    
private:
    void SetKey(const wstring& _Key) { m_Key = _Key; }
    void SetRelativePath(const wstring& _path) { m_RelativePath = _path; }
    void AddRef() { ++m_RefCount; }
    void Release()
    {
        --m_RefCount;
        if (m_RefCount <= 0)
        {
            delete this;
        }
    }

private:
    wstring             m_Key;
    wstring             m_RelativePath;
    const ASSET_TYPE    m_Type;
    int                 m_RefCount;
};

