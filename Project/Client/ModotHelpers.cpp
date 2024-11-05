#include "pch.h"
#include "ModotHelpers.h"

#include <ImGui/imgui_internal.h>

bool ModotHelpers::FilledSpinner(const char* _Label, float _Radius, int _Thickness, const ImU32& _Color)
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

bool ModotHelpers::BufferingBar(const char* _Label, float _Value, const ImVec2& _Size, const ImU32& _BG, const ImU32& _FG)
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

void ModotHelpers::DrawVec3Control(const string& label, Vec3& values, float resetValue, float columnWidth)
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

void ModotHelpers::LoadingIndicatorCircle(const char* _Label, const float _IndicatorRadius, const ImVec4& _MainColor, const ImVec4& _BackdropColor, const int _CircleCount, const float _Speed) {

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
		const auto growth = (((0.0f) >(std::sin(t * _Speed - i * degree_offset))) ? (0.0f) : (std::sin(t * _Speed - i * degree_offset)));
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