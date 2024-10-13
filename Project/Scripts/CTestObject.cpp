#include "spch.h"
#include "CTestObject.h"

CTestObject::CTestObject()
	: CScript(SCRIPT_TYPE::TESTOBJECT)
{
}

void CTestObject::Tick()
{
	Vec3& rotRef = Transform()->GetRelativeRotationRef();
	
	rotRef.x += 1 * DT;
	rotRef.y += 1 * DT;
}

void CTestObject::SaveToFile(FILE* _File)
{
}

void CTestObject::LoadFromFile(FILE* _File)
{
}
