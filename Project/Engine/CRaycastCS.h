#pragma once
#include "CComputeShader.h"
class CStructuredBuffer;
class CRaycastCS :
    public CComputeShader
{
public:
    CRaycastCS();
    ~CRaycastCS() = default;

public:
    void SetHeightMap(Ptr<CTexture> _HeightMap) { m_HeightMap = _HeightMap; }
    void SetRayInfo(const tRay& _ray) { m_Ray = _ray; }
    void SetFace(UINT _FaceX, UINT _FaceZ) { m_FaceX = _FaceX, m_FaceZ = _FaceZ; }
    void SetOutBuffer(std::shared_ptr<CStructuredBuffer> _Buffer) { m_OutBuffer = _Buffer; }

public:
    virtual int Bind() override;
    virtual void CalcGroupNum()override;
    virtual void Clear() override;

private:
    UINT                                m_FaceX;
    UINT                                m_FaceZ;
    tRay                                m_Ray;
    Ptr<CTexture>                       m_HeightMap;
    std::shared_ptr<CStructuredBuffer>  m_OutBuffer;
};