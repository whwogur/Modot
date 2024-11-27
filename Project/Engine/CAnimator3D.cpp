#include "pch.h"
#include "CAnimator3D.h"
#include "CAssetMgr.h"
#include "CTimeMgr.h"
#include "CStructuredBuffer.h"
#include "components.h"
#include "CAnimation3DShader.h"
CAnimator3D::CAnimator3D()
	: CComponent(COMPONENT_TYPE::ANIMATOR3D)
{
	m_pBoneFinalMatBuffer = new CStructuredBuffer;
}
CAnimator3D::CAnimator3D(const CAnimator3D& _Other)
	: m_pVecBones(_Other.m_pVecBones)
	, m_pVecClip(_Other.m_pVecClip)
	, m_iCurClip(_Other.m_iCurClip)
	, m_dCurTime(_Other.m_dCurTime)
	, m_iFrameCount(_Other.m_iFrameCount)
	, m_pBoneFinalMatBuffer(nullptr)
	, m_bFinalMatUpdate(false)
	, m_iFrameIdx(_Other.m_iFrameIdx)
	, m_iNextFrameIdx(_Other.m_iNextFrameIdx)
	, m_fRatio(_Other.m_fRatio)
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
{
	m_pBoneFinalMatBuffer = new CStructuredBuffer;
}
CAnimator3D::~CAnimator3D()
{
	if (nullptr != m_pBoneFinalMatBuffer)
		delete m_pBoneFinalMatBuffer;
}
void CAnimator3D::FinalTick()
{
	m_dCurTime = 0.f;
	// ���� ������� Clip �� �ð��� �����Ѵ�.
	m_vecClipUpdateTime[m_iCurClip] += EngineDT;

	if (m_vecClipUpdateTime[m_iCurClip] >= m_pVecClip->at(m_iCurClip).dTimeLength)
	{
		m_vecClipUpdateTime[m_iCurClip] = 0.f;
	}
	m_dCurTime = m_pVecClip->at(m_iCurClip).dStartTime + m_vecClipUpdateTime[m_iCurClip];

	// ���� ������ �ε��� ���ϱ�
	double dFrameIdx = m_dCurTime * (double)m_iFrameCount;
	m_iFrameIdx = (int)(dFrameIdx);

	// ���� ������ �ε���
	if (m_iFrameIdx >= m_pVecClip->at(0).iFrameLength - 1)
		m_iNextFrameIdx = m_iFrameIdx;	// ���̸� ���� �ε����� ����
	else
		m_iNextFrameIdx = m_iFrameIdx + 1;

	// �����Ӱ��� �ð��� ���� ������ �����ش�.
	m_fRatio = (float)(dFrameIdx - (double)m_iFrameIdx);

	// ��ǻƮ ���̴� ���꿩��
	m_bFinalMatUpdate = false;
}
void CAnimator3D::SetAnimClip(const std::vector<tMTAnimClip>* _vecAnimClip)
{
	m_pVecClip = _vecAnimClip;
	m_vecClipUpdateTime.resize(m_pVecClip->size());
	// �׽�Ʈ �ڵ�
	/*static float fTime = 0.f;
	fTime += 1.f;
	m_vecClipUpdateTime[0] = fTime;*/
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
		pUpdateShader->SetBoneCount(iBoneCount);
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

		pMtrl->SetUsingAnim3D(false); // Animation Mesh �˸���
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