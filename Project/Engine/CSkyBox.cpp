#include "pch.h"
#include "CSkyBox.h"
#include "CRenderMgr.h"
#include "CTransform.h"
CSkyBox::CSkyBox()
	: CRenderComponent(COMPONENT_TYPE::SKYBOX)
{
}

void CSkyBox::FinalTick()
{
}

void CSkyBox::Render()
{

	Transform()->Bind();
	GetMaterial()->Bind();
	GetMesh()->Render();
}

void CSkyBox::SaveToFile(FILE* _File)
{

}

void CSkyBox::LoadFromFile(FILE* _File)
{

}
