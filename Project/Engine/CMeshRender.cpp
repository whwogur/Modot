#include "pch.h"
#include "CMeshRender.h"

#include "CTransform.h"
#include "CAnimator2D.h"
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
	for (UINT i = 0; i < GetMesh()->GetSubsetCount(); ++i)
	{
		// 재질 바인딩(재질 상수, 쉐이더 등등)
		if (!GetMaterial(i))
			continue;
		GetMaterial(i)->Bind();
		GetMesh()->Render(i);
	}

	//for (UINT i = 0; i < GetMesh()->GetSubsetCount(); ++i)
	//{
	//	// 재질 바인딩(재질 상수, 쉐이더 등등)
	//	if (!GetMaterial(i))
	//		continue;
	//	GetMaterial(i)->Binding();
	//	// 버텍스버퍼, 인덱스버퍼 바인딩 및 렌더링 호출
	//	GetMesh()->Render(i);
	//}
}

void CMeshRender::SaveToFile(FILE* _File)
{
	SaveDataToFile(_File);
}

void CMeshRender::LoadFromFile(FILE* _File)
{
	LoadDataFromFile(_File);
}