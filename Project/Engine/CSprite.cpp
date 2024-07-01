#include "pch.h"
#include "CSprite.h"

#include "CTexture.h"

CSprite::CSprite()
	: CAsset(ASSET_TYPE::SPRITE)
{
}

CSprite::~CSprite()
{
}

void CSprite::Create(Ptr<CTexture> _Atlas, Vec2 _LeftTopPixel, Vec2 _SlicePixel)
{
	m_Atlas = _Atlas;
	assert(m_Atlas);

	UINT width = m_Atlas->Width();
	UINT height = m_Atlas->Height();

	m_LeftTop = Vec2(_LeftTopPixel.x / (float)width, _LeftTopPixel.y / (float)height);
	m_Slice = Vec2(_SlicePixel.x / (float)width, _SlicePixel.y / (float)height);
}