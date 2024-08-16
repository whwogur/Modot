#pragma once
#include "EditorUI.h"
class Content :
    public EditorUI
{
public:
    Content();
    ~Content() = default;

public:
    // Content ������ �ִ� ���ҽ�(����) ���� Ȯ�� �� �ε�
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
    vector<wstring> m_vecAssetPath;
};