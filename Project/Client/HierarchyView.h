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
    void GameObjectAddChild(DWORD_PTR _Param1, DWORD_PTR _Param2);
    void DropExtern(DWORD_PTR _OuterData, DWORD_PTR _DropNode);
private:
    friend class Inspector;
    class TreeUI* m_Tree;
};
