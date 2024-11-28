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
private:
    void RenewContent();

    void AssetClicked(DWORD_PTR _Param);

private:
    TreeUI*           m_Tree;
};