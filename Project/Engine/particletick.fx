#ifndef _PARTICLE_TICK
#define _PARTICLE_TICK

#include "value.fx"
#include "struct.fx"

RWStructuredBuffer<tParticle> ParticleBuffer : register(u0);
RWStructuredBuffer<tSpawnCount> SpawnCountBuffer : register(u1);
Texture2D NoiseTex : register(t20);

#define ParticleObjectPos   g_vec4_0.xyz
#define MAX_COUNT           g_int_0
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
            // 파티클의 위치를 특정 범위내에서 랜덤한 위치로 잡아준다.                
            float2 vUV = (float2) 0.f;

            // 스레드를 UV 로 맵핑하기위해서 ID 를 0~1 범위로 정규화                
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
            Particle.Life = 2.f;
            Particle.NormalizedAge = 0.f;
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