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
		//RenderCardinalDirections(); // �ص� ���߿�
		RenderPreview();
		RenderSequencer();
	}
}

void Animation3DEditor::Deactivate()
{
	if (m_Target != nullptr)
		LetGoOfTarget();

	EditorUI* menuUI = CEditorMgr::GetInst()->FindEditorUI("MainMenu"); // �ִϸ��̼� ������ ���� �� PLAY / PAUSE ������� �Ѱǵ�, ���߿� �ٸ�������� ���� ��
	if (menuUI != nullptr)
		menuUI->SetActive(true);
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

	// target Front Right �����ͼ� ����
	//m_TargetFront = m_Target->Transform()->GetRelativeDir(DIR::FRONT);
	//m_TargetRight = m_Target->Transform()->GetRelativeDir(DIR::RIGHT);

	m_EditorCam->Camera()->ClearLayerAll();
	m_EditorCam->Camera()->SetLayer(ANIMLAYER, true);

	EditorUI* menuUI = CEditorMgr::GetInst()->FindEditorUI("MainMenu");
	if (menuUI != nullptr)
		menuUI->SetActive(false);
}

void Animation3DEditor::LetGoOfTarget()
{
	EDITOR_TRACE("Letting go...");
	if (!m_Target->Animator3D()->IsPlayingAnim())
		m_Target->Animator3D()->ResumeAnimation(); // TEMP
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

	if (!m_TargetClip.dEndTime)
	{
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
						m_Frames.resize(clip.iEndFrame);
						std::iota(m_Frames.begin(), m_Frames.end(), 0);
					}
				}
				ImGui::EndCombo();
			}
		}
	}
	else
	{
		if (m_Target->Animator3D()->IsPlayingAnim())
		{
			if (ImGui::Button(ICON_FA_PAUSE"##Anim3DPause", { 22, 22 }))
			{
				m_Target->Animator3D()->PauseAnimation();
			}
		}
		else
		{
			if (ImGui::Button(ICON_FA_PLAY"##Anim3DPlay", { 22, 22 }))
			{
				m_Target->Animator3D()->ResumeAnimation();
			}
		}



		ImGui::SeparatorText(u8"Ŭ�� ����");
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

//void Animation3DEditor::RenderCardinalDirections()
//{
//	const Vec3& targetPos = m_Target->Transform()->GetRelativePosRef();
//	
//	DrawDebugLine(targetPos - m_TargetFront * 100.f, targetPos + m_TargetFront * 100.f, {1.f, 1.f, 1.f, 1.f}, 0.f, true);
//	DrawDebugLine(targetPos - m_TargetRight * 100.f, targetPos + m_TargetRight * 100.f, {1.f, 1.f, 1.f, 1.f}, 0.f, true);
//}
