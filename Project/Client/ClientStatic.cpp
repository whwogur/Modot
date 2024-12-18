#include "pch.h"
#include "ClientStatic.h"
#include <ImGui/imgui_internal.h>

#include <Engine/CPathMgr.h> // Open/saveFile
#include <shobjidl.h>
#include <shobjidl_core.h>
#include <comdef.h>
#include <shlobj_core.h>
#include "CEditorMgr.h"
namespace Modot
{
	bool ClientStatic::FilledSpinner(const char* _Label, float _Radius, int _Thickness, const ImU32& _Color)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(_Label);

		ImVec2 pos = window->DC.CursorPos;
		ImVec2 size((_Radius) * 2, (_Radius + style.FramePadding.y) * 2);

		const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
		ImGui::ItemSize(bb, style.FramePadding.y);
		if (!ImGui::ItemAdd(bb, id))
			return false;

		// Render
		window->DrawList->PathClear();

		int num_segments = 30;
		int start = (int)abs(ImSin((float)g.Time * 1.8f) * (num_segments - 5));

		const float a_min = IM_PI * 2.0f * ((float)start) / (float)num_segments;
		const float a_max = IM_PI * 2.0f * ((float)num_segments - 3) / (float)num_segments;

		const ImVec2 centre = ImVec2(pos.x + _Radius, pos.y + _Radius + style.FramePadding.y);

