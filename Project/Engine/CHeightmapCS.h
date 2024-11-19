#pragma once
#include "CComputeShader.h"

class CStructuredBuffer;

class CHeightmapCS :
    public CComputeShader
{
public:
    CHeightmapCS();
    ~CHeightmapCS() = default;

public:
    void SetBrushPos(CStructuredBuffer* _Buffer) { m_RaycastOut = _Buffer;}
    void SetBrushScale(Vec2 _Scale) { m_BrushScale = _Scale; }
    void SetHeightMap(Ptr<CTexture> _HeightMap) { m_HeightMapTex = _HeightMap; }
    void SetBrushTex(Ptr<CTexture> _BrushTex) { m_BrushTex = _BrushTex; }
public:
    virtual int Bind() override;
    virtual void CalcGroupNum() override;
    virtual void Clear() override;

private:
    Ptr<CTexture>                       m_HeightMapTex;
    Ptr<CTexture>                       m_BrushTex;
    CStructuredBuffer*                  m_RaycastOut;
    Vec2                                m_BrushScale;
};

