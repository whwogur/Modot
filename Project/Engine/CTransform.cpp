#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CRenderComponent.h"
#include "CNavigation.h"
#include "CCell.h"
CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM)
	, m_RelativeDir{}
	, m_WorldDir{}
	, m_IndependentScale(false)
{
}


void CTransform::FinalTick()
{
	Matrix matScale = XMMatrixScaling(m_RelativeScale.x, m_RelativeScale.y, m_RelativeScale.z);
	// 이동 행렬
	Matrix matTranslation = XMMatrixTranslation(m_RelativePos.x, m_RelativePos.y, m_RelativePos.z);
	// 회전 행렬
	Matrix matRot = XMMatrixRotationX(m_RelativeRotation.x)
		* XMMatrixRotationY(m_RelativeRotation.y)
		* XMMatrixRotationZ(m_RelativeRotation.z);

	m_matWorld = matScale * matRot * matTranslation; // OpenGL이랑 순서 다름

	static Vec3 vDefaultAxis[3] =
	{
		Vec3(1.f, 0.f, 0.f),
		Vec3(0.f, 1.f, 0.f),
		Vec3(0.f, 0.f, 1.f)
	};

	for (int i = 0; i < 3; ++i)
	{
		// w 를 0 으로 확장
		m_RelativeDir[i] = XMVector3TransformNormal(vDefaultAxis[i], matRot);
		m_RelativeDir[i].Normalize();
	}

	// 부모 오브젝트가 있는지 확인
	if (GetOwner()->GetParent())
	{
		m_matTransformation = GetOwner()->GetParent()->Transform()->GetWorldMat();
		// 부모의 월드행렬을 곱해서 최종 월드행렬을 계산함
		const Matrix& matParentWorldMat = GetOwner()->GetParent()->Transform()->GetWorldMat();
		if (m_IndependentScale)
		{
			Vec3 vParentScale = GetOwner()->GetParent()->Transform()->GetRelativeScale();
			Matrix matParentScaleInv = XMMatrixScaling(1.f / vParentScale.x, 1.f / vParentScale.y, 1.f / vParentScale.z);

			// 부모의 크기 행렬 상쇄
			m_matTransformation = matParentScaleInv * m_matTransformation;

			m_matWorld *= m_matTransformation;
		}
		else
		{
			m_matWorld *= m_matTransformation;
		}

		// 최종 월드기준 오브젝트의 방향벡터를 구함
		for (int i = 0; i < 3; ++i)
		{
			m_WorldDir[i] = XMVector3TransformNormal(vDefaultAxis[i], m_matWorld);
		}
	}

	// 부모가 없으면, RelativeDir 이 곧 WorldDir
	else
	{
		for (int i = 0; i < 3; ++i)
		{
			m_WorldDir[i] = m_RelativeDir[i];
		}
	}

	// 월드 역행렬 계산
	m_matWorldInv = XMMatrixInverse(nullptr, m_matWorld);

	if (m_UseFrustumCulling)
		m_BoundingSphere->RenderBounds(m_RelativePos);
}

void CTransform::Bind()
{
	g_Trans.matWorld = m_matWorld;
	g_Trans.matWorldInv = m_matWorldInv;

	g_Trans.matWV = g_Trans.matWorld * g_Trans.matView;
	g_Trans.matWVP = g_Trans.matWV * g_Trans.matProj;
	CConstBuffer* pTransformCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	pTransformCB->SetData(&g_Trans);
	pTransformCB->Bind();
}

Vec3 CTransform::GetWorldScale()
{
	Vec3 vWorldScale = Vec3(1.f, 1.f, 1.f);

	CGameObject* pObject = GetOwner();

	while (pObject)
	{
		vWorldScale *= pObject->Transform()->GetRelativeScale();

		if (pObject->Transform()->m_IndependentScale)
			break;

		pObject = pObject->GetParent();
	}

	return vWorldScale;
}

