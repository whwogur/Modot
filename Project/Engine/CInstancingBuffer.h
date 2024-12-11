#pragma once

#include "CStructuredBuffer.h"
#include "CTexture.h"
#include "CCopyBoneCS.h"

class CInstancingBuffer
	: public CSingleton<CInstancingBuffer>
{
public:
	CInstancingBuffer();
	~CInstancingBuffer();

public:
	void Init();
	void Clear()
	{
		m_vecData.clear();
		m_vecBoneMat.clear();
		m_AnimInstCount = 0;
	}
	void AddInstancingData(tInstancingData& _tData) { m_vecData.push_back(_tData); }

	UINT GetInstanceCount() { return (UINT)m_vecData.size(); }
	WRL::ComPtr<ID3D11Buffer> GetBuffer() { return m_InstancingBuffer; }
	void SetData();

	void AddInstancingBoneMat(CStructuredBuffer* _pBuffer);
	int GetAnimInstancingCount() { return m_AnimInstCount; };
	CStructuredBuffer* GetBoneBuffer() { return m_BoneBuffer; }

private:
	void Resize(UINT _iCount);


private:
	WRL::ComPtr<ID3D11Buffer>			m_InstancingBuffer;
	UINT								m_MaxCount;
	std::vector<tInstancingData>		m_vecData;

	// anim3D ¿ëµµ
	std::vector<CStructuredBuffer*>		m_vecBoneMat;
	int									m_AnimInstCount;
	CStructuredBuffer*					m_BoneBuffer;

	Ptr<CCopyBoneCS>					m_CopyShader;
};

