#pragma once
#include "CAsset.h"

class CMaterial :
    public CAsset
{
public:
    CMaterial();
    ~CMaterial();

public:
    void SetShader(Ptr<CGraphicShader> _Shader) { m_Shader = _Shader; }
    Ptr<CGraphicShader> GetShader() { return m_Shader; }
public:
    void Bind();

public:
    virtual int Load(const wstring& _FilePath) override { return S_OK; };
    virtual int Save(const wstring& _FilePath) override { return S_OK; };

private:
    Ptr<CGraphicShader>     m_Shader;
};