CTransform& CTransform::operator=(const CTransform& _Other)
{
	m_RelativePos = _Other.m_RelativePos;
	m_RelativeScale = _Other.m_RelativeScale;
	m_RelativeRotation = _Other.m_RelativeRotation;
	m_matWorld = _Other.m_matWorld;
	m_matWorldInv = _Other.m_matWorldInv;
	m_IndependentScale = _Other.m_IndependentScale;
	for (int i = 0; i < 3; ++i)
	{
		m_RelativeDir[i] = _Other.m_RelativeDir[i];
		m_WorldDir[i] = _Other.m_WorldDir[i];
	}
	return *this;
}

void CTransform::SetFrustumCulling(bool _Cull)
{
	m_UseFrustumCulling = _Cull;

	if (m_BoundingSphere == nullptr)
		m_BoundingSphere = std::make_shared<CBoundingSphere>();

	m_BoundingSphere->SetRadius(m_RelativeScale.x / 2.f);

	CRenderComponent* rComp = GetOwner()->GetRenderComponent();
	if (rComp != nullptr)
		rComp->SetFrustumCheck(_Cull);
}

std::weak_ptr<CBoundingSphere> CTransform::GetBoundingSphere()
{
	if (!m_UseFrustumCulling)
		return std::weak_ptr<CBoundingSphere>();

	return m_BoundingSphere;
}

bool CTransform::MoveForward(float _DT, CNavigation* _Navigation, float _Radius)
{
	Vec3 vLook = m_WorldDir[DIR::FRONT];

	m_RelativePos += vLook * /*TODO*/500.f * _DT;

	if (nullptr == _Navigation)
		m_RelativePos = m_RelativePos;
	else if (true == _Navigation->CanMove(m_RelativePos + (vLook * _Radius)))
		m_RelativePos = m_RelativePos;
	else
		return false;

	return true;
}

bool CTransform::MoveForwardSliding(float _DT, CNavigation* _Navigation, float _Radius)
{
	Vec3 vLook = m_WorldDir[DIR::FRONT];

	m_RelativePos += vLook * /*TODO*/500.f * _DT;

	if (_Navigation)
	{
		_Navigation->Compute_CurrentIdx_viaDistance(m_RelativePos);
	}

	if (nullptr == _Navigation)
		m_RelativePos = m_RelativePos;
	else if (true == _Navigation->CanMove(m_RelativePos + vLook * _Radius))
	{
		if (_Navigation->Get_CurrentCelltype() == CCell::DROP)
		{
			Vec3 vNormal = XMVector3Normalize(_Navigation->Get_LastNormal());

			float fDot = XMVectorGetX(XMVector3Dot(vLook, vNormal));
			vNormal = vNormal * fDot * -1.f;
			Vec3 vSliding = XMVector3Normalize(vLook + vNormal);
			
			if (true == _Navigation->CanMove(m_RelativePos + vLook * _Radius) && _Navigation->Get_CurrentCelltype() != CCell::DROP)
				m_RelativePos += vSliding * _DT * /*TODO*/500.f;
			return false;
		}
	}

	else if (false == _Navigation->CanMove(m_RelativePos + vLook * _Radius))
	{
		Vec3 vNormal = XMVector3Normalize(_Navigation->Get_LastNormal());

		float fDot = XMVectorGetX(XMVector3Dot(vLook, vNormal));
		vNormal = vNormal * fDot * -1.f;
		Vec3 vSliding = XMVector3Normalize(vLook + vNormal);

		
		if (true == _Navigation->CanMove(m_RelativePos + vLook * _Radius))
			m_RelativePos += vSliding * _DT * /*TODO*/500.f;
		return false;
	}


	return true;
}

void CTransform::SaveToFile(FILE* _File)
{
	fwrite(&m_RelativePos, sizeof(Vec3), 1, _File);
	fwrite(&m_RelativeScale, sizeof(Vec3), 1, _File);
	fwrite(&m_RelativeRotation, sizeof(Vec3), 1, _File);
	fwrite(&m_IndependentScale, sizeof(bool), 1, _File);
}

void CTransform::LoadFromFile(FILE* _File)
{
	fread(&m_RelativePos, sizeof(Vec3), 1, _File);
	fread(&m_RelativeScale, sizeof(Vec3), 1, _File);
	fread(&m_RelativeRotation, sizeof(Vec3), 1, _File);
	fread(&m_IndependentScale, sizeof(bool), 1, _File);
}