#pragma once

struct Vtx
{
	Vec3	vPos;
	Vec4	vColor;
	Vec2	vUV;
};

struct tDebugShapeInfo
{
	DEBUG_SHAPE		Shape;
	Vec3			vPos;
	Vec3			vScale;
	Vec3			vRot;
	Matrix			matWorld;
	Vec4			vColor;
	float			LifeTime;
	float			Age;
	bool			DepthTest;
};

struct tTask
{
	TASK_TYPE	Type;
	DWORD_PTR	Param_0;
	DWORD_PTR	Param_1;
};

struct tLight
{
	Vec4	Color;		// 빛의 색상
	Vec4	Ambient;	// 환경광	
};

struct tLightInfo
{
	tLight		light;		// 광원 색상정보
	Vec3		WorldPos;	// 광원 위치
	Vec3		WorldDir;	// 광윈이 진행하는 방향
	float		Radius;		// 광원의 반경
	float		Angle;		// 광원 범위 각도
	LIGHT_TYPE	Type;		// 광원 종류
	int			padding[3];
};

struct tPixel
{
	BYTE b;
	BYTE g;
	BYTE r;
	BYTE a;
};

struct tTransform
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;

	Matrix matWV;
	Matrix matWVP;
};

struct tMtrlConst
{
	int		iArr[4];
	float	fArr[4];
	Vec2	v2Arr[4];
	Vec4	v4Arr[4];
	Matrix	matArr[4];
	int		btex[TEX_PARAM::END + 2];
};

struct tSpriteInfo
{
	Vec2 LeftTopUV;
	Vec2 SliceUV;
	Vec2 BackGroundUV;
	Vec2 OffsetUV;
	int	 UseAnimator2D;
	int  Padding[3];
};

extern tTransform g_Trans;

struct tGlobalData
{
	// 시간 관련 정보
	float   g_DT;
	float   g_EngineDT;
	float   g_Time;
	float   g_EngineTime;

	// 렌더타겟의 해상도 정보
	Vec2	g_Resolution;

	// 바인딩 된 구조화버퍼에 광원이 몇개 들어있는지
	int     g_Light2DCount;
	int     g_Light3DCount;
};
extern tGlobalData g_GlobalData;