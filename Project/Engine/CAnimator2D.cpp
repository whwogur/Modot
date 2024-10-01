#include "pch.h"
#include "CAnimator2D.h"


#include "CDevice.h"
#include "CConstBuffer.h"
#include "CTimeMgr.h"
#include "CAssetMgr.h"
#include "CAnimation.h"


CAnimator2D::CAnimator2D()
	: CComponent(COMPONENT_TYPE::ANIMATOR2D)
	, m_CurAnimation(nullptr)
	, m_CurFrmIdx(0)
{
}

CAnimator2D::CAnimator2D(const CAnimator2D& _Other)
	: CComponent(_Other)
	, m_vecAnimation(_Other.m_vecAnimation)
	, m_CurAnimation(_Other.m_CurAnimation)
	, m_CurFrmSprite(nullptr)
	, m_CurFrmIdx(0)
	, m_FPS(_Other.m_FPS)
	, m_AccTime(0.0f)
	, m_Repeat(_Other.m_Repeat)
	, m_Finish(false)
{
	if (m_CurAnimation != nullptr)
	{
		UINT AnimIdx = 0;
		for (; AnimIdx < (int)m_vecAnimation.size(); ++AnimIdx)
		{
			if (m_CurAnimation == m_vecAnimation[AnimIdx])
				break;
		}

		Play(AnimIdx, m_FPS, m_Repeat);
	}
}

void CAnimator2D::FinalTick()
{
	if (m_Finish)
	{
		if (false == m_Repeat)
			return;

		Reset();
	}

	if (nullptr != m_CurAnimation)
	{
		float MaxTime = 1.f / m_FPS;

		m_AccTime += DT;

		if (MaxTime < m_AccTime)
		{
			m_AccTime -= MaxTime;
			++m_CurFrmIdx;

			if (m_CurAnimation->GetMaxFrameCount() <= m_CurFrmIdx)
			{
				--m_CurFrmIdx;
				m_Finish = true;
			}
		}

		m_CurFrmSprite = m_CurAnimation->GetSprite(m_CurFrmIdx);
	}
}

void CAnimator2D::SaveToFile(FILE* _File)
{
	size_t AnimationCount = m_vecAnimation.size();
	fwrite(&AnimationCount, sizeof(size_t), 1, _File);
	for (auto animation : m_vecAnimation)
	{
		SaveAssetRef(animation, _File);
	}

	// 현재 재생중인 Animation 정보
	SaveAssetRef(m_CurAnimation, _File);
	SaveAssetRef(m_CurFrmSprite, _File);

	fwrite(&m_CurFrmIdx, sizeof(int), 1, _File);
	fwrite(&m_FPS, sizeof(float), 1, _File);
	fwrite(&m_AccTime, sizeof(float), 1, _File);
	fwrite(&m_Repeat, sizeof(bool), 1, _File);
}

void CAnimator2D::LoadFromFile(FILE* _File)
{
	size_t AnimationCount = 0;
	fread(&AnimationCount, sizeof(size_t), 1, _File);
	for (size_t i = 0; i < AnimationCount; ++i)
	{
		Ptr<CAnimation> pAnimation;
		LoadAssetRef(pAnimation, _File);
		m_vecAnimation.push_back(pAnimation);
	}

	LoadAssetRef(m_CurAnimation, _File);
	LoadAssetRef(m_CurFrmSprite, _File);

	fread(&m_CurFrmIdx, sizeof(int), 1, _File);
	fread(&m_FPS, sizeof(float), 1, _File);
	fread(&m_AccTime, sizeof(float), 1, _File);
	fread(&m_Repeat, sizeof(bool), 1, _File);
}

void CAnimator2D::AddAnimation(int _Idx, Ptr<CAnimation> _Animation)
{
	if (m_vecAnimation.size() <= _Idx)
	{
		m_vecAnimation.resize(_Idx + 1);
	}

	m_vecAnimation[_Idx] = _Animation;
}

void CAnimator2D::PushBackAnimation(Ptr<CAnimation> _Animation)
{
	m_vecAnimation.push_back(_Animation);
}

Ptr<CAnimation> CAnimator2D::FindAnimation(const wstring& _Key)
{
	for (size_t i = 0; i < m_vecAnimation.size(); ++i)
	{
		if (m_vecAnimation[i].Get() != nullptr && 
			m_vecAnimation[i]->GetKey() == _Key)
			return m_vecAnimation[i];
	}

	return nullptr;
}

void CAnimator2D::Play(int _AnimationIdx, float _FPS, bool _Repeat)
{
	m_CurAnimation = m_vecAnimation[_AnimationIdx];

	if (nullptr == m_CurAnimation)
	{
		return;
	}

	m_CurFrmIdx = 0;
	m_AccTime = 0.f;
	m_FPS = _FPS;
	m_Repeat = _Repeat;
}

void CAnimator2D::Play(const wstring& _AnimName, float _FPS, bool _Repeat)
{
	m_CurAnimation = FindAnimation(_AnimName);

	if (nullptr == m_CurAnimation)
	{
		return;
	}

	m_CurFrmIdx = 0;
	m_AccTime = 0.f;
	m_FPS = _FPS;
	m_Repeat = _Repeat;
}

void CAnimator2D::Reset()
{
	m_CurFrmIdx = 0;
	m_AccTime = 0.f;
	m_Finish = false;
}

void CAnimator2D::Bind()
{
	if (nullptr != m_CurFrmSprite)
	{
		// 현재 표시해야하는 Sprite 의 정보를 Sprite 전용 상수버퍼를 통해서 GPU 에 전달
		tSpriteInfo tInfo = {};

		tInfo.LeftTopUV = m_CurFrmSprite->GetLeftTopUV();
		tInfo.SliceUV = m_CurFrmSprite->GetSliceUV();
		tInfo.BackGroundUV = m_CurFrmSprite->GetBackgroundUV();
		tInfo.OffsetUV = m_CurFrmSprite->GetOffsetUV();
		tInfo.UseAnimator2D = 1;

		static CConstBuffer* CB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::SPRITE);

		CB->SetData(&tInfo);
		CB->Bind();

		// FlipBook Sprite 아틀라스 텍스쳐 전용 레지스터번호 t10 에 바인딩
		Ptr<CTexture> pAtlas = m_CurFrmSprite->GetAtlasTexture();
		pAtlas->Bind(10);
	}
	else
	{
		Clear();
	}
}

void CAnimator2D::Clear()
{
	tSpriteInfo tInfo = {};
	static CConstBuffer* CB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::SPRITE);
	CB->SetData(&tInfo);
	CB->Bind();
}