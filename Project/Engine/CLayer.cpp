#include "pch.h"
#include "CLayer.h"

#include "CGameObject.h"

void CLayer::Tick()
{
	for (size_t i = 0; i < m_Parents.size(); ++i)
	{
		m_Parents[i]->Tick();
	}
}

void CLayer::FinalTick()
{
	for (size_t i = 0; i < m_Parents.size(); ++i)
	{
		m_Parents[i]->FinalTick();
	}
}

void CLayer::Render()
{
	for (size_t i = 0; i < m_Parents.size(); ++i)
	{
		m_Parents[i]->Render();
	}
}