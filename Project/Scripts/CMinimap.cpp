#include "spch.h"
#include "CMinimap.h"
#include <Engine/CLevelMgr.h>
CMinimap::CMinimap()
	: CScript(SCRIPT_TYPE::MINIMAP)
{
}

void CMinimap::Begin()
{
}

void CMinimap::Tick()
{
	if (m_SpaceDivX > 0.f)
	{
		const Vec3& playerPos = m_Player->Transform()->GetRelativePosRef();

		if (playerPos.x < m_SpaceDivX)
		{

		}
		else
		{

		}
	}

	else
	{

	}
}

void CMinimap::SaveToFile(FILE* _File)
{
	fwrite(&m_SpaceDivX, sizeof(float), 1, _File);
}

void CMinimap::LoadFromFile(FILE* _File)
{
	fread(&m_SpaceDivX, sizeof(float), 1, _File);
}
