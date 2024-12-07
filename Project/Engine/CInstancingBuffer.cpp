#include "pch.h"
#include "CInstancingBuffer.h"
#include "CDevice.h"
#include "CAssetMgr.h"

CInstancingBuffer::CInstancingBuffer()
{
	m_BoneBuffer = new CStructuredBuffer;
	m_PrevBoneBuffer = new CStructuredBuffer;
}

CInstancingBuffer::~CInstancingBuffer()
{
	if (nullptr != m_BoneBuffer)
		delete m_BoneBuffer;

	if (nullptr != m_PrevBoneBuffer)
		delete m_PrevBoneBuffer;
}

void CInstancingBuffer::Init()
{
	D3D11_BUFFER_DESC tDesc = {};
	tDesc.ByteWidth = sizeof(tInstancingData) * m_MaxCount;
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tDesc.Usage = D3D11_USAGE_DYNAMIC;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	MD_ENGINE_ASSERT(SUCCEEDED(DEVICE->CreateBuffer(&tDesc, NULL, &m_InstancingBuffer)), L"InstancingBuffer::Init �� ���� ���� ����!");
	
	m_CopyShader = (CCopyBoneCS*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"CopyBoneCS").Get();

	if (m_CopyShader == nullptr)
		m_CopyShader = new CCopyBoneCS;
}

void CInstancingBuffer::SetData()
{
	if (m_vecData.size() > m_MaxCount)
	{
		Resize((UINT)m_vecData.size());
	}

	D3D11_MAPPED_SUBRESOURCE tMap = {};

	CONTEXT->Map(m_InstancingBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tMap);
	memcpy(tMap.pData, &m_vecData[0], sizeof(tInstancingData) * m_vecData.size());
	CONTEXT->Unmap(m_InstancingBuffer.Get(), 0);

	// �� ������� �޸� ����
	if (m_vecBoneMat.empty())
		return;

	UINT iBufferSize = (UINT)m_vecBoneMat.size() * m_vecBoneMat[0]->GetBufferSize();
	if (m_BoneBuffer->GetBufferSize() < iBufferSize)
	{
		m_BoneBuffer->Create(m_vecBoneMat[0]->GetElementSize()
			, m_vecBoneMat[0]->GetElementCount() * (UINT)m_vecBoneMat.size(), SB_TYPE::SRV_UAV, false, nullptr);

		m_PrevBoneBuffer->Create(m_vecPrevBoneMat[0]->GetElementSize(), m_vecPrevBoneMat[0]->GetElementCount() * (UINT)m_vecPrevBoneMat.size(),
			SB_TYPE::SRV_UAV, false, nullptr);
	}

	// ����� ��ǻƮ ���̴� ����
	UINT iBoneCount = m_vecBoneMat[0]->GetElementCount();
	m_CopyShader->SetBoneCount(iBoneCount);
	for (UINT i = 0; i < (UINT)m_vecBoneMat.size(); ++i)
	{
		m_CopyShader->SetRowIndex(i);
		m_CopyShader->SetSourceBuffer(m_vecBoneMat[i]);
		m_CopyShader->SetDestBuffer(m_BoneBuffer);
		m_CopyShader->Execute();

		// Prev Bone
		m_CopyShader->SetSourceBuffer(m_vecPrevBoneMat[i]);
		m_CopyShader->SetDestBuffer(m_PrevBoneBuffer);
		m_CopyShader->Execute();
	}

	// Bone ���� ���� ��������
	m_BoneBuffer->Bind(17);
	m_PrevBoneBuffer->Bind(19);
}

void CInstancingBuffer::AddInstancingBoneMat(CStructuredBuffer* _pBuffer, CStructuredBuffer* _pPrevBuffer)
{
	++m_AnimInstCount;
	m_vecBoneMat.push_back(_pBuffer);
	m_vecPrevBoneMat.push_back(_pPrevBuffer);
}

void CInstancingBuffer::Resize(UINT _iCount)
{
	m_InstancingBuffer = nullptr;

	m_MaxCount = _iCount;

	D3D11_BUFFER_DESC tDesc = {};

	tDesc.ByteWidth = sizeof(tInstancingData) * m_MaxCount;
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tDesc.Usage = D3D11_USAGE_DYNAMIC;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	MD_ENGINE_ASSERT(SUCCEEDED(DEVICE->CreateBuffer(&tDesc, NULL, &m_InstancingBuffer)), L"�ν��Ͻ� ���� Resize �� ���� ����!");
}