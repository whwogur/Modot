#include "pch.h"
#include "CSound.h"
#include "CAssetMgr.h"

FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype
	, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype
	, void* commanddata1, void* commanddata2);

CSound::CSound()
	: CAsset(ASSET_TYPE::SOUND)
{
}

int CSound::Play(int _LoopCount, float _Volume, bool _AllowOverlap)
{
	if (_LoopCount <= -1)
	{
		assert(nullptr);
	}

	// ����ǰ� �ִ� ä���� �ִµ�, �ߺ������ ������� �ʾҴ� -> ��� ����
	if (!_AllowOverlap && !m_listChannel.empty())
	{
		return E_FAIL;
	}

	_LoopCount -= 1;

	FMOD::Channel* pChannel = nullptr;
	CAssetMgr::GetInst()->GetFMODSystem()->playSound(m_Sound, nullptr, false, &pChannel);

	// ��� ����
	if (nullptr == pChannel)
		return E_FAIL;

	pChannel->setVolume(_Volume);

	pChannel->setCallback(&CHANNEL_CALLBACK);
	pChannel->setUserData(this);

	pChannel->setMode(FMOD_LOOP_NORMAL);
	pChannel->setLoopCount(_LoopCount);

	m_listChannel.push_back(pChannel);

	int iIdx = -1;
	pChannel->getIndex(&iIdx);

	return iIdx;
}

void CSound::Stop()
{
	list<FMOD::Channel*>::iterator iter;

	while (!m_listChannel.empty())
	{
		iter = m_listChannel.begin();
		(*iter)->stop();
	}
}

void CSound::SetVolume(float _f, int _iChannelIdx)
{
	list<FMOD::Channel*>::iterator iter = m_listChannel.begin();

	int iIdx = -1;
	for (; iter != m_listChannel.end(); ++iter)
	{
		(*iter)->getIndex(&iIdx);
		if (_iChannelIdx == iIdx)
		{
			(*iter)->setVolume(_f);
			return;
		}
	}
}

void CSound::RemoveChannel(FMOD::Channel* _pTargetChannel)
{
	list<FMOD::Channel*>::iterator iter = m_listChannel.begin();
	for (; iter != m_listChannel.end(); ++iter)
	{
		if (*iter == _pTargetChannel)
		{
			m_listChannel.erase(iter);
			return;
		}
	}
}

int CSound::Load(const wstring& _RelativePath)
{
	const wstring& contPath = CPathMgr::GetInst()->GetContentPath();
	wstring fullpath = contPath + _RelativePath;
	string path(fullpath.begin(), fullpath.end());

	if (FMOD_OK != CAssetMgr::GetInst()->GetFMODSystem()->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &m_Sound))
	{
		assert(nullptr);
	}

	m_Sound->getLength(&m_SoundLength, FMOD_TIMEUNIT_MS);
	return S_OK;
}

int CSound::Save(const wstring& _RelativePath)
{
	return 0;
}

// =========
// Call Back
// =========
FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype
	, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype
	, void* commanddata1, void* commanddata2)
{
	FMOD::Channel* cppchannel = (FMOD::Channel*)channelcontrol;
	CSound* pSound = nullptr;

	switch (controltype)
	{
	case FMOD_CHANNELCONTROL_CALLBACK_END:
	{
		cppchannel->getUserData((void**)&pSound);
		pSound->RemoveChannel(cppchannel);
	}
	break;
	}

	return FMOD_OK;
}