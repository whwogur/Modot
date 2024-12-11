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
    // g_int_0 : BoneCount, g_int_1 : Frame Index
    void SetBoneCount(int _iBoneCount) { m_Const.iArr[0] = _iBoneCount; }
    void SetFrameIndex(int _iFrameIdx) { m_Const.iArr[1] = _iFrameIdx; }
    void SetNextFrameIdx(int _iFrameIdx) { m_Const.iArr[2] = _iFrameIdx; }
    void SetFrameRatio(float _fFrameRatio) { m_Const.fArr[0] = _fFrameRatio; }
    void SetFrameDataBuffer(std::weak_ptr<CStructuredBuffer> _buffer) { m_pFrameDataBuffer = _buffer; }
    void SetOffsetMatBuffer(std::weak_ptr<CStructuredBuffer> _buffer) { m_pOffsetMatBuffer = _buffer; }
    void SetOutputBuffer(std::weak_ptr<CStructuredBuffer> _buffer) { m_pOutputBuffer = _buffer; }

public:
    virtual int Bind() override;
    virtual void CalcGroupNum() override;
    virtual void Clear() override;

private:
    std::weak_ptr<CStructuredBuffer>              m_pFrameDataBuffer;			// t16
    std::weak_ptr<CStructuredBuffer>              m_pOffsetMatBuffer;			// t17 
    std::weak_ptr<CStructuredBuffer>              m_pOutputBuffer;				// u0

};