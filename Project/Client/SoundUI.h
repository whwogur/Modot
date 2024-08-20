#pragma once
#include "AssetUI.h"
class SoundUI :
    public AssetUI
{
public:
    SoundUI();
    ~SoundUI() = default;
public:
    virtual void Update() override;
    virtual void Activate() override;
    virtual void Deactivate() override;
private:
    Ptr<CSound>     m_Sound;
    char            m_SoundKey[64] = {};
    string          m_strKey;
    UINT            m_SoundLength;
    bool            m_Playing;
    int             m_ElapsedTime = 0;
};