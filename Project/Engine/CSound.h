#pragma once
#include "CAsset.h"
class CSound :
    public CAsset
{
public:
    CSound();
    ~CSound();

public:
    // _LoopCount : 0 (무한반복),  _Volume : 0 ~ 1(Volume), _Overlap : 소리 중첩 가능여부
    int Play(int _LoopCount, float _Volume, bool _AllowOverlap);
    void RemoveChannel(FMOD::Channel* _pTargetChannel);
    void Stop();

    // 0 ~ 1
    void SetVolume(float _f, int _iChannelIdx);
    // MS
    UINT GetSoundLength() const { return m_SoundLength; }
public:
    virtual int Load(const wstring& _RelativePath) override;
    virtual int Save(const wstring& _RelativePath) override;


private:
    FMOD::Sound*                    m_Sound;        // Sound 버퍼
    list<FMOD::Channel*>	        m_listChannel;  // Sound 가 재생되고 있는 채널 리스트
    UINT                            m_SoundLength;
};

