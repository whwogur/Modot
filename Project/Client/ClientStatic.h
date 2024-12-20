#pragma once
#include "ImGui/imgui.h"

namespace Modot
{
	class ClientStatic
	{
	public:
		ClientStatic() = delete;
		ClientStatic(const ClientStatic&) = delete;
		ClientStatic& operator=(const ClientStatic&) = delete;
	public:
		static bool FilledSpinner(const char* _Label, float _Radius, int _Thickness, const ImU32& _Color);
		static bool BufferingBar(const char* _Label, float _Value, const ImVec2& _Size, const ImU32& _BG, const ImU32& _FG);
		static void DrawVec3Control(const string& _Label, Vec3& _Values, float _ResetValue = 0.0f, float _ColumnWidth = 100.0f);
		static void LoadingIndicatorCircle(const char* _Label, const float _IndicatorRadius, const ImVec4& _MainColor, const ImVec4& _BackdropColor, const int _CircleCount, const float _Speed);

		static bool VSliderScalar(const char* label, const ImVec2& size, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags);
		static bool VSliderFloat(const char* label, const ImVec2& size, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags);

		static bool HSliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags);
		static bool HSliderFloat(const char* label, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags);

		static bool BeginTabItem(const char* label, ImGuiID* active_tab, ImVec4 _Color, bool* p_open = nullptr, ImGuiTabItemFlags flags = 0);

		static void ToggleButton(const char* str_id, bool* v);
		static void ToggleButton(const char* str_id, int* v);

		static void OpenFileDialog(std::vector<wstring>& _FilesName, const wstring& _RelativePath, const std::vector<std::pair<wstring, wstring>>& filter);
		static wstring SaveFileDialog(const wstring& strRelativePath, const wchar_t* filter);

		static bool ColorPicker(const char* label, Vec4& _Color, ImGuiColorEditFlags _Flags = 0, const Vec2& _Size = { 0, 0 });
		static bool ImageButton(const char* str_id, ImTextureID texture_id, const Vec2& image_size, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& bg_col, const ImVec4& tint_col, ImGuiButtonFlags flags);
	};
}