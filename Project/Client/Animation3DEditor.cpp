#include "pch.h"
#include "Animation3DEditor.h"
#include <Engine/CLevelMgr.h>
#include <Engine/CRenderMgr.h>

#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/CMesh.h>
#include <Engine/CAnimator3D.h>

#include <Engine/CScript.h>
#include "CEditorCameraScript.h"

#include "CEditorMgr.h"
Animation3DEditor::Animation3DEditor()
{
}

void Animation3DEditor::Init()
{
	const auto& vecScripts = CRenderMgr::GetInst()->GetEditorCamera()->GetOwner()->GetScriptsRef();
	MD_ENGINE_ASSERT(L"������ ī�޶� ��ũ��Ʈ �̾�", !vecScripts.empty());

	m_EditorCam = static_cast<CEditorCameraScript*>(vecScripts[0]); // ������ ī�޶�ũ��Ʈ �� ���ʿ� �ֵ���
}

void Animation3DEditor::Update()
{
	if (m_Target == nullptr)
	{
		ImGui::TextColored({ 1.f, 0.f, 0.f, 1.f }, u8"���õ� ������Ʈ ����");
		if (ImGui::BeginCombo("##3DAnimCombo", u8"����"))
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
						if (ImGui::Selectable(strName.c_str()))
						{
							SetTarget(obj);
						}
					}
				}
			}
			
			ImGui::EndCombo();
		}
	}
	else
	{
		Ptr<CTexture> rtTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"AlbedoTargetTex"); // TEMP
		if (rtTex != nullptr)
		{
			ImGui::Image(rtTex->GetSRV().Get(), { ANIMPREVIEW_SIZE, ANIMPREVIEW_SIZE });
		}
	}
}

void Animation3DEditor::Deactivate()
{
	if (m_Target != nullptr)
		LetGoOfTarget();
}

void Animation3DEditor::SetTarget(CGameObject* _Target)
{
	EDITOR_TRACE("Setting Target");
	m_Target = _Target;
	// ������ ī�޶�� Ÿ���� ���� ����(�������)�� �����س��´�
	m_OriginalMatCam = m_EditorCam->Transform()->GetWorldMat();
	m_OriginalMatTarget = _Target->Transform()->GetWorldMat();

	// ī�޶�� Ÿ���� ���� ��ġ�� �ű��
	m_EditorCam->Transform()->SetWorldMatrix(m_DesignatedMatCam);
	_Target->Transform()->SetWorldMatrix(m_DesignatedMatTarget);

	// ����Ʈ�� ������ ���ϰ� ����
	CEditorMgr::GetInst()->EnableViewport(false);
	// ���̾� ����
	_Target->DetachFromLayer();
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	pCurLevel->AddObject(ANIMLAYER, _Target, true);

	m_EditorCam->Camera()->ClearLayerAll();
	m_EditorCam->Camera()->SetLayer(ANIMLAYER, true);
}

void Animation3DEditor::LetGoOfTarget()
{
	EDITOR_TRACE("Letting go...");
	// ����
	m_EditorCam->Transform()->SetWorldMatrix(m_OriginalMatCam);
	m_Target->Transform()->SetWorldMatrix(m_OriginalMatTarget);
	m_Target = nullptr;

	m_EditorCam->Camera()->SetLayerAll();
	m_EditorCam->Camera()->SetLayer(31, false);

	CEditorMgr::GetInst()->EnableViewport(true);

	memset(&m_OriginalMatCam, 0, sizeof(Matrix));
	memset(&m_OriginalMatTarget, 0, sizeof(Matrix));
}
