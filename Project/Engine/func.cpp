#include "pch.h"
#include "func.h"

#include "CRenderMgr.h"
#include "CTaskMgr.h"
#include "CGameObject.h"

void CreateObject(CGameObject* _NewObject, int _LayerIndex)
{
	tTask task = {};
	task.Type = TASK_TYPE::CREATE_OBJECT;
	task.Param_0 = _LayerIndex;
	task.Param_1 = (DWORD_PTR)_NewObject;

	CTaskMgr::GetInst()->AddTask(task);
}

void DeleteObject(CGameObject* _DeleteObject)
{
	tTask task = {};
	task.Type = TASK_TYPE::DELETE_OBJECT;
	task.Param_0 = (DWORD_PTR)_DeleteObject;

	CTaskMgr::GetInst()->AddTask(task);
}


void ChangeLevelState(LEVEL_STATE _NextState)
{
	tTask task = {};
	task.Type = TASK_TYPE::CHANGE_LEVELSTATE;
	task.Param_0 = (DWORD_PTR)_NextState;

	CTaskMgr::GetInst()->AddTask(task);
}

void ChangeLevel(CLevel* _Level, LEVEL_STATE _NextLevelState)
{
	tTask task = {};
	task.Type = TASK_TYPE::CHANGE_LEVEL;

	task.Param_0 = (DWORD_PTR)_Level;
	task.Param_1 = (DWORD_PTR)_NextLevelState;

	CTaskMgr::GetInst()->AddTask(task);
}

bool IsValid(CGameObject*& _Object)
{
	if (nullptr == _Object)
		return false;

	if (_Object->IsDead())
	{
		_Object = nullptr;
		return false;
	}

	return true;
}

void DrawDebugRect(const Vec3& _Pos, const Vec3& _Scale, const Vec3& _Rot, const Vec4& _Color, float _Life, bool _DepthTest)
{
	tDebugShapeInfo Info = {};

	Info.Shape = DEBUG_SHAPE::RECT;
	Info.vPos = _Pos;
	Info.vScale = _Scale;
	Info.vRot = _Rot;
	Info.LifeTime = _Life;

	Info.matWorld = XMMatrixScaling(_Scale.x, _Scale.y, _Scale.z)
		* XMMatrixRotationX(_Rot.x)
		* XMMatrixRotationY(_Rot.y)
		* XMMatrixRotationZ(_Rot.z)
		* XMMatrixTranslation(_Pos.x, _Pos.y, _Pos.z);

	Info.vColor = _Color;
	Info.DepthTest = _DepthTest;

	CRenderMgr::GetInst()->AddDebugShapeInfo(Info);
}

void DrawDebugRect(const Matrix& _matWorld, Vec4 _Color, float _Life, bool _DepthTest)
{
	tDebugShapeInfo Info = {};

	Info.Shape = DEBUG_SHAPE::RECT;
	Info.matWorld = _matWorld;
	Info.vColor = _Color;
	Info.DepthTest = _DepthTest;
	Info.LifeTime = _Life;

	CRenderMgr::GetInst()->AddDebugShapeInfo(Info);
}

void DrawDebugCircle(const Vec3& _Pos, float _Radius, const Vec4& _Color, float _Life, bool _DepthTest)
{
	tDebugShapeInfo Info = {};

	Info.Shape = DEBUG_SHAPE::CIRCLE;
	Info.vPos = _Pos;
	Info.vScale = Vec3(_Radius * 2.f, _Radius * 2.f, 1.f);
	Info.vRot = Vec3(0.f, 0.f, 0.f);
	Info.LifeTime = _Life;
	Info.Age = 0;

	Info.matWorld = XMMatrixScaling(Info.vScale.x, Info.vScale.y, Info.vScale.z)
		* XMMatrixTranslation(_Pos.x, _Pos.y, _Pos.z);

	Info.vColor = _Color;
	Info.DepthTest = _DepthTest;

	CRenderMgr::GetInst()->AddDebugShapeInfo(Info);
}

