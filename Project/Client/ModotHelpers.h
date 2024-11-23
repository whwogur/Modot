#pragma once
#include "ImGui/imgui.h"

class ModotHelpers
{
public:
	ModotHelpers() = delete;
	ModotHelpers(const ModotHelpers&) = delete;
	ModotHelpers& operator=(const ModotHelpers&) = delete;
public:
	static bool FilledSpinner(const char* _Label, float _Radius, int _Thickness, const ImU32& _Color);
	static bool BufferingBar(const char* _Label, float _Value, const ImVec2& _Size, const ImU32& _BG, const ImU32& _FG);
	static void DrawVec3Control(const string& _Label, Vec3& _Values, float _ResetValue = 0.0f, float _ColumnWidth = 100.0f);
	static void LoadingIndicatorCircle(const char* _Label, const float _IndicatorRadius, const ImVec4& _MainColor, const ImVec4& _BackdropColor, const int _CircleCount, const float _Speed);
};