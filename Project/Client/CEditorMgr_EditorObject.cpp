#include "pch.h"

#include "CEditorMgr.h"
#include "CGameObjectEx.h"
#include <Engine/components.h>

#include "CEditorCameraScript.h"
#include <Engine/CRenderMgr.h>
#include "Gizmo.h"
#include "EditorLogger.h"

void CEditorMgr::CreateEditorObject()
{
	m_Gizmo = std::make_shared<Gizmo>();
	m_Logger = std::make_unique<EditorLogger>();

	CGameObject* pEditorCamera = new CGameObjectEx;
	pEditorCamera->SetName(L"EditorCamera");
	pEditorCamera->AddComponent(new CTransform);
	pEditorCamera->AddComponent(new CCamera);
	pEditorCamera->AddComponent(new CEditorCameraScript);

	pEditorCamera->Camera()->SetLayerAll();
	pEditorCamera->Camera()->SetLayer(31, false);
	pEditorCamera->Camera()->SetFar(10000.f);
	pEditorCamera->Camera()->SetProjType(ORTHOGRAPHIC);

	m_vecEditorObject.push_back(pEditorCamera);

	CRenderMgr::GetInst()->Init(pEditorCamera->Camera());
	m_Gizmo->Init(pEditorCamera->Camera());
	m_Logger->LogAdapterInfo();
}

