#pragma once
#include "CComputeShader.h"
class CStructuredBuffer;
class CWeightMapCS :
    public CComputeShader
{
public:
    CWeightMapCS();
    ~CWeightMapCS() = default;
public:
    void SetBrushPos(CStructuredBuffer* _Buffer) { m_RaycastOut = _Buffer; }
    void SetBrushScale(Vec2 _Scale) { m_BrushScale = _Scale; }
    void SetWeightIdx(int _Idx) { m_WeightIdx = _Idx; }
    void SetWeightMapWidthHeight(UINT _Width, UINT _Height)
    {
        m_WeightMapWidth = (int)_Width;
        m_WeightMapHeight = (int)_Height;
    }
    void SetWeightMap(CStructuredBuffer* _WeightMap) { m_WeightMap = _WeightMap; }
    void SetBrushTex(Ptr<CTexture> _BrushTex) { m_BrushTex = _BrushTex; }

public:
    virtual int Bind() override;
    virtual void CalcGroupNum()override;
    virtual void Clear() override;


private:
    CStructuredBuffer*          m_WeightMap = nullptr;
    CStructuredBuffer*          m_RaycastOut = nullptr;
    Ptr<CTexture>               m_BrushTex;
    Vec2                        m_BrushScale;
    int                         m_WeightIdx = 0;
    int                         m_WeightMapWidth = 0;
    int                         m_WeightMapHeight = 0;
};