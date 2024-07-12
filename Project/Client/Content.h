#pragma once
#include "EditorUI.h"
class Content :
    public EditorUI
{
public:
    Content();
    ~Content();

public:
    virtual void Update() override;

private:
    void RenewContent();

    void AssetClicked(DWORD_PTR _Param);

private:
    class TreeUI*   m_Tree;
};