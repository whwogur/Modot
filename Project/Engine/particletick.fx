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
    if (MAX_COUNT <= _ID.x)
        return;

    if (Particle.Active == false)
    {
        int SpawnCount = SpawnCountBuffer[0].iSpawnCont;

        while (0 < SpawnCount)
        {
            int Origin = 0;
            InterlockedCompareExchange(SpawnCountBuffer[0].iSpawnCont
                                      , SpawnCount
                                      , SpawnCountBuffer[0].iSpawnCont - 1
                                      , Origin);

            if (SpawnCount == Origin)
            {
                Particle.Active = true;
                break;
            }

            SpawnCount = SpawnCountBuffer[0].iSpawnCont;
        }
    }
    else
    {
        Particle.vWorldPos += Particle.vVelocity * g_EngineDT;
    }
}







#endif