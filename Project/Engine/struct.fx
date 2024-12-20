#ifndef _STRUCT
#define _STRUCT


struct tLight
{
    float4 Color;       // ���� ����
    float4 Ambient;     // ȯ�汤	
    float SpecCoefficient;
};

struct tLightInfo
{
    tLight  light;      // ���� ��������
    float3  WorldPos;   // ���� ��ġ
    float3  WorldDir;   // ������ �����ϴ� ����
    float   Radius;     // ������ �ݰ�
    float   Angle;      // ���� ���� ����
    uint    Type;       // ���� ����
    
    float   ConeOuter;
    float   ConeInner;
    
    float   Falloff;
    int3    Padding;
};

// Particle
struct tParticle
{
    float4 vColor;
    float3 vLocalPos;
    float3 vWorldPos;
    float3 vWorldInitScale;
    float3 vWorldCurrentScale;
    float3 vWorldRotation;
    
    float3 vForce;
    float3 vVelocity;
        
    float   NoiseForceAccTime; // Noise Force �ҿ� �����ϴ� ���� �ð�
    float3  NoiseForceDir; // ����� Noise Forec �� ����
            
    float   Mass;
    float   Life;
    float   Age;
    float   NormalizedAge;
    int     Active;
    
    float   Rotation;
    float   RotationSpeed;
};

struct tSpawnCount
{
    uint    iSpawnCount;
    uint3   padding;
};

struct tParticleModule
{
    // Spawn
    uint    SpawnRate;              // �ʴ� ��ƼŬ ���� ����
    float4  vSpawnColor;            // ���� ���� ����
    float4  vSpawnMinScale;         // ���� �� �ּ� ũ��
    float4  vSpawnMaxScale;         // ���� �� �ִ� ũ��

    float   MinLife;                // �ּ� ����
    float   MaxLife;                // �ִ� ����

    uint    SpawnShape;             // 0 : Box,  1: Sphere
    float3  SpawnShapeScale;        // SpawnShapeScale.x == Radius

    uint    BlockSpawnShape;        // 0 : Box,  1: Sphere
    float3  BlockSpawnShapeScale;   // SpawnShapeScale.x == Radius    

    uint    SpaceType;              // 0 : LocalSpcae, 1 : WorldSpace

    // Spawn Burst
    uint    SpawnBurstCount;        // �ѹ��� �߻���Ű�� Particle ��
    uint    SpawnBurstRepeat;
    float   SpawnBurstRepeatTime;

    // Add Velocity
    uint    AddVelocityType;        // 0 : Random, 1 : FromCenter, 2 : ToCenter, 4 : Fixed 
    float3  AddVelocityFixedDir;
    float   AddMinSpeed;
    float   AddMaxSpeed;

    // Scale Module
    float   StartScale;
    float   EndScale;

    // Drag Module
    float   DestNormalizedAge;
    float   LimitSpeed;

    // Noise Force Module
    float   NoiseForceTerm;         // Noise Force �����Ű�� ��
    float   NoiseForceScale;        // Noise Force �� ũ��

    // Render Module
    float3  EndColor;               // ���� ����
    uint    FadeOut;                // 0 : Off, 1 : Normalized Age
    float   StartRatio;             // FadeOut ȿ���� ���۵Ǵ� Normalized Age ����
    uint    VelocityAlignment;      // �ӵ� ���� 0 : Off, 1 : On

    float   MaxRotationSpeed;
    int     Gyrate;
    float   GyrateSpeed;
    
	// Module On / Off
    int     Module[8];
};

struct tRaycastOut
{
    float4  WorldPos;
    float2  LocationUV;
    uint    Distance;
    int     Success;
};

struct tWeight8
{
    float arrWeight[8];
};

struct tSkinningInfo
{
    float3 vPos;
    float3 vPrevPos;
    float3 vTangent;
    float3 vBinormal;
    float3 vNormal;
};

#endif