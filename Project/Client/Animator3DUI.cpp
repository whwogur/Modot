#include "pch.h"
#include "Animator3DUI.h"
#include <Engine/CGameObject.h>
#include <Engine/CAnimator3D.h>
#include "CEditorMgr.h"
Animator3DUI::Animator3DUI()
	: ComponentUI(COMPONENT_TYPE::ANIMATOR3D)
{
}

void Animator3DUI::Update()
{
	Title();
	if (!Collapsed())
	{
		CGameObject* targetObj = GetTargetObject();
		if (targetObj != nullptr)
		{
			UINT boneCnt = targetObj->Animator3D()->GetBoneCount();
			ImGui::TextColored(HEADER_2, "BoneCnt: ");
			ImGui::SameLine();
			ImGui::Text(std::to_string(boneCnt).c_str());

			const std::vector<tMTAnimClip>* clips = targetObj->Animator3D()->GetClips();

			if (clips != nullptr)
			{
				string previewVal(clips->front().strAnimName.begin(), clips->front().strAnimName.end());
				if (ImGui::BeginCombo("##AnimClipCombo", previewVal.c_str()))
				{
					for (const auto& clip : *clips)
					{
						if (ImGui::Selectable(ToString(clip.strAnimName).c_str()))
						{
							EDITOR_TRACE("StartFrame");
							EDITOR_TRACE(std::to_string(clip.iStartFrame));
							EDITOR_TRACE("FrameLength");
							EDITOR_TRACE(std::to_string(clip.iFrameLength));
							EDITOR_TRACE("EndFrame");
							EDITOR_TRACE(std::to_string(clip.iEndFrame));
							EDITOR_TRACE("TitleIndex");
							EDITOR_TRACE(std::to_string(clip.iMultipleClips));
							EDITOR_TRACE("TimeLength");
							EDITOR_TRACE(std::to_string(clip.dTimeLength));
							EDITOR_TRACE("StartTime");
							EDITOR_TRACE(std::to_string(clip.dStartTime));
							EDITOR_TRACE("EndTime");
							EDITOR_TRACE(std::to_string(clip.dEndTime));
						}
					}
					ImGui::EndCombo();
				}
			}
		}
	}
}
