#pragma once
#include "CAsset.h"
#include "Ptr.h"
#include "CTexture.h"
#include "CGraphicShader.h"

class CMaterial :
    public CAsset
{
public:
    CMaterial(bool _IsEngineAsset = false);
    ~CMaterial();
    CMaterial* Clone() { return new CMaterial(*this); }
public:
    template<typename T>
    void SetScalarParam(SCALAR_PARAM _Param, const T& _Data);
    void SetTexParam(TEX_PARAM _Param, Ptr<CTexture> _Tex) { m_arrTex[_Param] = _Tex; }

    void* GetScalarParam(SCALAR_PARAM _Param);
    Ptr<CTexture> GetTexParam(TEX_PARAM _Param) { return m_arrTex[(UINT)_Param]; }

    void SetMaterialCoefficient(Vec4 _vDiff, Vec4 _vSpec, Vec4 _vAmb, Vec4 _vEmis)
    {
        m_Const.mtrl.vDiff = _vDiff;
        m_Const.mtrl.vAmb = _vAmb;
        m_Const.mtrl.vSpec = _vSpec;
        m_Const.mtrl.vEmv = _vEmis;
    }

    void operator =(const CMaterial& _OtherMtrl)
    {
        SetName(_OtherMtrl.GetName());

        m_Const = _OtherMtrl.m_Const;
        for (UINT i = 0; i < (UINT)TEX_PARAM::END; ++i)
        {
            m_arrTex[i] = _OtherMtrl.m_arrTex[i];
        }
        m_Shader = _OtherMtrl.m_Shader;
    }

    void SetUsingAnim3D(bool _bTrue) { m_Const.arrAnimData[0] = (int)_bTrue; }
    void SetBoneCount(int _iBoneCount) { m_Const.arrAnimData[1] = _iBoneCount; }
public:
    void SetShader(Ptr<CGraphicShader> _Shader) { m_Shader = _Shader; }
    Ptr<CGraphicShader> GetShader() { return m_Shader; }
public:
    void Bind();
    
public:
    virtual int Load(const wstring& _RelativePath) override;
    virtual int Save(const wstring& _RelativePath) override;

private:
    Ptr<CGraphicShader>     m_Shader;
    tMtrlConst              m_Const;
    Ptr<CTexture>           m_arrTex[TEX_PARAM::END];
};

template<typename T>
void CMaterial::SetScalarParam(SCALAR_PARAM _Param, const T& _Data)
{
    if constexpr (std::is_same_v<T, int>) { m_Const.iArr[_Param] = _Data; return; }
    if constexpr (std::is_same_v<T, float>) { m_Const.fArr[_Param - FLOAT_0] = _Data; return; }
    if constexpr (std::is_same_v<T, Vec2>) { m_Const.v2Arr[_Param - VEC2_0] = _Data; return; }
    if constexpr (std::is_same_v<T, Vec4> || std::is_same_v<T, Vec3>) { m_Const.v4Arr[_Param - VEC4_0] = _Data; return; }
    if constexpr (std::is_same_v<T, Matrix>) { m_Const.matArr[_Param - MAT_0] = _Data; return; }

    assert(nullptr);
}