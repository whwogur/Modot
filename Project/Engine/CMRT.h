#pragma once
#include "CEntity.h"
class CMRT :
    public CEntity
{
public:
    CLONE_DISABLE(CMRT);
    CMRT() = default;
    ~CMRT() = default;

public:
    void Create(int _RTCount, Ptr<CTexture>* _arrRT, Ptr<CTexture> _DSTex);
    void SetClearColor(Vec4* _Color, bool _Independent);
    void SetOM();

    Ptr<CTexture> GetRT(int _Idx) { return m_arrRT[_Idx]; }
    void Clear() { ClearRT(); ClearDS(); }
    void ClearRT();
    void ClearDS();

private:
    Ptr<CTexture>   m_arrRT[8] = {};
    Vec4            m_ClearColor[8] = {};
    int             m_RTCount = 0;
    Ptr<CTexture>   m_DSTex = nullptr;

    D3D11_VIEWPORT  m_Viewport;
};

