#pragma once
#include "CComputeShader.h"
class CStructuredBuffer;

class CAnimation3DShader :
    public CComputeShader
{
public:
    CAnimation3DShader();
    ~CAnimation3DShader() = default;

public:
    virtual int Bind() override;
    virtual void CalcGroupNum() override;
    virtual void Clear() override;

public:
    // g_int_0 : BoneCnt 
    // g_int_1 : FrameIdx
    void SetBoneCount(int _iBoneCount) { m_Const.iArr[0] = _iBoneCount; }
    void SetFrameIndex(int _iFrameIdx) { m_Const.iArr[1] = _iFrameIdx; }
    void SetNextFrameIdx(int _iFrameIdx) { m_Const.iArr[2] = _iFrameIdx; }
    void SetFrameRatio(float _fFrameRatio) { m_Const.fArr[0] = _fFrameRatio; }
    void SetFrameDataBuffer(CStructuredBuffer* _buffer) { m_pFrameDataBuffer = _buffer; }
    void SetOffsetMatBuffer(CStructuredBuffer* _buffer) { m_pOffsetMatBuffer = _buffer; }
    void SetOutputBuffer(CStructuredBuffer* _buffer) { m_pOutputBuffer = _buffer; }

private:
    CStructuredBuffer* m_pFrameDataBuffer   = nullptr;	// t16
    CStructuredBuffer* m_pOffsetMatBuffer   = nullptr;	// t17 
    CStructuredBuffer* m_pOutputBuffer      = nullptr;	// u0
};