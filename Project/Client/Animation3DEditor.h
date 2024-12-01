#pragma once
#include "EditorUI.h"
constexpr const int ANIMLAYER = 30;
constexpr const float ANIMPREVIEW_SIZE = 350.f;
class CEditorCameraScript;
class CTransform;
struct tMTAnimClip;

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
    void RenderPreview();
    //void RenderCardinalDirections();
private:
    CGameObject*            m_Target = nullptr;
    CEditorCameraScript*    m_EditorCam = nullptr;
    tMTAnimClip             m_TargetClip = {};

    std::vector<int32_t>       m_Frames = {};
    int32_t*                m_Selected = nullptr;
    Matrix                  m_OriginalMatCam = {};
    int32_t                     m_CurrentIdx = 0;
    //Vec3                    m_TargetFront, m_TargetRight = {};
};

