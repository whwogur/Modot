#ifndef _PARTICLE_TICK
#define _PARTICLE_TICK

#include "value.fx"
#include "struct.fx"

RWStructuredBuffer<tParticle> ParticleBuffer : register(u0);
RWStructuredBuffer<tSpawnCount> SpawnCountBuffer : register(u1);

#define MAX_COUNT       g_int_0
#define Particle        ParticleBuffer[_ID.x]

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
            Particle.Active = true;
        }
    }
    else
    {
        Particle.vWorldPos += Particle.vVelocity * g_EngineDT;
    }
}

#endif