#include "pch.h"

#include "CEditorMgr.h"
#include "CGameObjectEx.h"
#include <Engine/components.h>

#include "CEditorCameraScript.h"
#include <Engine/CRenderMgr.h>
#include "EditorLogger.h"
#include "EditorViewport.h"

void CEditorMgr::CreateEditorObject()
{
	m_Logger = std::make_unique<EditorLogger>();
	m_Viewport = std::make_unique<EditorViewport>();

	CGameObject* pEditorCamera = new CGameObjectEx;
	pEditorCamera->SetName(L"EditorCamera");
	pEditorCamera->AddComponent(new CTransform);
	pEditorCamera->AddComponent(new CCamera);
	pEditorCamera->AddComponent(new CEditorCameraScript);

	pEditorCamera->Camera()->SetLayerAll();
	pEditorCamera->Camera()->SetLayer(31, false);
	pEditorCamera->Camera()->SetFar(10000.f);
	pEditorCamera->Camera()->SetProjType(PERSPECTIVE);

	pEditorCamera->Transform()->SetRelativePos(Vec3(0.f, 0.f, -1000.f));
	m_vecEditorObject.push_back(pEditorCamera);

	CRenderMgr::GetInst()->Init(pEditorCamera->Camera());
	m_Logger->LogAdapterInfo();
}

void CEditorMgr::EditorObjectUpdate()
{
	for (const auto& EditorObj : m_vecEditorObject)
	{
		EditorObj->Tick();
	}

	for (const auto& EditorObj : m_vecEditorObject)
	{
		EditorObj->FinalTick();
	}

	DrawDebugLine(Vec3(0.f, 0.f, 0.f), Vec3(10000.f, 0.f, 0.f), Vec4(1.f, 0.f, 0.f, 1.f), 0.f, true);
	DrawDebugLine(Vec3(0.f, 0.f, 0.f), Vec3(0.f, 10000.f, 0.f), Vec4(0.f, 1.f, 0.f, 1.f), 0.f, true);
	DrawDebugLine(Vec3(0.f, 0.f, 0.f), Vec3(0.f, 0.f, 10000.f), Vec4(0.f, 0.f, 1.f, 1.f), 0.f, true);
}