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
	/*if (KEY_TAP(KEY::_7))
	{
		const Matrix& ownerWM = Transform()->GetWorldMat();
		string temp("TestObject WorldMat:\n" + MatrixToString(ownerWM, 2));
        EDITOR_TRACE(temp.c_str());
	}*/
}

void CTestObject::SaveToFile(FILE* _File)
{
}

void CTestObject::LoadFromFile(FILE* _File)
{
}