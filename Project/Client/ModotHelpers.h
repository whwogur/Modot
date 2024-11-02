#pragma once
#include "ImGui/imgui.h"

class ModotHelpers
{
public:
	static bool Spinner(const char* _Label, float _Radius, int _Thickness, const ImU32& _Color);
	static bool BufferingBar(const char* _Label, float _Value, const ImVec2& _Size, const ImU32& _BG, const ImU32& _FG);
	static void DrawVec3Control(const string& label, Vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
};