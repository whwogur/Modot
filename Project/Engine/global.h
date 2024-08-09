#pragma once


#include <Windows.h>

#include <d3d11.h>
#include <dxgi.h>
#pragma comment(lib, "dxgi.lib")
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#include <wrl.h>
namespace WRL = Microsoft::WRL;

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

// DirectxTex
#include <DirectxTex/DirectXTex.h>

#ifdef _DEBUG
#pragma comment(lib, "DirectxTex//DirectXTex_debug.lib")
#else
#pragma comment(lib, "DirectxTex//DirectXTex.lib")
#endif
#include "Instrumentor.h"
#include "SimpleMath.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;

using std::string;
using std::wstring;

#include <vector>
using std::vector;

#include <list>
using std::list;

#include <map>
using std::map;
using std::make_pair;

#include <filesystem>
using namespace std::filesystem;

#ifdef _DEBUG
#define MD_ENABLE_ASSERTS
#endif

#ifdef MD_ENABLE_ASSERTS
#define MD_ASSERT(x, ...) { if(!(x)) { MD_ERROR(L"Assert 실패!: {0}", __VA_ARGS__); __debugbreak(); } }
#define MD_ENGINE_ASSERT(x, ...) { if(!(x)) { MD_ENGINE_ERROR(L"Assert 실패!: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define MD_ASSERT(...)
#define MD_ENGINE_ASSERT(...)
#endif

#include "singleton.h"
#include "define.h"
#include "enum.h"
#include "struct.h"
#include "func.h"
#include "Ptr.h"
