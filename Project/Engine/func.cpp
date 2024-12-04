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

std::string MatrixToString(const Matrix& matrix, int precision)
{
	std::ostringstream oss;
	oss.precision(precision);
	oss << std::fixed;

	
	for (int row = 0; row < 4; ++row)
	{
		oss << "[";
		for (int col = 0; col < 4; ++col)
		{
			oss << matrix.m[row][col];
			if (row != 3 || col != 3)
				oss << ", ";
		}
		oss << "]\n";
	}

	return oss.str();
}

void MakeLookAtWorldMatrix(const Matrix& _Mat, const Vec3& _Pos, Matrix* _Container)
{
	// 대상 위치
	Vec3 targetPosition(_Mat._41, _Mat._42, _Mat._43);

	// 방향 벡터 (Forward)
	Vec3 direction = targetPosition - _Pos;
	direction.Normalize();

	// 상향 벡터 (Up)
	Vec3 up(0.f, 1.f, 0.f);

	// Right 벡터 계산
	Vec3 right = up.Cross(direction);
	right.Normalize();

	// Up 벡터 재계산 (정확한 직교화)
	up = direction.Cross(right);
	up.Normalize();

	// LookAt 행렬 생성 (회전 부분)
	Matrix rotationMatrix(
		right.x, up.x, direction.x, 0.0f,
		right.y, up.y, direction.y, 0.0f,
		right.z, up.z, direction.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	// 위치 변환 (Translation)
	Matrix translationMatrix(Matrix::CreateTranslation(_Pos));

	// 최종 월드 행렬 조합
	*_Container = rotationMatrix * translationMatrix;
}

void GetOffsetPosition(const Matrix& _Mat, float _Distance, Vec3* _Container)
{
	// Forward 벡터 추출 (_31, _32, _33)
	Vec3 forward(_Mat._31, _Mat._32, _Mat._33);
	forward.Normalize();

	// Forward 방향으로 거리만큼 떨어진 위치 계산
	Vec3 targetPosition(_Mat._41, _Mat._42, _Mat._43);

	*_Container = targetPosition + (forward * _Distance);
}

void MakeLookAtWorldMatrix(const Matrix& _Mat, float _Distance, Matrix* _Container)
{
	Vec3 offsetPosition;
	GetOffsetPosition(_Mat, _Distance, &offsetPosition);

	MakeLookAtWorldMatrix(_Mat, offsetPosition, _Container);
}

Vec3 ExtractScale(const Matrix& _Mat)
{
	return {
		XMVectorGetX(XMVector3Length(XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&_Mat._11)))),
		XMVectorGetX(XMVector3Length(XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&_Mat._21)))),
		XMVectorGetX(XMVector3Length(XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&_Mat._31))))
	};
}

Vec3 ExtractRotation(const Matrix& _Mat)
{
	XMFLOAT3 scale = ExtractScale(_Mat);
	Matrix rotMatrix = _Mat;
	rotMatrix._11 /= scale.x; rotMatrix._12 /= scale.x; rotMatrix._13 /= scale.x;
	rotMatrix._21 /= scale.y; rotMatrix._22 /= scale.y; rotMatrix._23 /= scale.y;
	rotMatrix._31 /= scale.z; rotMatrix._32 /= scale.z; rotMatrix._33 /= scale.z;

	// 오일러 각도 추출
	float pitch = atan2(-rotMatrix._31, sqrt(rotMatrix._32 * rotMatrix._32 + rotMatrix._33 * rotMatrix._33));
	float yaw = atan2(rotMatrix._21, rotMatrix._11);
	float roll = atan2(rotMatrix._32, rotMatrix._33);

	return { pitch, yaw, roll };
}

Vec3 ExtractPosition(const Matrix& _Mat)
{
	return { _Mat._41, _Mat._42, _Mat._43 };
}
