#pragma once

struct Vtx
{
	Vec3	vPos;
	Vec4	vColor;
	Vec2	vUV;

	Vec3	vTangent;	// 접선
	Vec3	vNormal;	// 법선
	Vec3	vBinormal;  // 종법선

	Vec4	vWeights;  // Bone 가중치
	Vec4	vIndices;  // Bone 인덱스
};

struct tPrimitiveInfo
{
	std::vector<Vtx> vertices;
	std::vector<UINT> indices;
};

struct tDebugShapeInfo
{
	tDebugShapeInfo()
		: Shape(DEBUG_SHAPE::RECT)
		, vPos{}
		, vScale{}
		, vRot{}
		, matWorld{}
		, vColor{}
		, LifeTime(0.0f)
		, Age(0.0f)
		, DepthTest(true)
	{}

	tDebugShapeInfo(DEBUG_SHAPE shape, const Vec3& pos, const Vec3& scale, const Vec3& rot, const Matrix& world, const Vec4& color, float lifetime, bool depthTest)
		: Shape(shape)
		, vPos(pos)
		, vScale(scale)
		, vRot(rot)
		, matWorld(world)
		, vColor(color)
		, LifeTime(lifetime)
		, Age(0.0f)
		, DepthTest(depthTest)
	{}
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
	float   SpecCoefficient;
};

struct tLightInfo
{
	tLightInfo() = default;

	tLightInfo(const tLight& lightInfo, const Vec3& pos, const Vec3& dir, float radius, float angle, LIGHT_TYPE type, float outer, float inner, float falloff)
		: light(lightInfo)
		, WorldPos(pos)
		, WorldDir(dir)
		, Radius(radius)
		, Angle(angle)
		, Type(type)
		, ConeOuter(outer)
		, ConeInner(inner)
		, Falloff(falloff)
		, Padding{0,}
	{}

	tLight		light;		// 광원 색상정보
	Vec3		WorldPos;	// 광원 위치
	Vec3		WorldDir;	// 광윈이 진행하는 방향
	float		Radius;		// 광원의 반경
	float		Angle;		// 광원 범위 각도
	LIGHT_TYPE	Type;		// 광원 종류
	
	float		ConeOuter;
	float		ConeInner;
	float		Falloff;
	int			Padding[3];
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

	Matrix matWorldInv;
	Matrix matViewInv;
	Matrix matProjInv;

	Matrix matWV;
	Matrix matWVP;
};

// Particle
struct tParticle
{
	Vec4	vColor;
	Vec3	vLocalPos;
	Vec3	vWorldPos;
	Vec3	vWorldInitScale;
	Vec3	vWorldCurrentScale;
	Vec3	vWorldRotation;

	Vec3	vForce;

	Vec3	vVelocity;

	float	NoiseForceAccTime;	// Noise Force 텀에 도달하는 누적 시간
	Vec3	NoiseForceDir;		// Noise Force 의 방향

	float	Mass;
	float	Life;
	float	Age;
	float	NormalizedAge;
	int		Active;

	float	Rotation;
	float	RotationSpeed;
};

// Material 계수
struct tMtrlData
{
	Vec4 vDiff;
	Vec4 vSpec;
	Vec4 vAmb;
	Vec4 vEmv;
};

struct tMtrlConst
{
	tMtrlData	mtrl;
	int			iArr[4];
	float		fArr[4];
	Vec2		v2Arr[4];
	Vec4		v4Arr[4];
	Matrix		matArr[4];
	int			btex[TEX_PARAM::END];

	// 3D Animation 정보
	int			arrAnimData[2];
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

	// 카메라 월드 위치
	Vec4	g_CamWorldPos;
	// 렌더타겟의 해상도 정보
	Vec2	g_Resolution;
	// 바인딩 된 구조화버퍼에 광원이 몇개 들어있는지
	int     g_Light2DCount;
	int     g_Light3DCount;
};
extern tGlobalData g_GlobalData;

