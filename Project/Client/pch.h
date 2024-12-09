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
#define HEADER_3 { 1.0f, 1.0f, 0.33f, 1.0f }

constexpr static const float INDENT_1 = 120.f;
constexpr static const float INDENT_2 = 70.f;
constexpr static const float INDENT_3 = 40.f;