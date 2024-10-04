#include "pch.h"
#include "CTextRender.h"
#include "CRenderMgr.h"
#include "CCamera.h"
#include "CDevice.h"
#include "CFontMgr.h"
#include "CTransform.h"
#include "CAssetMgr.h"
#include "CLevelMgr.h"
#include "CLevel.h"
CTextRender::CTextRender()
	: CRenderComponent(COMPONENT_TYPE::TEXTRENDER)
{
    SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
}

void CTextRender::FinalTick()
{
}

void CTextRender::Render()
{
    CCamera* pCam = nullptr;
    if (CLevelMgr::GetInst()->GetCurrentLevel()->GetState() == LEVEL_STATE::PLAY)
        pCam = CRenderMgr::GetInst()->GetCamera(m_CameraIdx);
    else
        pCam = CRenderMgr::GetInst()->GetEditorCamera();

    if (nullptr == pCam)
        return;

    // World Coordinate → Ndc Coordinate
    Vec3 WorldPos = Transform()->GetWorldPos();
    Matrix viewMat = pCam->GetcamViewRef();
    Matrix projMat = pCam->GetcamProjRef();

    Vec4 viewpos = XMVector3Transform(WorldPos, viewMat);
    Vec4 projPos = XMVector4Transform(viewpos, projMat);

    Vec3 NdcPos = Vec3();
    NdcPos.x = projPos.x / projPos.w;
    NdcPos.y = projPos.y / projPos.w;
    NdcPos.z = projPos.z / projPos.w;

    // 화면 밖인경우
    if (NdcPos.x < -1.0 || NdcPos.y < -1.0 || NdcPos.x > 1.0 || NdcPos.y > 1.0)
        return;

    // Ndc Coordinate → Screen Coordinate
    Vec2 Resolution = CDevice::GetInst()->GetResolution();

    float x_screen = (NdcPos.x + 1.0f) * 0.5f * Resolution.x;
    float y_screen = (1.0f - NdcPos.y) * 0.5f * Resolution.y;

    CFontMgr::GetInst()->DrawFont(m_Text.c_str(), x_screen, y_screen, m_Size, FONT_RGBA(m_Color.x, m_Color.y, m_Color.z, m_Color.w));
}

void CTextRender::SaveToFile(FILE* _File)
{
    SaveWString(m_Text, _File);
    fwrite(&m_Size, sizeof(float), 1, _File);
    fwrite(&m_Color, sizeof(Vec4), 1, _File);
    fwrite(&m_CameraIdx, sizeof(int), 1, _File);
}

void CTextRender::LoadFromFile(FILE* _File)
{
    LoadWString(m_Text, _File);
    fread(&m_Size, sizeof(float), 1, _File);
    fread(&m_Color, sizeof(Vec4), 1, _File);
    fread(&m_CameraIdx, sizeof(int), 1, _File);
}