// Particle Module
struct tParticleModule
{
	// Spawn
	UINT	SpawnRate;				// 초당 파티클 생성개수 (Spawn Per Second)
	Vec4	vSpawnColor;			// 생성 시점 색상
	Vec4	vSpawnMinScale;			// 생성 시 최소 크기
	Vec4	vSpawnMaxScale;			// 생성 시 최대 크기

	float	MinLife;				// 최소 수명
	float	MaxLife;				// 최대 수명

	UINT	SpawnShape;				// 0 : Box,  1: Sphere
	Vec3	SpawnShapeScale;		// SpawnShapeScale.x == Radius

	UINT	BlockSpawnShape;		// 0 : Box,  1: Sphere
	Vec3	BlockSpawnShapeScale;	// SpawnShapeScale.x == Radius

	UINT	SpaceType;				// 0 : LocalSpace, 1 : WorldSpace

	// Spawn Burst
	UINT	SpawnBurstCount;		// 한번에 발생시키는 Particle 수
	UINT	SpawnBurstRepeat;
	float	SpawnBurstRepeatTime;

	// Add Velocity
	UINT	AddVelocityType;		// 0 : Random, 1 : FromCenter, 2 : ToCenter, 4 : Fixed 
	Vec3	AddVelocityFixedDir;
	float	AddMinSpeed;
	float	AddMaxSpeed;

	// Scale Module
	float	StartScale;
	float	EndScale;

	// Drag Module
	float	DestNormalizedAge;
	float	LimitSpeed;

	// Noise Force Module
	float	NoiseForceTerm;		// Noise Force 적용시키는 텀
	float	NoiseForceScale;	// Noise Force 크기

	// Render Module
	Vec3	EndColor;			// 최종 색상
	UINT	FadeOut;			// 0 : Off, 1 : Normalized Age
	float	FadeOutStartRatio;	// FadeOut 효과가 시작되는 Normalized Age 지점
	UINT	VelocityAlignment;	// 속도 정렬 0 : Off, 1 : On

	float	MaxRotationSpeed;
	int		Gyrate;
	float	GyrateSpeed;
	// Module On / Off
	int		Module[(UINT)PARTICLE_MODULE::END];
};

struct tRay
{
	Vec3 vStart;
	Vec3 vDir;
};

// ============
// Animation 3D
// ============
struct tFrameTrans
{
	Vec4	vTranslate;
	Vec4	vScale;
	Vec4	qRot;
};
struct tMTKeyFrame
{
	double	dTime;
	int		iFrame;
	Vec3	vTranslate;
	Vec3	vScale;
	Vec4	qRot;
};

struct tMTBone
{
	wstring						strBoneName;
	int							iDepth;
	int							iParentIndx;
	Matrix						matOffset;	// Inverse 행렬( Skin 정점을 -> 기본상태로 되돌림)
	Matrix						matBone;
	std::vector<tMTKeyFrame>	vecKeyFrame;
};

struct tMTAnimClip
{
	wstring						strAnimName;
	int							iStartFrame;
	int							iEndFrame;
	int							iFrameLength;
	double						dStartTime;
	double						dEndTime;
	double						dTimeLength;
	int							iMultipleClips = 0; // 0: 한 클립에 애니메이션 하나 1: 한 클립에 애니메이션 여러개
	FbxTime::EMode				eMode;
};

// ===========
// Instancing
// ===========
union uInstID
{
	struct
	{
		UINT iMesh;
		WORD iMtrl;
		WORD iMtrlIdx;
	}; ULONG64 llID;
};

class CGameObject;
struct tInstObj
{
	tInstObj() = default;
	tInstObj(CGameObject* _pObj, UINT _iMtrlIdx)
		: pObj(_pObj)
		, iMtrlIdx(_iMtrlIdx)
	{}

	CGameObject* pObj = nullptr;
	UINT		 iMtrlIdx = 0;
};
struct tInstancingData
{
	Matrix matWorld;
	Matrix matWV;
	Matrix matWVP;
	int    iRowIdx;
};