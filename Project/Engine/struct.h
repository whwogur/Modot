#pragma once

struct Vtx
{
	Vec3	vPos;
	Vec4	vColor;
	Vec2	vUV;

	Vec3	vTangent;	// ����
	Vec3	vNormal;	// ����
	Vec3	vBinormal;  // ������

	Vec4	vWeights;  // Bone ����ġ
	Vec4	vIndices;  // Bone �ε���
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
	Vec4	Color;		// ���� ����
	Vec4	Ambient;	// ȯ�汤
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
	{}

	tLight		light;		// ���� ��������
	Vec3		WorldPos;	// ���� ��ġ
	Vec3		WorldDir;	// ������ �����ϴ� ����
	float		Radius;		// ������ �ݰ�
	float		Angle;		// ���� ���� ����
	LIGHT_TYPE	Type;		// ���� ����
	
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

	float	NoiseForceAccTime;	// Noise Force �ҿ� �����ϴ� ���� �ð�
	Vec3	NoiseForceDir;		// Noise Force �� ����

	float	Mass;
	float	Life;
	float	Age;
	float	NormalizedAge;
	int		Active;

	float	Rotation;
	float	RotationSpeed;
};

// Material ���
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

	// 3D Animation ����
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
	// �ð� ���� ����
	float   g_DT;
	float   g_EngineDT;
	float   g_Time;
	float   g_EngineTime;

	// ī�޶� ���� ��ġ
	Vec4	g_CamWorldPos;
	// ����Ÿ���� �ػ� ����
	Vec2	g_Resolution;
	// ���ε� �� ����ȭ���ۿ� ������ � ����ִ���
	int     g_Light2DCount;
	int     g_Light3DCount;
};
extern tGlobalData g_GlobalData;

// Particle Module
struct tParticleModule
{
	// Spawn
	UINT	SpawnRate;				// �ʴ� ��ƼŬ �������� (Spawn Per Second)
	Vec4	vSpawnColor;			// ���� ���� ����
	Vec4	vSpawnMinScale;			// ���� �� �ּ� ũ��
	Vec4	vSpawnMaxScale;			// ���� �� �ִ� ũ��

	float	MinLife;				// �ּ� ����
	float	MaxLife;				// �ִ� ����

	UINT	SpawnShape;				// 0 : Box,  1: Sphere
	Vec3	SpawnShapeScale;		// SpawnShapeScale.x == Radius

	UINT	BlockSpawnShape;		// 0 : Box,  1: Sphere
	Vec3	BlockSpawnShapeScale;	// SpawnShapeScale.x == Radius

	UINT	SpaceType;				// 0 : LocalSpace, 1 : WorldSpace

	// Spawn Burst
	UINT	SpawnBurstCount;		// �ѹ��� �߻���Ű�� Particle ��
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
	float	NoiseForceTerm;		// Noise Force �����Ű�� ��
	float	NoiseForceScale;	// Noise Force ũ��

	// Render Module
	Vec3	EndColor;			// ���� ����
	UINT	FadeOut;			// 0 : Off, 1 : Normalized Age
	float	FadeOutStartRatio;	// FadeOut ȿ���� ���۵Ǵ� Normalized Age ����
	UINT	VelocityAlignment;	// �ӵ� ���� 0 : Off, 1 : On

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
	Matrix						matOffset;	// Inverse ���( Skin ������ -> �⺻���·� �ǵ���)
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
	int							iTitleIndex;
	FbxTime::EMode				eMode;
};