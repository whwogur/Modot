#pragma once
#include "CShader.h"

class CGraphicShader :
    public CShader
{
public:
    CGraphicShader();
    ~CGraphicShader();
public:
    int CreateVertexShader(const wstring& _RelativePath, const string& _FuncName);
    int CreatePixelShader(const wstring& _RelativePath, const string& _FuncName);

    virtual void Bind() override;

private:
    WRL::ComPtr<ID3DBlob>			    m_VSBlob;
    WRL::ComPtr<ID3DBlob>			    m_PSBlob;

    WRL::ComPtr<ID3D11VertexShader>     m_VS;
    WRL::ComPtr<ID3D11PixelShader>	    m_PS;

    WRL::ComPtr<ID3D11InputLayout>      m_Layout;

    D3D11_PRIMITIVE_TOPOLOGY            m_Topology;
};

