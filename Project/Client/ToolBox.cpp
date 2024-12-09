#include "pch.h"
#include "ToolBox.h"
#include "Animation2DEditor.h"
#include "CollisionCheck.h"
#include "FileBrowser.h"
#include "TexturePreview.h"
#include "MaterialEditor.h"
#include <ModotHelpers.h>

ToolBox::ToolBox()
{
}

void ToolBox::Init()
{
    // FileBrowser
    m_Tools[(UINT)TOOL_TYPE::FILEBROWSER] = std::make_unique<FileBrowser>();
    m_Tools[(UINT)TOOL_TYPE::FILEBROWSER]->SetName(ICON_FA_FOLDER" FileBrowser");
    m_Tools[(UINT)TOOL_TYPE::FILEBROWSER]->Init();

    // Animation2DEditor
    m_Tools[(UINT)TOOL_TYPE::ANIM2D] = std::make_unique<Animation2DEditor>();
    m_Tools[(UINT)TOOL_TYPE::ANIM2D]->SetName(ICON_FA_UNIVERSAL_ACCESS" 2D");
    m_Tools[(UINT)TOOL_TYPE::ANIM2D]->Init();

    m_Tools[(UINT)TOOL_TYPE::COLLISION] = std::make_unique<CollisionCheck>();
    m_Tools[(UINT)TOOL_TYPE::COLLISION]->SetName(ICON_FA_BOLT" Collision");
    m_Tools[(UINT)TOOL_TYPE::COLLISION]->Init();

    m_Tools[(UINT)TOOL_TYPE::MATERIAL] = std::make_unique<MaterialEditor>();
    m_Tools[(UINT)TOOL_TYPE::MATERIAL]->SetName(ICON_FA_SLIDERS" Material");
    m_Tools[(UINT)TOOL_TYPE::MATERIAL]->Init();

    // FileBrowser
    m_Tools[(UINT)TOOL_TYPE::TEXTUREPREVIEW] = std::make_unique<TexturePreview>();
    m_Tools[(UINT)TOOL_TYPE::TEXTUREPREVIEW]->SetName(ICON_FA_PICTURE_O" Preview");
    m_Tools[(UINT)TOOL_TYPE::TEXTUREPREVIEW]->Init();
}

void ToolBox::Update()
{
    ImVec4* colors = ImGui::GetStyle().Colors;
    ImGui::PushStyleColor(ImGuiCol_Tab, colors[ImGuiCol_ButtonHovered]);
    ImGui::BeginTabBar("##ToolbarTab");
    for (UINT i = 0; i < (UINT)TOOL_TYPE::END; ++i)
    {
        if (ImGui::BeginTabItem(m_Tools[i]->GetName().c_str()))
        {
            m_Tools[i]->Update();
            ImGui::EndTabItem();
        }
    }
    ImGui::EndTabBar();
    ImGui::PopStyleColor(1);
}