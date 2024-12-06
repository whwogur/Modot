#pragma once
#include "CTool.h"
constexpr const float ANIMPREVIEW_SIZE = 350.f;
constexpr const char* staticString[] = { u8"Ω√¿€: ", u8"≥°: ", u8"CurrentFrame: " };
class CEditorCameraScript;
class CTransform;
struct tMTAnimClip;

class Animation3DEditor :
    public CTool
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
    //void RenderCardinalDirections();
private:
    CGameObject*                    m_Target = nullptr;
    int                             m_TargetOriginLayer = 0;
    CEditorCameraScript*            m_EditorCam = nullptr;
    tMTAnimClip                     m_TargetClip = {};

    std::vector<int32_t>            m_Frames = {};
    Matrix                          m_OriginalMatCam = {};
    int32_t                         m_CurrentFrameIdx = 0;

    uint32_t                        m_SelectedFramesCnt = 0;
    char m_CharBuffer[255] = {};
    //Vec3                  m_TargetFront, m_TargetRight = {};
};

