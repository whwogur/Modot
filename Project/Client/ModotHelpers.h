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

	static bool VSliderScalar(const char* label, const ImVec2& size, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags);
	static bool VSliderFloat(const char* label, const ImVec2& size, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags);

	static bool HSliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags);
	static bool HSliderFloat(const char* label, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags);
};