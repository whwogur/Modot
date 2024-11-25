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
	GetMaterial(0)->Bind();
	GetMesh()->Render(0);

	//for (UINT i = 0; i < GetMesh()->GetSubsetCount(); ++i)
	//{
	//	// ���� ���ε�(���� ���, ���̴� ���)
	//	if (!GetMaterial(i))
	//		continue;
	//	GetMaterial(i)->Binding();
	//	// ���ؽ�����, �ε������� ���ε� �� ������ ȣ��
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