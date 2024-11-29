#pragma once
#include "EditorUI.h"
constexpr const int ANIMLAYER = 30;
constexpr const float ANIMPREVIEW_SIZE = 350.f;
class CEditorCameraScript;
class CTransform;

class Animation3DEditor :
    public EditorUI
{
public:
    Animation3DEditor();
    ~Animation3DEditor() = default;

public:
    virtual void Init() override;
    virtual void Update() override;

    virtual void Deactivate() override;
public:
    void SetTarget(CGameObject* _Target);
    void LetGoOfTarget();

private:
    void SetWorldPosition(CTransform* _Transform, const Matrix& _Mat);
    void RenderSequencer();
private:
    CGameObject*            m_Target = nullptr;
    CEditorCameraScript*    m_EditorCam = nullptr;

    Matrix                  m_OriginalMatCam = {};
};

