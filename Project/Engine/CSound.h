#pragma once
#include "CAsset.h"
class CSound :
    public CAsset
{
public:
    CSound();
    ~CSound() = default;

public:
    // _LoopCount : 0 (���ѹݺ�),  _Volume : 0 ~ 1(Volume), _Overlap : �Ҹ� ��ø ���ɿ���
    int Play(int _LoopCount, float _Volume, bool _AllowOverlap);
    void RemoveChannel(FMOD::Channel* _pTargetChannel);
    void Stop();

    // 0 ~ 1
    void SetVolume(float _f, int _iChannelIdx);

public:
    virtual int Load(const wstring& _RelativePath) override;
    virtual int Save(const wstring& _RelativePath) override;


private:
    FMOD::Sound*                    m_Sound;        // Sound ����
    list<FMOD::Channel*>	        m_listChannel;  // Sound �� ����ǰ� �ִ� ä�� ����Ʈ
};

