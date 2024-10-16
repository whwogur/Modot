#pragma once

void CreateObject(class CGameObject* _NewObject, int _LayerIndex);
void DeleteObject(CGameObject* _DeleteObject);
void ChangeLevelState(LEVEL_STATE _NextState);
void ChangeLevel(class CLevel* _Level, LEVEL_STATE _NextLevelState);
bool IsValid(CGameObject*& _Object);
void DrawDebugCircle(Vec3 _Pos, float _Radius, Vec4 _Color, float _Life, bool _DepthTes);
void DrawDebugRect(Vec3 _Pos, Vec3 _Scale, Vec3 _Rot, Vec4 _Color, float _Life, bool _DepthTest);
void DrawDebugRect(const Matrix& _matWorld, Vec4 _Color, float _Life, bool _DepthTest);

void SaveWString(const wstring& _String, FILE* _File);
void LoadWString(wstring& _String, FILE* _File);
wstring ToWstring(const string& str);
string ToString(const wstring& wstr);

template<typename T, int _Size>
void Delete_Array(T(&_arr)[_Size])
{
	for (int i = 0; i < _Size; ++i)
	{
		if (nullptr != _arr[i])
			delete _arr[i];
	}
}



template<typename T>
void Delete_Vec(std::vector<T*>& _vec)
{
	for (size_t i = 0; i < _vec.size(); ++i)
	{
		if (nullptr != _vec[i])
			delete _vec[i];
	}
	_vec.clear();
}


template<typename T1, typename T2>
void Delete_Map(map<T1, T2>& _map)
{
	for (const auto& pair : _map)
	{
		if (nullptr != pair.second)
			delete pair.second;
	}

	_map.clear();
}

#include "assets.h"
template<typename T>
ASSET_TYPE GetAssetType()
{
	if constexpr (std::is_same_v<T, CMesh>)
		return ASSET_TYPE::MESH;
	//if constexpr (std::is_same_v<T, CMeshData>)
	//	return ASSET_TYPE::MESH_DATA;
	if constexpr (std::is_same_v<T, CMaterial>)
		return ASSET_TYPE::MATERIAL;
	if constexpr (std::is_same_v<T, CPrefab>)
		return ASSET_TYPE::PREFAB;
	if constexpr (std::is_same_v<T, CTexture>)
		return ASSET_TYPE::TEXTURE;
	if constexpr (std::is_same_v<T, CSound>)
		return ASSET_TYPE::SOUND;
	if constexpr (std::is_same_v<T, CGraphicShader>)
		return ASSET_TYPE::GRAPHIC_SHADER;
	if constexpr (std::is_same_v<T, CSprite>)
		return ASSET_TYPE::SPRITE;
	if constexpr (std::is_same_v<T, CAnimation>)
		return ASSET_TYPE::ANIMATION;
	if constexpr(std::is_same_v<T, CComputeShader>)
		return ASSET_TYPE::COMPUTE_SHADER;
}