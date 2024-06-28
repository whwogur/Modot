#pragma once
#include "CAsset.h"
#include "Ptr.h"
#include "CTexture.h"

class CMaterial :
    public CAsset
{
public:
    CMaterial();
    ~CMaterial();

public:
    template<typename T>
    void SetScalarParam(SCALAR_PARAM _Param, const T& _Data);
    void SetTexParam(TEX_PARAM _Param, Ptr<CTexture> _Tex) { m_arrTex[_Param] = _Tex; }

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
    tMtrlConst              m_Const;
    Ptr<CTexture>           m_arrTex[TEX_PARAM::END];
};

template<typename T>
void CMaterial::SetScalarParam(SCALAR_PARAM _Param, const T& _Data)
{
    if constexpr (std::is_same_v<T, int>) { m_Const.iArr[_Param] = _Data; }
    if constexpr (std::is_same_v<T, float>) { m_Const.fArr[_Param - FLOAT_0] = _Data; }
    if constexpr (std::is_same_v<T, Vec2>) { m_Const.v2Arr[_Param - VEC2_0] = _Data; }
    if constexpr (std::is_same_v<T, Vec4>) { m_Const.v4Arr[_Param - VEC4_0] = _Data; }
    if constexpr (std::is_same_v<T, Matrix>) { m_Const.matArr[_Param - MAT_0] = _Data; }
}