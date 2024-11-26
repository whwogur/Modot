#include "pch.h"
#include "CMeshRender.h"

#include "CTransform.h"
#include "CAnimator2D.h"
#include "CAnimator3D.h"
#include "CSprite.h"

CMeshRender::CMeshRender()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDER)
{
}

void CMeshRender::FinalTick()
{
}

void CMeshRender::Render()
{
	if (Animator2D() != nullptr)
		Animator2D()->Bind();
	else
		CAnimator2D::Clear();

	Transform()->Bind();

	// Animator3D Binding
	if (Animator3D())
	{
		Animator3D()->Bind();
		for (UINT i = 0; i < GetMesh()->GetSubsetCount(); ++i)
		{
			if (nullptr == GetMaterial(i))
				continue;
			GetMaterial(i)->SetUsingAnim3D(true); // Animation Mesh 알리기
			GetMaterial(i)->SetBoneCount(Animator3D()->GetBoneCount());
		}
	}

	for (UINT i = 0; i < GetMesh()->GetSubsetCount(); ++i)
	{
		// 재질 바인딩(재질 상수, 쉐이더 등등)
		if (!GetMaterial(i))
			continue;
		GetMaterial(i)->Bind();
		GetMesh()->Render(i);
	}
}

void CMeshRender::SaveToFile(FILE* _File)
{
	SaveDataToFile(_File);
}

void CMeshRender::LoadFromFile(FILE* _File)
{
	LoadDataFromFile(_File);
}