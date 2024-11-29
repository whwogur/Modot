#include "pch.h"
#include "Animation3DEditor.h"
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/CMesh.h>
#include <Engine/CAnimator3D.h>

Animation3DEditor::Animation3DEditor()
{
}

void Animation3DEditor::Update()
{
	if (m_Target == nullptr)
	{
		ImGui::TextColored({ 1.f, 0.f, 0.f, 1.f }, u8"선택된 오브젝트 없음");
		if (ImGui::BeginCombo("##3DAnimCombo", u8"선택"))
		{
			CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			for (UINT i = 0; i < MAX_LAYER; ++i)
			{
				CLayer* pLayer = pLevel->GetLayer(i);
				
				for (const auto& obj : pLayer->GetObjects())
				{
					if (obj->Animator3D())
					{
						const wstring& objName = obj->GetName();
						string strName(objName.begin(), objName.end());
						ImGui::Selectable(strName.c_str()); // TODO
					}
				}
			}
			
			ImGui::EndCombo();
		}
	}
}
