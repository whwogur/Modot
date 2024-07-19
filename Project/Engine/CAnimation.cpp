#include "pch.h"
#include "CAnimation.h"
#include "CAssetMgr.h"

CAnimation::CAnimation()
	: CAsset(ASSET_TYPE::ANIMATION)
{
}

CAnimation::CAnimation(const CAnimation& _Other)
	: CAsset(_Other)
{
	for (auto sprite : _Other.m_vecSprite)
	{
		m_vecSprite.push_back((CSprite*)sprite->Clone());
	}
}

void CAnimation::FinalTick()
{
}

// ����� ����
int CAnimation::Save(const wstring& _RelativePath)
{
	SetRelativePath(_RelativePath);

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _RelativePath;

	FILE* File = nullptr;
	_wfopen_s(&File, strFilePath.c_str(), L"wb");

	if (nullptr == File)
		return E_FAIL;

	size_t SpriteCount = m_vecSprite.size();
	fwrite(&SpriteCount, sizeof(size_t), 1, File);

	for (size_t i = 0; i < SpriteCount; ++i)
	{
		SaveAssetRef(m_vecSprite[i], File);
	}

	fclose(File);

	return S_OK;
}

// ����� ����
int CAnimation::Load(const wstring& _RelativePath)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _RelativePath;

	if (GetRelativePath().length() < 1)
		SetRelativePath(_RelativePath);

	FILE* File = nullptr;
	_wfopen_s(&File, strFilePath.c_str(), L"rb");
	if (nullptr == File)
		return E_FAIL;

	size_t SpriteCount = 0;
	fread(&SpriteCount, sizeof(size_t), 1, File);
	m_vecSprite.resize(SpriteCount);

	for (size_t i = 0; i < SpriteCount; ++i)
	{
		LoadAssetRef(m_vecSprite[i], File);
	}

	fclose(File);

	return S_OK;
}