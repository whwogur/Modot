#include "pch.h"
#include "Outliner.h"
#include "TreeUI.h"
Outliner::Outliner()
{
	m_Tree = new TreeUI;
	m_Tree->SetName("OutlinerTree");
	AddChild(m_Tree);
}

Outliner::~Outliner()
{
}

void Outliner::Update()
{

}