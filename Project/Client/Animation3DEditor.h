#pragma once
#include "EditorUI.h"
constexpr const int ANIMLAYER = 30;
constexpr const float ANIMPREVIEW_SIZE = 300.f;
class CEditorCameraScript;
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
    CGameObject*            m_Target = nullptr;
    CEditorCameraScript*    m_EditorCam = nullptr;

    Matrix                  m_OriginalMatCam = {};
    Matrix                  m_OriginalMatTarget = {};

    const Matrix            m_DesignatedMatCam  = { -0.3f, 0.f, 0.9f, 0.f, 
                                                    -0.06f, 1.f, -0.02f, 0.f, 
                                                    -1.f, -0.07f, -0.34f, 0.f, 
                                                    254.f, 184.f, 78.f, 1.f 
                                                  };

    const Matrix            m_DesignatedMatTarget = { 5, 0, 0, 0, 
                                                      0, 5, 0, 0,
                                                      0, 0, 5, 0,
                                                      0, 0, 0, 1 };
};