		for (int i = 0; i < num_segments; i++)
		{
			const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
			window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a + (float)g.Time * 8) * _Radius, centre.y + ImSin(a + (float)g.Time * 8) * _Radius));
		}

		window->DrawList->PathStroke(_Color, false, (float)_Thickness);

		return true;
	}

	bool ClientStatic::BufferingBar(const char* _Label, float _Value, const ImVec2& _Size, const ImU32& _BG, const ImU32& _FG)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(_Label);

		ImVec2 pos = window->DC.CursorPos;
		ImVec2 size = _Size;
		size.x -= style.FramePadding.x * 2;

		const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
		ImGui::ItemSize(bb, style.FramePadding.y);
		if (!ImGui::ItemAdd(bb, id))
			return false;

		// Render
		const float circleStart = size.x * 0.7f;
		const float circleEnd = size.x;
		const float circleWidth = circleEnd - circleStart;

		window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + circleStart, bb.Max.y), _BG);
		window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + circleStart * _Value, bb.Max.y), _FG);

		const float t = (float)g.Time;
		const float r = size.y / 2;
		const float speed = 1.5f;

		const float a = speed * 0;
		const float b = speed * 0.333f;
		const float c = speed * 0.666f;

		const float o1 = (circleWidth + r) * (t + a - speed * (int)((t + a) / speed)) / speed;
		const float o2 = (circleWidth + r) * (t + b - speed * (int)((t + b) / speed)) / speed;
		const float o3 = (circleWidth + r) * (t + c - speed * (int)((t + c) / speed)) / speed;

		window->DrawList->AddNgonFilled(ImVec2(pos.x + circleEnd - o1, bb.Min.y + r), r, _BG, 3);
		window->DrawList->AddNgonFilled(ImVec2(pos.x + circleEnd - o2, bb.Min.y + r), r, _BG, 4);
		window->DrawList->AddNgonFilled(ImVec2(pos.x + circleEnd - o3, bb.Min.y + r), r, _BG, 5);

		return true;
	}

	void ClientStatic::DrawVec3Control(const string& label, Vec3& values, float resetValue, float columnWidth)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.77f, 0.22f, 0.23f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.87f, 0.31f, 0.33f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.7f, 0.15f, 0.15f, 1.0f });
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.22f, 0.77f, 0.23f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.31f, 0.87f, 0.33f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.15f, 0.7f, 0.15f, 1.0f });
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.22f, 0.23f, 0.77f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.35f, 0.87f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.15f, 0.2f, 0.7f, 1.0f });
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::Columns(1);

		ImGui::PopID();
	}

	void ClientStatic::LoadingIndicatorCircle(const char* _Label, const float _IndicatorRadius, const ImVec4& _MainColor, const ImVec4& _BackdropColor, const int _CircleCount, const float _Speed) {

		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
		{
			return;
		}

		ImGuiContext& g = *GImGui;
		const ImGuiID id = window->GetID(_Label);

		const ImVec2 pos = window->DC.CursorPos;
		const float circle_radius = _IndicatorRadius / 15.0f;
		const float updated_indicator_radius = _IndicatorRadius - 4.0f * circle_radius;
		const ImRect bb(pos, ImVec2(pos.x + _IndicatorRadius * 2.0f, pos.y + _IndicatorRadius * 2.0f));
		ImGui::ItemSize(bb);
		if (!ImGui::ItemAdd(bb, id)) {
			return;
		}
		const float t = g.Time;
		const auto degree_offset = 2.0f * IM_PI / _CircleCount;
		for (int i = 0; i < _CircleCount; ++i) {
			const auto x = updated_indicator_radius * std::sin(degree_offset * i);
			const auto y = updated_indicator_radius * std::cos(degree_offset * i);
			const auto growth = (((0.0f) > (std::sin(t * _Speed - i * degree_offset))) ? (0.0f) : (std::sin(t * _Speed - i * degree_offset)));
			ImVec4 color;
			color.x = _MainColor.x * growth + _BackdropColor.x * (1.0f - growth);
			color.y = _MainColor.y * growth + _BackdropColor.y * (1.0f - growth);
			color.z = _MainColor.z * growth + _BackdropColor.z * (1.0f - growth);
			color.w = 1.0f;
			window->DrawList->AddCircleFilled(ImVec2(pos.x + _IndicatorRadius + x,
				pos.y + _IndicatorRadius - y),
				circle_radius + growth * circle_radius, ImGui::GetColorU32(color));
		}
	}

	bool ClientStatic::VSliderScalar(const char* label, const ImVec2& size, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags)
	{
		using namespace ImGui;
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);

		const ImVec2 label_size = CalcTextSize(label, NULL, true);
		const ImRect frame_bb(window->DC.CursorPos, ImVec2(window->DC.CursorPos.x + size.x, window->DC.CursorPos.y + size.y));
		const ImVec2 temp(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f);
		const ImRect bb(frame_bb.Min, ImVec2(frame_bb.Max.x + temp.x, frame_bb.Max.y + temp.y));

		ItemSize(bb, style.FramePadding.y);
		if (!ItemAdd(frame_bb, id))
			return false;

		// Default format string when passing NULL
		if (format == NULL)
			format = DataTypeGetInfo(data_type)->PrintFmt;

		const bool hovered = ItemHoverable(frame_bb, id, g.LastItemData.ItemFlags);
		const bool clicked = hovered && IsMouseClicked(0, ImGuiInputFlags_None, id);
		if (clicked || g.NavActivateId == id)
		{
			if (clicked)
				SetKeyOwner(ImGuiKey_MouseLeft, id);
			SetActiveID(id, window);
			SetFocusID(id, window);
			FocusWindow(window);
			g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Up) | (1 << ImGuiDir_Down);
		}

		// Draw frame
		const ImU32 frame_col = GetColorU32(g.ActiveId == id ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
		RenderNavHighlight(frame_bb, id);
		RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, g.Style.FrameRounding);

		// Slider behavior
		ImRect grab_bb;
		const bool value_changed = SliderBehavior(frame_bb, id, data_type, p_data, p_min, p_max, format, flags | ImGuiSliderFlags_Vertical, &grab_bb);
		if (value_changed)
			MarkItemEdited(id);

		// Render grab
		if (grab_bb.Max.y > grab_bb.Min.y)
			window->DrawList->AddRectFilled(grab_bb.Min, grab_bb.Max, GetColorU32(g.ActiveId == id ? ImVec4(0.f, 0.33f, 0.22f, 1.f) : ImVec4(0.1f, 0.53f, 0.32f, 1.f)), 2.f);

		return value_changed;
	}

	bool ClientStatic::VSliderFloat(const char* label, const ImVec2& size, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
	{
		return VSliderScalar(label, size, ImGuiDataType_Float, v, &v_min, &v_max, format, flags);
	}

	bool ClientStatic::HSliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags)
	{
		using namespace ImGui;
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		const float w = CalcItemWidth();

		const ImVec2 label_size = CalcTextSize(label, NULL, true);
		const ImRect frame_bb(window->DC.CursorPos, ImVec2(window->DC.CursorPos.x + w, window->DC.CursorPos.y + label_size.y + style.FramePadding.y));
		const ImRect total_bb(frame_bb.Min, ImVec2(frame_bb.Max.x + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), frame_bb.Max.y));

		const bool temp_input_allowed = (flags & ImGuiSliderFlags_NoInput) == 0;
		ItemSize(total_bb, style.FramePadding.y);
		if (!ItemAdd(total_bb, id, &frame_bb, temp_input_allowed ? ImGuiItemFlags_Inputable : 0))
			return false;

		// Default format string when passing NULL
		if (format == NULL)
			format = DataTypeGetInfo(data_type)->PrintFmt;

		const bool hovered = ItemHoverable(frame_bb, id, g.LastItemData.ItemFlags);
		bool temp_input_is_active = temp_input_allowed && TempInputIsActive(id);
		if (!temp_input_is_active)
		{
			// Tabbing or CTRL-clicking on Slider turns it into an input box
			const bool clicked = hovered && IsMouseClicked(0, ImGuiInputFlags_None, id);
			const bool make_active = (clicked || g.NavActivateId == id);
			if (make_active && clicked)
				SetKeyOwner(ImGuiKey_MouseLeft, id);
			if (make_active && temp_input_allowed)
				if ((clicked && g.IO.KeyCtrl) || (g.NavActivateId == id && (g.NavActivateFlags & ImGuiActivateFlags_PreferInput)))
					temp_input_is_active = true;

			if (make_active && !temp_input_is_active)
			{
				SetActiveID(id, window);
				SetFocusID(id, window);
				FocusWindow(window);
				g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
			}
		}

		if (temp_input_is_active)
		{
			// Only clamp CTRL+Click input when ImGuiSliderFlags_AlwaysClamp is set
			const bool is_clamp_input = (flags & ImGuiSliderFlags_AlwaysClamp) != 0;
			return TempInputScalar(frame_bb, id, label, data_type, p_data, format, is_clamp_input ? p_min : NULL, is_clamp_input ? p_max : NULL);
		}

		// Draw frame
		const ImU32 frame_col = GetColorU32(g.ActiveId == id ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
		RenderNavHighlight(frame_bb, id);
		RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, g.Style.FrameRounding);

		// Slider behavior
		ImRect grab_bb;
		const bool value_changed = SliderBehavior(frame_bb, id, data_type, p_data, p_min, p_max, format, flags, &grab_bb);
		if (value_changed)
			MarkItemEdited(id);

		// Render grab
		if (grab_bb.Max.x > grab_bb.Min.x)
			window->DrawList->AddRectFilled(grab_bb.Min, grab_bb.Max, GetColorU32(g.ActiveId == id ? ImVec4(0.f, 0.33f, 0.22f, 1.f) : ImVec4(0.1f, 0.53f, 0.32f, 1.f)), 3.141592f);

		return value_changed;
	}

	bool ClientStatic::HSliderFloat(const char* label, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
	{
		return HSliderScalar(label, ImGuiDataType_Float, v, &v_min, &v_max, format, flags);
	}

	bool ClientStatic::BeginTabItem(const char* label, ImGuiID* active_tab, ImVec4 _Color, bool* p_open, ImGuiTabItemFlags flags)
	{
		ImGuiID id = ImGui::GetID(label);
		bool wasActive = *active_tab == id;

		if (!wasActive)
			ImGui::PushStyleColor(ImGuiCol_Text, _Color);

		bool isActive = ImGui::BeginTabItem(label, p_open, flags);

		if (!wasActive)
			ImGui::PopStyleColor();

		if (isActive)
			*active_tab = id;

		return isActive;
	}


	void ClientStatic::ToggleButton(const char* str_id, bool* v)
	{
		ImVec4* colors = ImGui::GetStyle().Colors;
		ImVec2 p = ImGui::GetCursorScreenPos();
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		float height = ImGui::GetFrameHeight();
		float width = height * 1.55f;
		float radius = height * 0.50f;

		ImGui::InvisibleButton(str_id, ImVec2(width, height));
		if (ImGui::IsItemClicked()) *v = !*v;
		ImGuiContext& gg = *GImGui;
		float ANIM_SPEED = 0.085f;
		if (gg.LastActiveId == gg.CurrentWindow->GetID(str_id))// && g.LastActiveIdTimer < ANIM_SPEED)
			float t_anim = ImSaturate(gg.LastActiveIdTimer / ANIM_SPEED);
		if (ImGui::IsItemHovered())
			draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), ImGui::GetColorU32(*v ? colors[ImGuiCol_SliderGrab] : ImVec4(0.78f, 0.78f, 0.78f, 1.0f)), height * 0.5f);
		else
			draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), ImGui::GetColorU32(*v ? colors[ImGuiCol_SliderGrabActive] : ImVec4(0.85f, 0.85f, 0.85f, 1.0f)), height * 0.50f);
		draw_list->AddCircleFilled(ImVec2(p.x + radius + (*v ? 1 : 0) * (width - radius * 2.0f), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));
	}

	void ClientStatic::ToggleButton(const char* str_id, int* v)
	{
		ImVec4* colors = ImGui::GetStyle().Colors;
		ImVec2 p = ImGui::GetCursorScreenPos();
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		float height = ImGui::GetFrameHeight();
		float width = height * 1.55f;
		float radius = height * 0.50f;

		ImGui::InvisibleButton(str_id, ImVec2(width, height));
		if (ImGui::IsItemClicked()) *v ? *v = 0 : *v = 1;
		ImGuiContext& gg = *GImGui;
		float ANIM_SPEED = 0.085f;
		if (gg.LastActiveId == gg.CurrentWindow->GetID(str_id))// && g.LastActiveIdTimer < ANIM_SPEED)
			float t_anim = ImSaturate(gg.LastActiveIdTimer / ANIM_SPEED);
		if (ImGui::IsItemHovered())
			draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), ImGui::GetColorU32(*v ? colors[ImGuiCol_SliderGrab] : ImVec4(0.78f, 0.78f, 0.78f, 1.0f)), height * 0.5f);
		else
			draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), ImGui::GetColorU32(*v ? colors[ImGuiCol_SliderGrabActive] : ImVec4(0.85f, 0.85f, 0.85f, 1.0f)), height * 0.50f);
		draw_list->AddCircleFilled(ImVec2(p.x + radius + (*v ? 1 : 0) * (width - radius * 2.0f), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));
	}

	void ClientStatic::OpenFileDialog(std::vector<wstring>& _FilesName, const wstring& _RelativePath, const std::vector<std::pair<wstring, wstring>>& filter)
	{
		WRL::ComPtr<IFileOpenDialog> pFileDialog;
		HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, static_cast<void**>(&pFileDialog));
		if (FAILED(hr))
		{
			EDITOR_ERROR("Failed to create FileOpenDialog instance");
			return;
		}

		DWORD dwOptions;
		hr = pFileDialog->GetOptions(&dwOptions);
		if (SUCCEEDED(hr) && !(dwOptions & FOS_ALLOWMULTISELECT))
		{
			hr = pFileDialog->SetOptions(dwOptions | FOS_ALLOWMULTISELECT);
		}

		if (SUCCEEDED(hr))
		{
			wstring fullPath = CPathMgr::GetInst()->GetContentPath() + _RelativePath;
			WRL::ComPtr<IShellItem> pInitialDirItem;
			hr = SHCreateItemFromParsingName(fullPath.c_str(), NULL, IID_PPV_ARGS(&pInitialDirItem));
			if (SUCCEEDED(hr))
			{
				pFileDialog->SetFolder(pInitialDirItem.Get());
			}
		}

		std::vector<COMDLG_FILTERSPEC> fileTypes;
		for (const auto& f : filter)
		{
			fileTypes.push_back({ f.first.c_str(), f.second.c_str() });
		}
		hr = pFileDialog->SetFileTypes(static_cast<UINT>(fileTypes.size()), fileTypes.data());
		if (FAILED(hr))
		{
			EDITOR_ERROR("Failed to set file types");
			return;
		}

		hr = pFileDialog->Show(NULL);
		if (FAILED(hr))
		{
			if (hr != HRESULT_FROM_WIN32(ERROR_CANCELLED))
			{
				EDITOR_ERROR("Failed to open FileOpenDialog");
			}
			return;
		}

		WRL::ComPtr<IShellItemArray> pItems;
		hr = pFileDialog->GetResults(&pItems);
		if (SUCCEEDED(hr))
		{
			DWORD itemCount;
			hr = pItems->GetCount(&itemCount);
			if (SUCCEEDED(hr))
			{
				for (DWORD i = 0; i < itemCount; ++i)
				{
					WRL::ComPtr<IShellItem> pItem;
					hr = pItems->GetItemAt(i, &pItem);
					if (SUCCEEDED(hr))
					{
						PWSTR pszFilePath;
						hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
						if (SUCCEEDED(hr))
						{
							_FilesName.emplace_back(pszFilePath);
							CoTaskMemFree(pszFilePath);
						}
					}
				}
			}
		}
	}

	wstring ClientStatic::SaveFileDialog(const wstring& strRelativePath, const wchar_t* filter)
	{
		wchar_t szName[256] = {};
		OPENFILENAME ofn = {};

		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = CEngine::GetInst()->GetMainWnd();
		ofn.lpstrFile = szName;
		ofn.nMaxFile = sizeof(szName);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 0;
		ofn.lpstrFileTitle = nullptr;
		ofn.nMaxFileTitle = 0;

		wstring Path = CPathMgr::GetInst()->GetContentPath();
		Path += strRelativePath;
		ofn.lpstrInitialDir = Path.c_str();
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetSaveFileName(&ofn) == TRUE)
			return ofn.lpstrFile;

		return wstring();
	}

	bool ClientStatic::ColorPicker(const char* label, Vec4& _Color, ImGuiColorEditFlags _Flags, const Vec2& _Size)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiID id = window->GetID(label);
		const float default_size = ImGui::GetFrameHeight();
		const ImVec2 size(_Size.x == 0.0f ? default_size : _Size.x, _Size.y == 0.0f ? default_size : _Size.y);
		const ImRect bb(window->DC.CursorPos, { window->DC.CursorPos.x + size.x, window->DC.CursorPos.y + size.y });
		ImGui::ItemSize(bb, (size.y >= default_size) ? g.Style.FramePadding.y : 0.0f);
		if (!ImGui::ItemAdd(bb, id))
			return false;

		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

		if (_Flags & ImGuiColorEditFlags_NoAlpha)
			_Flags &= ~(ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaPreviewHalf);

		ImVec4 col_rgb{ _Color.x, _Color.y, _Color.z, _Color.w };
		if (_Flags & ImGuiColorEditFlags_InputHSV)
			ImGui::ColorConvertHSVtoRGB(col_rgb.x, col_rgb.y, col_rgb.z, col_rgb.x, col_rgb.y, col_rgb.z);

		ImVec4 col_rgb_without_alpha(col_rgb.x, col_rgb.y, col_rgb.z, 1.0f);
		float grid_step = ImMin(size.x, size.y) / 2.99f;
		float rounding = ImMin(g.Style.FrameRounding, grid_step * 0.5f);
		ImRect bb_inner = bb;
		float off = 0.0f;
		if ((_Flags & ImGuiColorEditFlags_NoBorder) == 0)
		{
			off = -0.75f; // The border (using Col_FrameBg) tends to look off when color is near-opaque and rounding is enabled. This offset seemed like a good middle ground to reduce those artifacts.
			bb_inner.Expand(off);
		}
		if ((_Flags & ImGuiColorEditFlags_AlphaPreviewHalf) && col_rgb.w < 1.0f)
		{
			float mid_x = IM_ROUND((bb_inner.Min.x + bb_inner.Max.x) * 0.5f);
			ImGui::RenderColorRectWithAlphaCheckerboard(window->DrawList, ImVec2(bb_inner.Min.x + grid_step, bb_inner.Min.y), bb_inner.Max, ImGui::GetColorU32(col_rgb), grid_step, ImVec2(-grid_step + off, off), rounding, ImDrawFlags_RoundCornersRight);
			window->DrawList->AddRectFilled(bb_inner.Min, ImVec2(mid_x, bb_inner.Max.y), ImGui::GetColorU32(col_rgb_without_alpha), rounding, ImDrawFlags_RoundCornersLeft);
		}
		else
		{
			// Because GetColorU32() multiplies by the global style Alpha and we don't want to display a checkerboard if the source code had no alpha
			ImVec4 col_source = (_Flags & ImGuiColorEditFlags_AlphaPreview) ? col_rgb : col_rgb_without_alpha;
			if (col_source.w < 1.0f)
				ImGui::RenderColorRectWithAlphaCheckerboard(window->DrawList, bb_inner.Min, bb_inner.Max, ImGui::GetColorU32(col_source), grid_step, ImVec2(off, off), rounding);
			else
				window->DrawList->AddRectFilled(bb_inner.Min, bb_inner.Max, ImGui::GetColorU32(col_source), rounding);
		}
		ImGui::RenderNavCursor(bb, id);
		if ((_Flags & ImGuiColorEditFlags_NoBorder) == 0)
		{
			if (g.Style.FrameBorderSize > 0.0f)
				ImGui::RenderFrameBorder(bb.Min, bb.Max, rounding);
			else
				window->DrawList->AddRect(bb.Min, bb.Max, ImGui::GetColorU32(ImGuiCol_FrameBg), rounding); // Color button are often in need of some sort of border
		}

		// Tooltip
		if (!(_Flags & ImGuiColorEditFlags_NoTooltip) && hovered && ImGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip))
			ImGui::ColorTooltip(label, &_Color.x, _Flags & (ImGuiColorEditFlags_InputMask_ | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaPreviewHalf));

		return pressed;
	}
}