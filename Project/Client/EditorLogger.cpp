#include "pch.h"
#include "EditorLogger.h"

EditorLogger::EditorLogger()
{
	AutoScroll = true;
	Clear();
}

void EditorLogger::Clear()
{
	Buf.clear();
	LineOffsets.clear();
	LineOffsets.push_back(0);
}

void EditorLogger::Draw(const char* title, bool* p_open)
{
	if (!ImGui::Begin(title, p_open))
	{
		ImGui::End();
		return;
	}

	// Options menu
	if (ImGui::BeginPopup("LogOptions"))
	{
		ImGui::Checkbox("Auto scroll", &AutoScroll);
		if (ImGui::Checkbox("Combo Filter", &ComboBox))
			Filter.Clear();
		ImGui::EndPopup();
	}

	// Main window
	if (ImGui::Button(ICON_FA_COG))
		ImGui::OpenPopup("LogOptions");
	ImGui::SameLine();
	bool clear = ImGui::Button(ICON_FA_TRASH);
	ImGui::SetItemTooltip(u8"로그 삭제");
	ImGui::SameLine();
	bool copy = ImGui::Button(ICON_FA_CLONE);
	ImGui::SetItemTooltip(u8"로그 내용 복사");
	ImGui::SameLine();
	if (ComboBox)
	{
		ImGui::SetNextItemWidth(100);
		if (ImGui::BeginCombo(ICON_FA_FILTER, "Filter", ImGuiComboFlags_NoArrowButton))
		{
			if (ImGui::Selectable("INFO"))
			{
				Filter.InputBuf[0] = 'I';
				Filter.InputBuf[1] = 'N';
				Filter.InputBuf[2] = 'F';
				Filter.InputBuf[3] = 'O';
				Filter.InputBuf[4] = ']';
				Filter.Build();
			}

			if (ImGui::Selectable("TRACE"))
			{
				Filter.InputBuf[0] = 'T';
				Filter.InputBuf[1] = 'R';
				Filter.InputBuf[2] = 'A';
				Filter.InputBuf[3] = 'C';
				Filter.InputBuf[4] = 'E';
				Filter.Build();
			}

			if (ImGui::Selectable("WARN"))
			{
				Filter.InputBuf[0] = 'W';
				Filter.InputBuf[1] = 'A';
				Filter.InputBuf[2] = 'R';
				Filter.InputBuf[3] = 'N';
				Filter.InputBuf[4] = ']';
				Filter.Build();
			}

			if (ImGui::Selectable("ERROR"))
			{
				Filter.InputBuf[0] = 'E';
				Filter.InputBuf[1] = 'R';
				Filter.InputBuf[2] = 'R';
				Filter.InputBuf[3] = 'O';
				Filter.InputBuf[4] = 'R';
				Filter.Build();
			}
			ImGui::EndCombo();
		}
		ImGui::SetItemTooltip(u8"필터");
	}
	else
	{
		Filter.Draw(ICON_FA_FILTER, -100.0f);
	}

	ImGui::SameLine(); 
	if (!ComboBox && ImGui::Button("Clear"))
	{
		Filter.Clear();
	}
	ImGui::SetItemTooltip(u8"필터 초기화");
	
	ImGui::Separator();

	if (ImGui::BeginChild("scrolling", ImVec2(0, 0), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar))
	{
		if (clear)
			Clear();
		if (copy)
			ImGui::LogToClipboard();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		const char* buf = Buf.begin();
		const char* buf_end = Buf.end();
		if (Filter.IsActive())
		{
			// In this example we don't use the clipper when Filter is enabled.
			// This is because we don't have random access to the result of our filter.
			// A real application processing logs with ten of thousands of entries may want to store the result of
			// search/filter.. especially if the filtering function is not trivial (e.g. reg-exp).
			for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
			{
				const char* line_start = buf + LineOffsets[line_no];
				const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
				if (Filter.PassFilter(line_start, line_end))
					ImGui::TextUnformatted(line_start, line_end);
			}
		}
		else
		{
			// The simplest and easy way to display the entire buffer:
			//   ImGui::TextUnformatted(buf_begin, buf_end);
			// And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward
			// to skip non-visible lines. Here we instead demonstrate using the clipper to only process lines that are
			// within the visible area.
			// If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them
			// on your side is recommended. Using ImGuiListClipper requires
			// - A) random access into your data
			// - B) items all being the  same height,
			// both of which we can handle since we have an array pointing to the beginning of each line of text.
			// When using the filter (in the block of code above) we don't have random access into the data to display
			// anymore, which is why we don't use the clipper. Storing or skimming through the search result would make
			// it possible (and would be recommended if you want to search through tens of thousands of entries).
			ImGuiListClipper clipper;
			clipper.Begin(LineOffsets.Size);
			while (clipper.Step())
			{
				for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
				{
					const char* line_start = buf + LineOffsets[line_no];
					const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
					ImGui::TextUnformatted(line_start, line_end);
				}
			}
			clipper.End();
		}
		ImGui::PopStyleVar();

		// Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
		// Using a scrollbar or mouse-wheel will take away from the bottom edge.
		if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.0f);
	}
	ImGui::EndChild();
	ImGui::End();
}

void EditorLogger::LogAdapterInfo()
{
	IDXGIFactory* factory = nullptr;
	IDXGIAdapter* adapter = nullptr;
	DXGI_ADAPTER_DESC adapterDesc;

	CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);

	factory->EnumAdapters(0, &adapter);

	adapter->GetDesc(&adapterDesc);

	char videoCardDescription[128];
	std::string vendor, major, minor, release, build;
	LARGE_INTEGER driverVersion;

	wcstombs_s(NULL, videoCardDescription, 128, adapterDesc.Description, 128);

	if (adapterDesc.VendorId == 0x10DE)
		vendor = "NVIDIA Corporation";
	else if (adapterDesc.VendorId == 0x1002)
		vendor = "AMD";
	else if (adapterDesc.VendorId == 0x8086)
		vendor = "Intel";
	else if (adapterDesc.VendorId == 0x1414)
		vendor = "Microsoft";
	else
		vendor = "Unknown vendor!";

	adapter->CheckInterfaceSupport(__uuidof(IDXGIDevice), &driverVersion);

	major = std::to_string(HIWORD(driverVersion.HighPart));
	minor = std::to_string(LOWORD(driverVersion.HighPart));
	release = std::to_string(HIWORD(driverVersion.LowPart));
	build = std::to_string(LOWORD(driverVersion.LowPart));

	AddLog("[INFO] DirectX Information\n");
	AddLog("[INFO] Vendor : %s\n", vendor.c_str());
	AddLog("[INFO] Renderer : %s\n", videoCardDescription);
	AddLog("[INFO] Version : %s.%s.%s.%s\n", major.c_str(), minor.c_str(), release.c_str(), build.c_str());
}