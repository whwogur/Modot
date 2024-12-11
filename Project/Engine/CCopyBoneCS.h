#pragma once
#include "CComputeShader.h"
class CStructuredBuffer;

class CCopyBoneCS :
    public CComputeShader
{
public:
    CCopyBoneCS();
    ~CCopyBoneCS() = default;
public:
    // g_int_0 : BonCount
    void SetBoneCount(UINT _iBoneCount) { m_Const.iArr[0] = _iBoneCount; }
    // g_int_1 : RowIndex	
    void SetRowIndex(UINT _iRowIdx) { m_Const.iArr[1] = _iRowIdx; }
    void SetSourceBuffer(CStructuredBuffer* _pBuffer) { m_SrcBuffer = _pBuffer; }
    void SetDestBuffer(CStructuredBuffer* _pBuffer) { m_DestBuffer = _pBuffer; }

public:
    virtual int Bind() override;
    virtual void CalcGroupNum() override;
    virtual void Clear() override;
private:
    CStructuredBuffer* m_SrcBuffer;
    CStructuredBuffer* m_DestBuffer;
};

