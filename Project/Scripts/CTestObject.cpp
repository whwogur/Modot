#include "spch.h"
#include "CTestObject.h"

CTestObject::CTestObject()
	: CScript(SCRIPT_TYPE::TESTOBJECT)
{
}

void CTestObject::Begin()
{
}

void CTestObject::Tick()
{
	if (KEY_TAP(KEY::_7))
	{
		const Matrix& ownerWM = Transform()->GetWorldMat();
		
        EDITOR_TRACE(MatrixToString(ownerWM).c_str());
	}
}

void CTestObject::SaveToFile(FILE* _File)
{
}

void CTestObject::LoadFromFile(FILE* _File)
{
}