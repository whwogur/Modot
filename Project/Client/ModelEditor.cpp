#include "pch.h"
#include "ModelEditor.h"
#include "CGameObjectEx.h"
#include "CCamera.h"
#include "CTransform.h"
#include "CEditorCameraScript.h"

ModelEditor::ModelEditor()
{
}

ModelEditor::~ModelEditor()
{
}

void ModelEditor::Update()
{
}

void ModelEditor::LateUpdate()
{
}

void ModelEditor::Init()
{
    //=============
    // 에디터 카메라
    //=============
    m_ModelEditorCam = std::make_unique<CGameObjectEx>();
    m_ModelEditorCam->SetName(L"ModelEditorCamera");
    m_ModelEditorCam->AddComponent(new CTransform);
    m_ModelEditorCam->AddComponent(new CCamera);
    m_ModelEditorCam->AddComponent(new CEditorCameraScript);

    m_ModelEditorCam->Transform()->SetRelativeScale(1, 1, 1);
    m_ModelEditorCam->Camera()->SetLayerAll();
    m_ModelEditorCam->Camera()->SetLayer(31, false);
    m_ModelEditorCam->Camera()->SetFar(10000.f);
    m_ModelEditorCam->Camera()->SetProjType(PERSPECTIVE);
    m_ModelEditorCam->Camera()->SetFrustumDebug(false);

    m_ModelEditorCam->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
}
