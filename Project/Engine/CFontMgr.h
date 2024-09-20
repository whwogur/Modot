#pragma once
#include "singleton.h"

#include <FontEngine/FW1FontWrapper.h>
#include <FontEngine/FW1CompileSettings.h>

#ifdef _DEBUG
#pragma comment(lib, "FontEngine/FW1FontWrapper_debug.lib")
#else
#pragma comment(lib, "FontEngine/FW1FontWrapper.lib"))
#endif

class CFontMgr :
    public CSingleton<CFontMgr>
{
    SINGLE(CFontMgr);
public:
    void Init();
    void DrawFont(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color);

private:
    WRL::ComPtr<IFW1Factory>        m_FW1Factory;
    WRL::ComPtr<IFW1FontWrapper>    m_FontWrapper;
};