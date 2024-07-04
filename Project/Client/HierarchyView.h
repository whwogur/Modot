#pragma once
#include "EditorUI.h"

class HierarchyView :
    public EditorUI
{
public:
    HierarchyView();
    ~HierarchyView();
public:
    virtual void Update() override;

private:
    void RefreshLevel();
    void GameObjectClicked(DWORD_PTR _Param);
    void AddGameObject(class TreeNode* pNode, class CGameObject* _Object);
private:
    class TreeUI* m_Tree;
};
