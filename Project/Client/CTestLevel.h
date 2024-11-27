#pragma once
class CTestLevel
{
public:
	CTestLevel() = delete;
	CTestLevel(const CTestLevel&) = delete;
	CTestLevel& operator=(const CTestLevel&) = delete;

public:
	static void CreateTestLevel();
};

