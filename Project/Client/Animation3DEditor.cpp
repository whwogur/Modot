#include "pch.h"
#include "Animation3DEditor.h"
#include <Engine/CLevelMgr.h>
#include <Engine/CRenderMgr.h>

#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/CMesh.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CTransform.h>

#include <Engine/CScript.h>
#include "CEditorCameraScript.h"

#include "CEditorMgr.h"
#include <Modot_sequencer.h>
#include <numeric>
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
		RenderPreview();
		RenderSequencer();
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
	// ������ ī�޶�� ���� ����(�������)�� �����س��´�
	m_OriginalMatCam = m_EditorCam->Transform()->GetWorldMat();

	const Matrix& targetWM = _Target->Transform()->GetWorldMat();
	Vec3 desiredPos = GetOffsetPosition(targetWM, 300.f);
	Matrix lookAtWM = MakeLookAtWorldMatrix(targetWM, desiredPos);

	// ī�޶� ��ǥ ������ �ű��
	SetWorldPosition(m_EditorCam->Transform(), lookAtWM);

	// ����Ʈ�� ������ ���ϰ�
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
	SetWorldPosition(m_EditorCam->Transform(), m_OriginalMatCam);
	m_Target = nullptr;

	m_EditorCam->Camera()->SetLayerAll();
	m_EditorCam->Camera()->SetLayer(31, false);

	CEditorMgr::GetInst()->EnableViewport(true);

	memset(&m_OriginalMatCam, 0, sizeof(Matrix));
	memset(&m_TargetClip, 0, sizeof(tMTAnimClip));
}

void Animation3DEditor::SetWorldPosition(CTransform* _Transform, const Matrix& _Mat)
{
	_Transform->SetRelativeScale(ExtractScale(_Mat));
	_Transform->SetRelativeRotation(ExtractRotation(_Mat));
	_Transform->SetRelativePos(ExtractPosition(_Mat));
}

void Animation3DEditor::RenderSequencer()
{
	ImGui::Begin("Clips##3DAnimSequencer", 0, ImGuiWindowFlags_AlwaysAutoResize);

	std::vector<tMTAnimClip> a = *m_Target->Animator3D()->GetClips();
	
	if (!a.empty())
	{
		string test = u8"Ŭ�� ����: " + std::to_string(a.size());
		if (ImGui::BeginCombo("##Clips", test.c_str()))
		{
			for (const auto& clip : a)
			{
				const string strName(clip.strAnimName.begin(), clip.strAnimName.end());
				if (ImGui::Selectable(strName.c_str()))
				{
					m_TargetClip = clip;
					m_Frames.reserve(clip.iEndFrame);
					std::iota(m_Frames.begin(), m_Frames.end(), 0);
				}
			}
			ImGui::EndCombo();
		}
	}
	
	ImGui::SeparatorText(u8"Ŭ�� ����");

	if (m_TargetClip.dTimeLength)
	{
		static int frameIdx = m_Target->Animator3D()->GetFrameIdx();
		if (Modot::BeginModotSequencer(u8"�ִϸ��̼�", &frameIdx, &m_TargetClip.iStartFrame, &m_TargetClip.iEndFrame, {0, 0}))
		{
			if (Modot::BeginModotTimeline(u8"Ű������", m_Frames))
			{
				Modot::EndModotTimeLine();
			}

			if (Modot::BeginModotTimeline(u8"�׽�Ʈ", m_Frames))
			{
				Modot::EndModotTimeLine();
			}
			Modot::EndModotSequencer();
		}
	}
	ImGui::End();
}

void Animation3DEditor::RenderPreview()
{
	ImGui::Begin("Preview##Animation3D");

	Ptr<CTexture> rtTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"AlbedoTargetTex"); // TEMP
	if (rtTex != nullptr)
	{
		ImGui::Image(rtTex->GetSRV().Get(), { ANIMPREVIEW_SIZE, ANIMPREVIEW_SIZE }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 }, HEADER_2);
	}

	ImGui::End();
}