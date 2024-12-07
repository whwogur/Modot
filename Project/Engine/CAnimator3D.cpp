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
	m_pBoneFinalMatBuffer		= new CStructuredBuffer;
	m_pBoneTransformMatBuffer	= new CStructuredBuffer;
	m_pPrevBoneFinalMatBuffer	= new CStructuredBuffer;
}

CAnimator3D::CAnimator3D(const CAnimator3D& _Other)
	: m_iCurClipIdx(_Other.m_iCurClipIdx)
	, m_dCurTime(_Other.m_dCurTime)
	, m_iFrameCount(_Other.m_iFrameCount)
	, m_pBoneFinalMatBuffer(nullptr)
	, m_bFinalMatUpdated(false)
	, m_iFrameIdx(_Other.m_iFrameIdx)
	, m_iNextFrameIdx(_Other.m_iNextFrameIdx)
	, m_fRatio(_Other.m_fRatio)
	, m_bPlay(_Other.m_bPlay)
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
{
	m_pBoneFinalMatBuffer		= new CStructuredBuffer;
	m_pBoneTransformMatBuffer	= new CStructuredBuffer;
	m_pPrevBoneFinalMatBuffer	= new CStructuredBuffer;
}

CAnimator3D::~CAnimator3D()
{
	if (nullptr != m_pBoneFinalMatBuffer)
		delete m_pBoneFinalMatBuffer;

	if (nullptr != m_pPrevBoneFinalMatBuffer)
		delete m_pPrevBoneFinalMatBuffer;
}

UINT CAnimator3D::GetClipIdx(const wstring& _ClipName)
{
	auto iter = m_AnimationHash.find(_ClipName);
	if (iter != m_AnimationHash.end())
		return iter->second;
	else
		MD_ENGINE_ERROR(L"�ִϸ��̼� \"{0}\" ��ã��", _ClipName);
	
	return 0;
}

void CAnimator3D::Play(const wstring& _ClipName, bool _Repeat, float _PlaySpeed, double _TransitionDuration)
{
	Play(GetClipIdx(_ClipName), _Repeat, _PlaySpeed, _TransitionDuration);
}

void CAnimator3D::Play(int _ClipIdx, bool _Repeat, float _PlaySpeed, double _TransitionDuration)
{
	if (-1 == _ClipIdx || nullptr == m_SkeletalMesh || m_vecClipUpdateTime.empty())
		return;

	// Change Duration�� 0�̸� �ִϸ��̼� ��� ��ȯ
	if (_TransitionDuration <= 1e-9)
	{
		// �ִϸ��̼� ��� ����
		m_bRepeat = _Repeat;
		m_fPlaySpeed = _PlaySpeed;

		SetCurClipIdx(_ClipIdx);

		// �ʱ�ȭ
		m_bTransition = false;
		m_dCurTransitionTime = 0.;
		m_dTransitionDuration = 0.;
		m_iNextClipIdx = -1;
		m_bNextRepeat = true;
		m_fNextPlaySpeed = 1.f;

		m_vecClipUpdateTime[m_iCurClipIdx] = 0.f; // �ִϸ��̼��� ó������ ����
		m_iNextFrameIdx = m_SkeletalMesh->GetAnimClip()->at(m_iCurClipIdx).iStartFrame;

		m_iFrameIdx = m_iNextFrameIdx;
		m_iNextFrameIdx = m_iNextFrameIdx;
		m_fRatio = 0.f;

		return;
	}

	m_bTransition = true;
	m_dCurTransitionTime = 0.;
	m_dTransitionDuration = _TransitionDuration;
	m_iNextClipIdx = _ClipIdx;
	m_bNextRepeat = _Repeat;
	m_fNextPlaySpeed = _PlaySpeed;

	m_iNextFrameIdx = m_SkeletalMesh->GetAnimClip()->at(m_iNextClipIdx).iStartFrame;
}

