#pragma once
#include "EditorUI.h"
class TreeUI;

class Content :
    public EditorUI
{
public:
    Content();
    ~Content() = default;

public:
    virtual void Update() override;

public:
    void DropExtern(DWORD_PTR _ExternData, DWORD_PTR _DropNode);
private:
    void BuildContentTree();

    void AssetClicked(DWORD_PTR _Param);

private:
    TreeUI*           m_Tree;
};