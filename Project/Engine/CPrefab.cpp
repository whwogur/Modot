#include "pch.h"
#include "CPrefab.h"

#include "CGameObject.h"
#include "CPathMgr.h"

OBJECT_SAVE  CPrefab::g_ObjectSaveFunc = nullptr;
OBJECT_LOAD  CPrefab::g_ObjectLoadFunc = nullptr;

CPrefab::CPrefab()
    : CAsset(ASSET_TYPE::PREFAB)
{
}

CGameObject* CPrefab::Instantiate()
{
    return m_OriginalObject->Clone();
}

int CPrefab::Load(const wstring& _RelativePath)
{
    const wstring& contentPath = CPathMgr::GetInst()->GetContentPath();
    wstring wstrPath(contentPath + _RelativePath);

    FILE* File = nullptr;
    _wfopen_s(&File, wstrPath.c_str(), L"rb");

    if (nullptr == File)
        return E_FAIL;

    m_OriginalObject = g_ObjectLoadFunc(File);

    fclose(File);

    return S_OK;
}

int CPrefab::Save(const wstring& _RelativePath)
{
    const wstring& contentPath = CPathMgr::GetInst()->GetContentPath();
    wstring wstrPath(contentPath + _RelativePath);

    FILE* File = nullptr;
    _wfopen_s(&File, wstrPath.c_str(), L"wb");

    if (nullptr == File)
        return E_FAIL;

    g_ObjectSaveFunc(File, m_OriginalObject);

    fclose(File);

    return S_OK;
}