void CAnimator3D::FinalTick()
{
    if (!IsValid())
        return;

    float DeltaTime = m_eTimeOption == DT_OPTIONS::SCALE ? DT : EngineDT;

    // �ٸ� Clip ���� ��ȯ ���� ���
    if (m_bTransition)
    {
        if (m_bPlay)
        {
            m_dCurTransitionTime += DeltaTime * m_fPlaySpeed;
        }

        m_fRatio = float(m_dCurTransitionTime / m_dTransitionDuration);

        if (m_dCurTransitionTime >= m_dTransitionDuration)
        {
            // �ִϸ��̼� ��� ����
            m_bRepeat   = m_bNextRepeat;
            m_fPlaySpeed = m_fNextPlaySpeed;

            SetCurClipIdx(m_iNextClipIdx);
            
            m_vecClipUpdateTime[m_iCurClipIdx] = 0.f; // �ִϸ��̼��� ó������ ����

            // �ʱ�ȭ
            m_bTransition = false;
            m_dCurTransitionTime = 0.;
            m_dTransitionDuration = 0.;
            m_iNextClipIdx = -1;
            m_bNextRepeat = true;
            m_fNextPlaySpeed = 1.f;

            // �̹� ������ �ִϸ��̼� ����
            m_iFrameIdx = m_iNextFrameIdx;
            m_iNextFrameIdx = m_iNextFrameIdx;
            m_fRatio = 0.;
        }
    }
    else
    {
        // ���� ������� Clip �� �ð��� �����Ѵ�.
        if (m_bPlay)
        {
			m_vecClipUpdateTime[m_iCurClipIdx] += DeltaTime * m_fPlaySpeed;

			if (m_vecClipUpdateTime[m_iCurClipIdx] >= m_SkeletalMesh->GetAnimClip()->at(m_iCurClipIdx).dTimeLength)
			{
				// �ݺ� ���
				if (m_bRepeat)
				{
					m_vecClipUpdateTime[m_iCurClipIdx] = 0.f;
				}
				else
				{
					m_vecClipUpdateTime[m_iCurClipIdx] = m_SkeletalMesh->GetAnimClip()->at(m_iCurClipIdx).dTimeLength;
				}
			}
        }

        m_dCurTime = m_SkeletalMesh->GetAnimClip()->at(m_iCurClipIdx).dStartTime + m_vecClipUpdateTime[m_iCurClipIdx];

        // ���� ������ �ε��� ���ϱ�
        double dFrameIdx = m_dCurTime * m_iFrameCount;
        m_iFrameIdx = (int)dFrameIdx;
		if (m_iFrameIdx >= m_SkeletalMesh->GetAnimClip()->at(m_iCurClipIdx).iEndFrame)
		{
			m_iNextFrameIdx = m_iFrameIdx; // ���̸� ���� �ε����� ����
		}
		else
		{
			m_iNextFrameIdx = m_iFrameIdx + 1;
		}

		// �����Ӱ��� �ð��� ���� ������ �����ش�.
		m_fRatio = (float)(dFrameIdx - (double)m_iFrameIdx);
    }

    // ��ǻƮ ���̴� ���꿩��
    m_bFinalMatUpdated = false;
}

void CAnimator3D::Bind()
{
	if (!IsValid())
		return;

	if (!m_bFinalMatUpdated)
	{
		// Animation Update Compute Shader
		static CAnimation3DShader* pUpdateShader =
			(CAnimation3DShader*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"Animation3DUpdateCS").Get();

		// Bone Data Check
		CheckBoneMatBuffer();

		UINT BoneCount = m_SkeletalMesh->GetBoneCount();
		pUpdateShader->SetBoneCount(BoneCount);
		pUpdateShader->SetFrameIndex(m_iFrameIdx);
		pUpdateShader->SetNextFrameIdx(m_iNextFrameIdx);
		pUpdateShader->SetFrameRatio(m_fRatio);

		pUpdateShader->SetFrameDataBuffer(m_SkeletalMesh->GetBoneFrameDataBuffer());
		pUpdateShader->SetOffsetMatBuffer(m_SkeletalMesh->GetBoneInverseBuffer());
		pUpdateShader->SetBoneTransformMatBuffer(m_pBoneTransformMatBuffer);
		pUpdateShader->SetOutputBuffer(m_pBoneFinalMatBuffer);
		pUpdateShader->SetPrevOutputBuffer(m_pPrevBoneFinalMatBuffer);

		// ������Ʈ ���̴� ����
		pUpdateShader->Execute();

		// Bone Transformation Matrix ����
		m_pBoneTransformMatBuffer->GetData(m_vecBoneTransformMat.data(), BoneCount);

		m_bFinalMatUpdated = true;
	}

	// t17 �������Ϳ� ������� ������(��������) ���ε�		
	m_pBoneFinalMatBuffer->Bind(17);
	m_pPrevBoneFinalMatBuffer->Bind(19);
}

