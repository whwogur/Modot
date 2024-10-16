#pragma once
#include "EditorUI.h"
class Content :
    public EditorUI
{
public:
    Content();
    ~Content() = default;

public:
    // Content 폴더에 있는 리소스(에셋) 들을 확인 및 로딩
    void Reload();
public:
    virtual void Update() override;
    virtual void Init() override;

private:
    void RenewContent();

    void AssetClicked(DWORD_PTR _Param);
    void FindAssetName(const wstring& _FolderPath, const wstring& _Filter);
    void LoadAsset(const path& _Path);

private:
    class TreeUI*   m_Tree;
    std::vector<wstring> m_vecAssetPath;
};