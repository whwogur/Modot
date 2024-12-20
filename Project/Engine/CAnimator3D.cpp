#include "pch.h"
#include "CAnimator3D.h"
#include "CAssetMgr.h"
#include "CTimeMgr.h"
#include "CStructuredBuffer.h"
#include "components.h"
#include "CAnimation3DShader.h"
CAnimator3D::CAnimator3D()
	: CComponent(COMPONENT_TYPE::ANIMATOR3D)
	, m_pVecBones(nullptr)
	, m_pVecClip(nullptr)
	, m_iCurClip(107)
	, m_dCurTime(0.)
	, m_iFrameCount(30)
	, m_pBoneFinalMatBuffer(nullptr)
	, m_bFinalMatUpdate(false)
	, m_iFrameIdx(0)
	, m_iNextFrameIdx(0)
	, m_fRatio(0.f)
	, m_bPlay(true)
	, m_fPlaySpeed(1.0f)
	, m_bRepeat(true)
{
	m_pBoneFinalMatBuffer = std::make_shared<CStructuredBuffer>();
}

CAnimator3D::CAnimator3D(const CAnimator3D& _origin)
	: CComponent(COMPONENT_TYPE::ANIMATOR3D)
	, m_pVecClip(_origin.m_pVecClip)
	, m_iCurClip(_origin.m_iCurClip)
	, m_dCurTime(_origin.m_dCurTime)
	, m_iFrameCount(_origin.m_iFrameCount)
	, m_pBoneFinalMatBuffer(nullptr)
	, m_bFinalMatUpdate(false)
	, m_iFrameIdx(_origin.m_iFrameIdx)
	, m_iNextFrameIdx(_origin.m_iNextFrameIdx)
	, m_fRatio(_origin.m_fRatio)
	, m_pVecBones(_origin.m_pVecBones)
	, m_bPlay(_origin.m_bPlay)
	, m_fPlaySpeed(_origin.m_fPlaySpeed)
	, m_bRepeat(_origin.m_bRepeat)
{
	m_pBoneFinalMatBuffer = std::make_shared<CStructuredBuffer>();
}


void CAnimator3D::FinalTick()
{
	if (m_bPlay)
	{
		m_dCurTime = 0.f;
		// ���� ������� Clip �� �ð��� �����Ѵ�.
		m_vecClipUpdateTime[m_iCurClip] += m_fPlaySpeed * EngineDT;

		if (m_vecClipUpdateTime[m_iCurClip] >= m_pVecClip->at(m_iCurClip).dTimeLength)
		{
			if (m_bRepeat)
			{
				m_vecClipUpdateTime[m_iCurClip] = 0.f;
			}
			else
			{
				m_bPlay = false;
				return;
			}
		}

		m_dCurTime = m_pVecClip->at(m_iCurClip).dStartTime + m_vecClipUpdateTime[m_iCurClip];

		// ���� ������ �ε��� ���ϱ�
		double dFrameIdx = m_dCurTime * (double)m_iFrameCount;
		m_iFrameIdx = (int)(dFrameIdx);

		// ���� ������ �ε���
		if (m_iFrameIdx >= m_pVecClip->at(m_iCurClip).iStartFrame + m_pVecClip->at(m_iCurClip).iFrameLength - 1)
		{
			m_iNextFrameIdx = m_iFrameIdx;	// ���̸� ���� �ε����� ����		
		}
		else
		{
			m_iNextFrameIdx = m_iFrameIdx + 1;
		}

		// �����Ӱ��� �ð��� ���� ������ �����ش�.
		m_fRatio = (float)(dFrameIdx - (double)m_iFrameIdx);

		// ��ǻƮ ���̴� ���꿩��
		m_bFinalMatUpdate = false;
	}
}

void CAnimator3D::SetAnimClip(const std::vector<tMTAnimClip>* _vecAnimClip)
{
	m_pVecClip = _vecAnimClip;
	m_vecClipUpdateTime.resize(m_pVecClip->size());
}

void CAnimator3D::Bind()
{
	if (!m_bFinalMatUpdate)
	{
		// Animation3D Update Compute Shader
		CAnimation3DShader* pUpdateShader = (CAnimation3DShader*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"Animation3DUpdateCS").Get();

		// Bone Data
		Ptr<CMesh> pMesh = MeshRender()->GetMesh();
		check_mesh(pMesh);

		pUpdateShader->SetFrameDataBuffer(pMesh->GetBoneFrameDataBuffer());
		pUpdateShader->SetOffsetMatBuffer(pMesh->GetBoneInverseBuffer());
		pUpdateShader->SetOutputBuffer(m_pBoneFinalMatBuffer);

		UINT iBoneCount = (UINT)m_pVecBones->size();
		pUpdateShader->SetBoneCount((int)iBoneCount);
		pUpdateShader->SetFrameIndex(m_iFrameIdx);
		pUpdateShader->SetNextFrameIdx(m_iNextFrameIdx);
		pUpdateShader->SetFrameRatio(m_fRatio);

		// ������Ʈ ���̴� ����
		pUpdateShader->Execute();

		m_bFinalMatUpdate = true;
	}
	// t17 �������Ϳ� ������� ������(��������) ���ε�		
	m_pBoneFinalMatBuffer->Bind(17);
}

void CAnimator3D::ClearData()
{
	m_pBoneFinalMatBuffer->Clear(17);

	UINT iMtrlCount = MeshRender()->GetMaterialCount();
	Ptr<CMaterial> pMtrl = nullptr;
	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		pMtrl = MeshRender()->GetSharedMaterial(i);
		if (nullptr == pMtrl)
			continue;

		pMtrl->SetUsingAnim3D(0);
		pMtrl->SetBoneCount(0);
	}
}

void CAnimator3D::check_mesh(Ptr<CMesh> _pMesh)
{
	UINT iBoneCount = _pMesh->GetBoneCount();
	if (m_pBoneFinalMatBuffer->GetElementCount() != iBoneCount)
	{
		m_pBoneFinalMatBuffer->Create(sizeof(Matrix), iBoneCount, SB_TYPE::SRV_UAV, false, nullptr);
	}
}

void CAnimator3D::SaveToFile(FILE* _File)
{
}

void CAnimator3D::LoadFromFile(FILE* _File)
{
}