void DrawDebugCube(const Vec3& _Pos, const Vec3& _Scale, const Vec3& _Rot, const Vec4& _Color, float _Life, bool _DepthTest)
{
	tDebugShapeInfo Info = {};
	Info.Shape = DEBUG_SHAPE::CUBE;
	Info.vPos = _Pos;
	Info.vScale = _Scale;
	Info.vRot = _Rot;
	Info.LifeTime = _Life;
	Info.matWorld = XMMatrixScaling(_Scale.x, _Scale.y, _Scale.z)
		* XMMatrixRotationX(_Rot.x)
		* XMMatrixRotationY(_Rot.y)
		* XMMatrixRotationZ(_Rot.z)
		* XMMatrixTranslation(_Pos.x, _Pos.y, _Pos.z);
	Info.vColor = _Color;
	Info.DepthTest = _DepthTest;
	CRenderMgr::GetInst()->AddDebugShapeInfo(Info);
}

void DrawDebugCube(const Matrix& _matWorld, const Vec4& _Color, float _Life, bool _DepthTest)
{
	tDebugShapeInfo Info = {};
	Info.Shape = DEBUG_SHAPE::CUBE;
	Info.matWorld = _matWorld;
	Info.vColor = _Color;
	Info.DepthTest = _DepthTest;
	Info.LifeTime = _Life;
	CRenderMgr::GetInst()->AddDebugShapeInfo(Info);
}

void DrawDebugSphere(const Vec3& _Pos, float _Radius, const Vec4& _Color, float _Life, bool _DepthTest)
{
	tDebugShapeInfo Info = {};
	Info.Shape = DEBUG_SHAPE::SPHERE;
	Info.vPos = _Pos;
	Info.vScale = Vec3(_Radius * 2.f, _Radius * 2.f, _Radius * 2.f);
	Info.vRot = Vec3(0.f, 0.f, 0.f);
	Info.LifeTime = _Life;
	Info.matWorld = XMMatrixScaling(Info.vScale.x, Info.vScale.y, Info.vScale.z)
		* XMMatrixTranslation(_Pos.x, _Pos.y, _Pos.z);
	Info.vColor = _Color;
	Info.DepthTest = _DepthTest;
	CRenderMgr::GetInst()->AddDebugShapeInfo(Info);
}

void DrawDebugSphere(const Matrix& _matWorld, const Vec4& _Color, float _Life, bool _DepthTest)
{
	tDebugShapeInfo Info = {};
	Info.Shape = DEBUG_SHAPE::SPHERE;
	Info.matWorld = _matWorld;
	Info.vColor = _Color;
	Info.DepthTest = _DepthTest;
	Info.LifeTime = _Life;
	CRenderMgr::GetInst()->AddDebugShapeInfo(Info);
}

void DrawDebugLine(const Vec3& _Start, const Vec3& _End, const Vec4& _Color, float _Life, bool _DepthTest)
{
	tDebugShapeInfo Info = {};
	Info.Shape = DEBUG_SHAPE::LINE;
	Info.vPos = _Start;
	Info.vScale = _End;
	Info.LifeTime = _Life;
	Info.vColor = _Color;
	Info.DepthTest = _DepthTest;
	CRenderMgr::GetInst()->AddDebugShapeInfo(Info);
}

void DrawDebugCone(const Matrix& _matWorld, const Vec4& _Color, float _Life, bool _DepthTest)
{
	tDebugShapeInfo info = {};
	info.Shape = DEBUG_SHAPE::CONE;
	info.matWorld = _matWorld;
	info.vColor = _Color;
	info.DepthTest = _DepthTest;
	info.LifeTime = _Life;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

Matrix GetMatrixFromFbxMatrix(FbxAMatrix& _mat)
{
	Matrix mat;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			mat.m[i][j] = (float)_mat.Get(i, j);
		}
	}
	return mat;
}

void SaveWString(const wstring& _String, FILE* _File)
{
	size_t len = _String.length();
	fwrite(&len, sizeof(size_t), 1, _File);
	fwrite(_String.c_str(), sizeof(wchar_t), len, _File);
}

void LoadWString(wstring& _String, FILE* _File)
{
	size_t len = 0;
	fread(&len, sizeof(size_t), 1, _File);

	_String.resize(len);
	fread((wchar_t*)_String.c_str(), sizeof(wchar_t), len, _File);
}

std::wstring ToWstring(const std::string& str)
{
	if (str.empty()) return std::wstring();

	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstr(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstr[0], size_needed);
	return wstr;
}

std::string ToString(const std::wstring& wstr)
{
	if (wstr.empty()) return std::string();

	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string str(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &str[0], size_needed, NULL, NULL);
	return str;
}