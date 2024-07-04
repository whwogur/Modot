#include "pch.h"
#include "Animator2DUI.h"

Animator2DUI::Animator2DUI()
	: ComponentUI(COMPONENT_TYPE::ANIMATOR2D)
{
}

Animator2DUI::~Animator2DUI()
{
}

void Animator2DUI::Update()
{
	Title();
}
