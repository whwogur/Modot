#pragma once


#ifdef _DEBUG
#pragma comment(lib, "Engine\\Engine_D")
#else
#pragma comment(lib, "Engine\\Engine")
#endif

#include <Engine/global.h>
#include <Engine/CEngine.h>
#include "ImGui/IconsFontAwesome4.h"

// Scripts
#ifdef _DEBUG
#pragma comment(lib, "Scripts\\Scripts_D")
#else
#pragma comment(lib, "Scripts\\Scripts")
#endif

#define HEADER_1 { 0.0f, 0.45f, 0.67f, 1.0f }
#define HEADER_2 { 0.0f, 0.67f, 0.45f, 1.0f }

#ifdef _DEBUG
#define EDITOR_WARN(...) CEditorMgr::GetInst()->EditorWarn(__VA_ARGS__)
#define EDITOR_ERROR(...) CEditorMgr::GetInst()->EditorError(__VA_ARGS__)
#define EDITOR_TRACE(...) CEditorMgr::GetInst()->EditorTrace(__VA_ARGS__)
#else
#define EDITOR_WARN(...)
#define EDITOR_ERROR(...)
#define EDITOR_TRACE(...)
#endif