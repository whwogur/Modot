#pragma once
#include "ImGui/imgui.h"
constexpr const char* LOG_CATEGORY[3] = { "WARN", "ERROR", "TRACE" };
class EditorLogger
{
public:
	EditorLogger();
	~EditorLogger() = default;

private:
	void Clear();
	void AddLog(const char* fmt, ...) IM_FMTARGS(2)
	{
		int old_size = Buf.size();
		va_list args;
		va_start(args, fmt);
		Buf.appendfv(fmt, args);
		va_end(args);
		for (int new_size = Buf.size(); old_size < new_size; old_size++)
			if (Buf[old_size] == '\n')
				LineOffsets.push_back(old_size + 1);
	}
	void Draw(const char* title, bool* p_open = 0);

private:
	void LogAdapterInfo();
private:
	friend class CEditorMgr;
	ImGuiTextBuffer     Buf;
	ImGuiTextFilter     Filter;
	ImVector<int>       LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
	bool                AutoScroll;  // Keep scrolling if already at the bottom.
};