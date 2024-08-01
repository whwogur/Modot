#pragma once
#include "CShader.h"
class CComputeShader :
    public CShader
{
public:
    CComputeShader(UINT _ThreadPerGroupX, UINT _ThreadPerGroupY, UINT _ThreadPerGroupZ
        , const wstring& _ShaderRelativePath, const string& _FuncName);
    virtual ~CComputeShader() = default;

public:
    int Execute();

private:
    int CreateComputeShader(const wstring& _RelativePath, const string& _FuncName);

    virtual int Bind() = 0;
    virtual void CalcGroupNum() = 0;
    virtual void Clear() = 0;
protected:
    // 1 ���� �׷�� ������ ��
    const UINT                  m_ThreadPerGroupX;
    const UINT                  m_ThreadPerGroupY;
    const UINT                  m_ThreadPerGroupZ;

    // �׷� ��
    UINT                        m_GroupX;
    UINT                        m_GroupY;
    UINT                        m_GroupZ;
    // ��� ������ ���� �뵵
    tMtrlConst                  m_Const;

private:
    WRL::ComPtr<ID3DBlob>            m_CSBlob;
    WRL::ComPtr<ID3D11ComputeShader> m_CS;
};