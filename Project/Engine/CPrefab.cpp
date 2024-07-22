#include "pch.h"
#include "CPrefab.h"

#include "CGameObject.h"

CPrefab::CPrefab()
    : CAsset(ASSET_TYPE::PREFAB)
{
}

CGameObject* CPrefab::Instantiate()
{
    return m_OriginalObject->Clone();
}

int CPrefab::Load(const wstring& _FilePath)
{
    return 0;
}

int CPrefab::Save(const wstring& _FilePath)
{
    return 0;
}