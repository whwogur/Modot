#include "pch.h"
#include "CBoundingSphere.h"

CBoundingSphere::CBoundingSphere(const CBoundingSphere& _Other)
	: m_Radius(_Other.m_Radius)
{
}

void CBoundingSphere::RenderBounds(const Vec3& _Pos)
{
	DrawDebugSphere(_Pos, m_Radius, { 0.3f, 0.4f, 0.5f, 1.f }, 0.f, true);
}