#include "pch.h"
#include "CCollider2D.h"

#include "CTransform.h"
#include "CScript.h"

CCollider2D::CCollider2D()
	: CComponent(COMPONENT_TYPE::COLLIDER2D)
	, m_OverlapCount(0)
{
}

CCollider2D::~CCollider2D()
{
}

CCollider2D::CCollider2D(const CCollider2D& _Other)
	: CComponent(_Other)
	, m_Offset(_Other.m_Offset)
	, m_Scale(_Other.m_Scale)
	, m_matColWorld{}
	, m_OverlapCount(0)
	, m_IndependentScale(_Other.m_IndependentScale)
{
}

void CCollider2D::FinalTick()
{
	Matrix matTranslation = XMMatrixTranslation(m_Offset.x, m_Offset.y, m_Offset.z);
	Matrix matScale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);

	// ũ��, ȸ��, �̵�
	Matrix matObjectScaleInv = XMMatrixIdentity();

	if (m_IndependentScale)
	{
		Vec3 vObjectScale = GetOwner()->Transform()->GetWorldScale();
		matObjectScaleInv = XMMatrixScaling(vObjectScale.x, vObjectScale.y, vObjectScale.z);
		matObjectScaleInv = XMMatrixInverse(nullptr, matObjectScaleInv);
	}

	m_matColWorld = matScale * matTranslation * matObjectScaleInv * GetOwner()->Transform()->GetWorldMat();

	if (m_OverlapCount)
		DrawDebugRect(m_matColWorld, Vec4(1.f, 0.f, 0.f, 1.f), 0.f, false);
	else
		DrawDebugRect(m_matColWorld, Vec4(0.f, 1.f, 0.f, 1.f), 0.f, false);
}

void CCollider2D::BeginOverlap(CCollider2D* _Other)
{
	++m_OverlapCount;

	const vector<CScript*>& vecScripts = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScripts.size(); ++i)
	{
		vecScripts[i]->BeginOverlap(this, _Other->GetOwner(), _Other);
	}
}

void CCollider2D::Overlap(CCollider2D* _Other)
{
	const vector<CScript*>& vecScripts = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScripts.size(); ++i)
	{
		vecScripts[i]->Overlap(this, _Other->GetOwner(), _Other);
	}
}

void CCollider2D::EndOverlap(CCollider2D* _Other)
{
	--m_OverlapCount;
	const vector<CScript*>& vecScripts = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScripts.size(); ++i)
	{
		vecScripts[i]->EndOverlap(this, _Other->GetOwner(), _Other);
	}
}