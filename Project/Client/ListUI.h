#pragma once
#include "EditorUI.h"

class ListUI :
    public EditorUI
{
public:
    ListUI();
    ~ListUI();

public:
    void AddItem(const string& _item);
    void AddList(const std::vector<string>& _list);
    void AddDelegate(EditorUI* _UI, DELEGATE_1 _Func)
    {
        m_UI = _UI;
        m_MemFunc = _Func;
    }

    string GetSelectName() { return m_SelectedName; }
public:
    virtual void Activate() override;
    virtual void Deactivate() override;
    virtual void Update() override;

private:
    std::vector<string>  m_vecList;
    int             m_SelectedIdx;
    string          m_SelectedName;
    EditorUI*       m_UI;
    DELEGATE_1      m_MemFunc;
};
