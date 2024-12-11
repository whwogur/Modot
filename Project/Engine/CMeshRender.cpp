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
	if (Animator3D())
		Animator3D()->ClearData();
}

void CMeshRender::Render()
{
	// 2D�ִϸ����Ͱ� ������ ���� ������� Sprite ������ Binding �Ѵ�.
	if (Animator2D())
		Animator2D()->Bind();
	else
		CAnimator2D::Clear();

	// ��ġ, ũ��, ȸ�� �������� ���ε�
	Transform()->Bind();

	// Animator3D Binding
	if (Animator3D())
	{
		Animator3D()->Bind();

		for (UINT i = 0; i < GetMesh()->GetSubsetCount(); ++i)
		{
			if (nullptr == GetMaterial(i))
				continue;

			GetMaterial(i)->SetUsingAnim3D(1); // Animation Mesh �˸���
			GetMaterial(i)->SetBoneCount(Animator3D()->GetBoneCount());
		}
	}

	for (UINT i = 0; i < GetMesh()->GetSubsetCount(); ++i)
	{
		// ���� ���ε�(���� ���, ���̴� ���)
		if (!GetMaterial(i))
			continue;

		GetMaterial(i)->Bind();

		// ���ؽ�����, �ε������� ���ε� �� ������ ȣ��
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