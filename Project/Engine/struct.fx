#pragma once
#ifndef _STRUCT
#define _STRUCT


struct tLight
{
    float4 Color;       // ���� ����
    float4 Ambient;     // ȯ�汤	
};

struct tLightInfo
{
    tLight  light;      // ���� ��������
    float3  WorldPos;   // ���� ��ġ
    float3  WorldDir;   // ������ �����ϴ� ����
    float   Radius;     // ������ �ݰ�
    float   Angle;      // ���� ���� ����
    uint    Type;       // ���� ����
    int3    padding;    // �е�
};

// Particle
struct tParticle
{
    float3  vLocalPos;
    float3  vWorldPos;
    float3  vWorldScale;
    float4  vColor;
    float3  vVelocity;
    float   Age;
    float   Life;
    float   NormalizedAge;
    int     Active;
};

struct tSpawnCount
{
    uint    iSpawnCount;
    uint3   padding;
};
#endif