#pragma once
#include "EditorUI.h"
class CTool;
enum class TOOL_TYPE
{
    ANIM2D,
    COLLISION,
    MATERIAL,
    TEXTUREPREVIEW,
    FILEBROWSER,

    END
};

class ToolBox :
    public EditorUI
{
public:
    ToolBox();
    ~ToolBox() = default;

public:
    virtual void Init() override;
    virtual void Update() override;

private:
    std::unique_ptr<CTool>          m_Tools[(UINT)TOOL_TYPE::END] = {nullptr,};
};

