#include "pch.h"
#include "CAnimator2D.h"

CAnimator2D::CAnimator2D()
	: CComponent(COMPONENT_TYPE::ANIMATOR2D)
{
}

CAnimator2D::~CAnimator2D()
{
}

void CAnimator2D::FinalTick()
{
	if (m_CurAnim)
	{
		//m_CurAnim->FinalTick();
	}
}