#pragma once
#include "EditorUI.h"
class FileBrowser :
    public EditorUI
{
public:
    FileBrowser();
    ~FileBrowser() = default;

public:
    virtual void Init() override;
    virtual void Tick() override;
    virtual void Update() override;

private:
    path            m_CurrentDirectory;
    path            m_ContentPath;

    Ptr<CTexture>   m_DirectoryIcon;
    Ptr<CTexture>   m_FileIcon;

    float           m_RefreshPeriod = 1.5f;
    float           m_Acc = 0.0f;
};

