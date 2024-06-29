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

    void SetDomain(SHADER_DOMAIN _Domain) { m_Domain = _Domain; }
    void SetRSType(RS_TYPE _Type) { m_RSType = _Type; }
    void SetDSType(DS_TYPE _Type) { m_DSType = _Type; }
    void SetBSType(BS_TYPE _Type) { m_BSType = _Type; }

    SHADER_DOMAIN GetDomain() { return m_Domain; }
    RS_TYPE GetRSType() { return m_RSType; }
    DS_TYPE GetDSType() { return m_DSType; }
    BS_TYPE GetBSType() { return m_BSType; }

    void Bind();

private:
    WRL::ComPtr<ID3DBlob>			    m_VSBlob;
    WRL::ComPtr<ID3DBlob>			    m_PSBlob;

    WRL::ComPtr<ID3D11VertexShader>     m_VS;
    WRL::ComPtr<ID3D11PixelShader>	    m_PS;

    WRL::ComPtr<ID3D11InputLayout>      m_Layout;

    D3D11_PRIMITIVE_TOPOLOGY            m_Topology;
    RS_TYPE                             m_RSType;
    DS_TYPE                             m_DSType;
    BS_TYPE                             m_BSType;

    SHADER_DOMAIN                       m_Domain;
};

