#ifndef _PARTICLE_TICK
#define _PARTICLE_TICK

#include "value.fx"
#include "struct.fx"

RWStructuredBuffer<tParticle> ParticleBuffer : register(u0);
RWStructuredBuffer<tSpawnCount> SpawnCountBuffer : register(u1);
Texture2D NoiseTex : register(t20);

#define ParticleObjectPos   g_vec4_0.xyz
#define MAX_COUNT           g_int_0
#define CSType              g_int_1
#define Particle            ParticleBuffer[_ID.x]

[numthreads(1024, 1, 1)]
void CS_ParticleTick(int3 _ID : SV_DispatchThreadID)
{
    if (_ID.x >= MAX_COUNT)
        return;

    if (!Particle.Active)
    {
        int SpawnCount = 0;
        InterlockedAdd(SpawnCountBuffer[0].iSpawnCount, -1, SpawnCount);

        if (SpawnCount > 0)
        {
            switch (CSType)
            {
                case 0:
                {
                    // ��ƼŬ�� ��ġ�� Ư�� ���������� ������ ��ġ�� ����ش�.                
                        float2 vUV = (float2) 0.f;

                    // �����带 UV �� �����ϱ����ؼ� ID �� 0~1 ������ ����ȭ                
                    float RandomFactor = frac(sin(dot(_ID.xy, float2(12.9898, 78.233))) * 43758.5453);

                    vUV.x = ((float) _ID.x / (float) (MAX_COUNT - 1)) + RandomFactor * g_EngineTime * 0.5f;
                    vUV.y = sin(vUV.x * 20 * PI) * 0.5f + RandomFactor * g_EngineTime * 0.1f;

                    float3 vNoise = NoiseTex.SampleLevel(g_sam_1, vUV, 0).xyz;

                    float BoxScale = 300.f;

                    float3 vRandomPos = (float3) 0.f;

                    vRandomPos.x = vNoise.x * BoxScale - (BoxScale / 2.f);
                    vRandomPos.y = vNoise.y * BoxScale - (BoxScale / 2.f);
                    vRandomPos.z = vNoise.z * BoxScale - (BoxScale / 2.f);

                    Particle.Active = true;
                    Particle.vLocalPos = vRandomPos;
                    Particle.vWorldPos = Particle.vLocalPos + ParticleObjectPos;
                    Particle.Age = 0.f;
                    Particle.Life = 10.f;
                    Particle.NormalizedAge = 0.f;
                    Particle.vVelocity = float3(0.0f, 0.0f, 0.0f);
                    break;
                }
                case 1:
                {
                    // ������׷��� ī�޶� ���������� �ڳʸ� ���
                    float3 frustumCorners[4];
                        frustumCorners[0] = mul(mul(matProj, matView), float4(-1, -1, 0, 1)).xyz; // ���ϴ�
                        frustumCorners[1] = mul(mul(matProj, matView), float4(1, -1, 0, 1)).xyz; // ���ϴ�
                        frustumCorners[2] = mul(mul(matProj, matView), float4(-1, 1, 0, 1)).xyz; // �»��
                        frustumCorners[3] = mul(mul(matProj, matView), float4(1, 1, 0, 1)).xyz; // ����

                    // ī�޶��� �ּ� �� �ִ� ���� ���
                    float2 cameraMin = float2(min(frustumCorners[0].x, frustumCorners[2].x), min(frustumCorners[0].y, frustumCorners[1].y));
                    float2 cameraMax = float2(max(frustumCorners[1].x, frustumCorners[3].x), max(frustumCorners[2].y, frustumCorners[3].y));

                    // ��ƼŬ�� ��ġ�� Ư�� ���� ������ ������ ��ġ�� ����
                    float2 vUV = (float2) 0.f;

                    // UV ������ ���� ID�� 0~1 ������ ����ȭ
                    float RandomFactor = frac(sin(dot(_ID.xy, float2(12.9898, 78.233))) * 43758.5453);
                    float3 vNoise = NoiseTex.SampleLevel(g_sam_1, vUV, 0).xyz;
                    
                    vUV.x = ((float) _ID.x / (float) (MAX_COUNT - 1)) + RandomFactor * g_EngineTime * 0.5f;

                    // ī�޶��� ��ܿ��� ������ x ��ġ�� ��ƼŬ ����
                    float3 vRandomPos = (float3) 0.f;
                    vRandomPos.x = lerp(cameraMin.x, cameraMax.x, vUV.x);
                    vRandomPos.y = cameraMax.y; // ȭ�� ��ܿ��� ����
                    vRandomPos.z = 0.0f; // z���� ������� ����

                    Particle.Active = true;
                    Particle.vLocalPos = vRandomPos;
                    Particle.vWorldPos = Particle.vLocalPos + ParticleObjectPos;
                    Particle.Age = 0.f;
                    Particle.Life = 2.f;
                    Particle.NormalizedAge = 0.f;

                    // ��ƼŬ�� �ʱ� �ӵ� ���� (�Ʒ��� ����������)
                    Particle.vVelocity = float3(0.0f, -50.0f, 0.0f); // y�� �������� �������� �ӵ� ����
                    break;
                }
            }
            
        }
    }
    else
    {
        Particle.vWorldPos += Particle.vVelocity * g_EngineDT;
        
        Particle.Age += g_EngineDT;
        if (Particle.Life <= Particle.Age)
        {
            Particle.Active = false;
        }
    }
}

#endif