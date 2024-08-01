#pragma once
#include "CComputeShader.h"

class CSetColorCS :
    public CComputeShader
{
public:
    CSetColorCS();
    ~CSetColorCS() = default;

public:
    void SetTargetTexture(Ptr<CTexture> _Tex) { m_TargetTex = _Tex; }
    void SetClearColor(Vec4 _Color) { m_ClearColor = _Color; }

private:
    virtual int Bind() override;
    virtual void CalcGroupNum() override;
    virtual void Clear() override;

private:
    Ptr<CTexture>       m_TargetTex;
    Vec4                m_ClearColor;
};