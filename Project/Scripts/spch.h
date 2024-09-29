#ifndef SPCH_H
#define SPCH_H

// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"

#ifdef _DEBUG
#pragma comment(lib, "Engine\\Engine_D")
#else
#pragma comment(lib, "Engine\\Engine")
#endif

#include <Engine/global.h>
#include <Engine/CEngine.h>
#include "CScriptMgr.h"
#include "../Client/CPlayerManager.h"

#define PLAY_BGM(SOUND) CPlayerManager::GetInst()->PlayBGM(SOUND)
#define PLAY_EFFECT(SOUND) CPlayerManager::GetInst()->PlayEffect(SOUND)

constexpr float PI = 3.14159265359;
#endif //PCH_H