UINT CAnimator3D::GetBoneCount() const
{
	if (nullptr == m_SkeletalMesh)
		return 0;

	return m_SkeletalMesh->GetBoneCount();
}

void CAnimator3D::SetFrameIdx(int _FrameIdx)
{
	double CurFrameTime = (_FrameIdx / m_iFrameCount) - m_SkeletalMesh->GetAnimClip()->at(m_iCurClipIdx).dStartTime;
	m_vecClipUpdateTime[m_iCurClipIdx] = std::clamp(CurFrameTime, 0., m_SkeletalMesh->GetAnimClip()->at(m_iCurClipIdx).dTimeLength);
}

void CAnimator3D::ClearData()
{
	m_pBoneFinalMatBuffer->Clear(17);
	m_pPrevBoneFinalMatBuffer->Clear(19);

	UINT iMtrlCount = MeshRender()->GetMaterialCount();
	Ptr<CMaterial> pMtrl = nullptr;

	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		pMtrl = MeshRender()->GetSharedMaterial(i);
		if (nullptr == pMtrl)
			continue;

		pMtrl->SetUsingAnim3D(0); // Animation Mesh �˸���
		pMtrl->SetBoneCount(0);
	}
}

void CAnimator3D::SetSkeletalMesh(Ptr<CMesh> _SkeletalMesh)
{
	// Reset
	m_SkeletalMesh = nullptr;
	m_AnimationHash.clear();
	m_iNextClipIdx = 0;
	m_iCurClipIdx = 0;
	m_vecClipUpdateTime.clear();
	m_bRepeat = true;
	m_bNextRepeat = true;
	m_fPlaySpeed = 1.f;
	m_fNextPlaySpeed = 1.f;
	m_iFrameCount = 30;
	m_dCurTime = 0.;
	m_iFrameIdx = 0;
	m_iNextFrameIdx = 0;
	m_fRatio = 0.;
	m_bFinalMatUpdated = false;
	m_dCurTransitionTime = 0.0;
	m_dTransitionDuration = 0.0;

	if (nullptr == _SkeletalMesh)
	{
		return;
	}

	m_SkeletalMesh = _SkeletalMesh;

	const std::vector<tMTAnimClip>* vecAnimClip = m_SkeletalMesh->GetAnimClip();
	for (int i = 0; i < (int)vecAnimClip->size(); ++i)
	{
		const tMTAnimClip& Clip = vecAnimClip->at(i);
		m_AnimationHash.emplace(Clip.strAnimName, i);
	}

	m_vecClipUpdateTime.resize(vecAnimClip->size());

	// Frame Rate ����
	if (!vecAnimClip->empty())
	{
		m_iFrameCount = FbxTime::GetFrameRate(vecAnimClip->back().eMode);
	}

	CheckBoneMatBuffer();
}

void CAnimator3D::CheckBoneMatBuffer()
{
	if (!IsValid())
		return;

	UINT iBoneCount = m_SkeletalMesh->GetBoneCount();
	if (m_pBoneTransformMatBuffer->GetElementCount() != iBoneCount)
	{
		m_pBoneTransformMatBuffer->Create(sizeof(Matrix), iBoneCount, SB_TYPE::SRV_UAV, true, nullptr);
		m_vecBoneTransformMat.clear();
		m_vecBoneTransformMat.resize(iBoneCount);
	}

	if (m_pBoneFinalMatBuffer->GetElementCount() != iBoneCount)
	{
		m_pBoneFinalMatBuffer->Create(sizeof(Matrix), iBoneCount, SB_TYPE::SRV_UAV, false, nullptr);
		m_pPrevBoneFinalMatBuffer->Create(sizeof(Matrix), iBoneCount, SB_TYPE::SRV_UAV, false, nullptr);
	}
}

void CAnimator3D::SaveToFile(FILE* _File)
{
}
void CAnimator3D::LoadFromFile(FILE* _File)
{
}