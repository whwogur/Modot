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
	Vec4	Color;		// ���� ����
	Vec4	Ambient;	// ȯ�汤	
};

struct tLightInfo
{
	tLight		light;		// ���� ��������
	Vec3		WorldPos;	// ���� ��ġ
	Vec3		WorldDir;	// ������ �����ϴ� ����
	float		Radius;		// ������ �ݰ�
	float		Angle;		// ���� ���� ����
	LIGHT_TYPE	Type;		// ���� ����
	int			padding[3];
};

struct tTransform
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;

	Matrix matWV;
	Matrix matWVP;
};

// Particle
struct tParticle
{
	Vec3	vLocalPos;
	Vec3	vWorldPos;
	Vec4	vColor;

	Vec3	vForce;
	Vec3	vVelocity;

	float	Mass;
	int		Active;

	int		Padding[2];
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
	// �ð� ���� ����
	float   g_DT;
	float   g_EngineDT;
	float   g_Time;
	float   g_EngineTime;

	// ����Ÿ���� �ػ� ����
	Vec2	g_Resolution;

	// ���ε� �� ����ȭ���ۿ� ������ � ����ִ���
	int     g_Light2DCount;
	int     g_Light3DCount;
};
extern tGlobalData g_GlobalData;