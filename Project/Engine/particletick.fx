#ifndef _PARTICLE_TICK
#define _PARTICLE_TICK

#include "value.fx"
#include "struct.fx"
#include "func.fx"


RWStructuredBuffer<tParticle> ParticleBuffer : register(u0);
RWStructuredBuffer<tSpawnCount> SpawnCountBuffer : register(u1);
Texture2D NoiseTex : register(t20);
StructuredBuffer<tParticleModule> Module : register(t21);

#define ParticleObjectPos   g_vec4_0.xyz
#define MAX_COUNT           g_int_0
#define Particle            ParticleBuffer[_ID.x]


// Module Check
#define SpawnModule         Module[0].Module[0]
#define SpawnBurstModule    Module[0].Module[1]
#define SpawnShapeType      Module[0].SpawnShape
#define AddVelocityModule   Module[0].Module[2]
#define ScaleModule         Module[0].Module[3]
#define DragModule          Module[0].Module[4]
#define NoiseForce          Module[0].Module[5]
#define Render              Module[0].Module[6]


[numthreads(1024, 1, 1)]
void CS_ParticleTick(int3 _ID : SV_DispatchThreadID)
{
    if (MAX_COUNT <= _ID.x)
        return;
    
    if (false == Particle.Active)
    {
        int SpawnCount = SpawnCountBuffer[0].iSpawnCount;
                
        while (0 < SpawnCount)
        {
            int Origin = 0;
            
            InterlockedCompareExchange(SpawnCountBuffer[0].iSpawnCount
                                      , SpawnCount
                                      , SpawnCountBuffer[0].iSpawnCount - 1
                                      , Origin);

            if (SpawnCount == Origin)
            {
                // ��ƼŬ�� ��ġ�� Ư�� ���������� ������ ��ġ�� ����ش�.                
                float2 vUV = (float2) 0.f;
                
                // �����带 UV �� �����ϱ����ؼ� ID �� 0~1 ������ ����ȭ     
                float3 vRandom0 = GetRandom(NoiseTex, _ID.x, MAX_COUNT);
                float3 vRandom1 = GetRandom(NoiseTex, _ID.x + 1, MAX_COUNT);
                float3 vRandom2 = GetRandom(NoiseTex, _ID.x + 2, MAX_COUNT);
                                
                float3 vSpawnPos = (float3) 0.f;
                
                // 0 : Box,  1 : Sphere
                if (0 == SpawnShapeType)
                {
                    vSpawnPos.x = vRandom0.x * Module[0].SpawnShapeScale.x - (Module[0].SpawnShapeScale.x / 2.f);
                    vSpawnPos.y = vRandom0.y * Module[0].SpawnShapeScale.y - (Module[0].SpawnShapeScale.y / 2.f);
                    vSpawnPos.z = vRandom0.z * Module[0].SpawnShapeScale.z - (Module[0].SpawnShapeScale.z / 2.f);
                }
                else if (1 == SpawnShapeType)
                {
                    float fRadius = Module[0].SpawnShapeScale.x;
                    float fBlockRadius = Module[0].BlockSpawnShapeScale.x;
                    float fDifferRadius = fRadius - fBlockRadius;
                        
                    vSpawnPos = normalize(vRandom1 - 0.5f) * fDifferRadius * vRandom2.x 
                                    + normalize(vRandom1 - 0.5f) * fBlockRadius;
                }
                                                        
                // Add Velocity Module
                Particle.vVelocity = (float3) 0.f;
                
                if (AddVelocityModule)
                {
                    float fSpeed = Module[0].AddMinSpeed + (Module[0].AddMaxSpeed - Module[0].AddMinSpeed) * vRandom2.x;
                        
                    // Random
                    if (0 == Module[0].AddVelocityType)                        
                        Particle.vVelocity = normalize(vRandom2 - 0.5f) * fSpeed;
                    // FromCenter
                    else if (1 == Module[0].AddVelocityType)                        
                        Particle.vVelocity = normalize(vSpawnPos) * fSpeed;
                    // ToCenter
                    else if (2 == Module[0].AddVelocityType)
                        Particle.vVelocity = -normalize(vSpawnPos) * fSpeed;
                    // Fixed
                    else
                        Particle.vVelocity = normalize(Module[0].AddVelocityFixedDir) * fSpeed;
                }
                    
                    
                Particle.vLocalPos = vSpawnPos;
                Particle.vWorldPos = Particle.vLocalPos + ParticleObjectPos.xyz;
                Particle.vWorldInitScale = (Module[0].vSpawnMaxScale - Module[0].vSpawnMinScale) * vRandom0.x + Module[0].vSpawnMinScale;
                                    
                Particle.vColor = Module[0].vSpawnColor;
                Particle.Mass = 1.f;
                
                Particle.Age = 0.f;
                Particle.NormalizedAge = 0;
                Particle.Life = (Module[0].MaxLife - Module[0].MinLife) * vRandom1.y + Module[0].MinLife;
                Particle.Active = 1;
                
                break;
            }
            
            SpawnCount = SpawnCountBuffer[0].iSpawnCount;
        }
    }
     
    // �����尡 ����ϴ� ��ƼŬ�� Ȱ��ȭ ���¸� Tick �� �����Ѵ�.
    else
    {
        Particle.vForce = float3(0.f, 0.f, 0.f);
        
        // Noise Force Module
        if (NoiseForce)
        {
            // ���� �ð����� Noise Force �� ������ �����ϰ� ����
            if (Module[0].NoiseForceTerm <= Particle.NoiseForceAccTime)
            {
                Particle.NoiseForceAccTime -= Module[0].NoiseForceTerm;
                
                float3 vRandom = GetRandom(NoiseTex, _ID.x, MAX_COUNT);
                float3 vNoiseForce = normalize(vRandom.xyz - 0.5f);

                Particle.NoiseForceDir = vNoiseForce;
            }
                        
            Particle.vForce += Particle.NoiseForceDir * Module[0].NoiseForceScale;
            Particle.NoiseForceAccTime += g_EngineDT;
        }
        
        // Particle �� �־��� ���� ���� ���ӵ� ���
        float3 vAccel = Particle.vForce / Particle.Mass;
        
        // ���ӵ��� ���� �ӵ��� ��ȭ
        Particle.vVelocity += vAccel * g_EngineDT;
                        
        // Velocity �� ���� �̵� ����        
        if (0 == Module[0].SpaceType)
        {
            // Space �� Local �̶��
            Particle.vLocalPos += Particle.vVelocity * g_EngineDT;
            Particle.vWorldPos = Particle.vLocalPos + ParticleObjectPos.xyz;
        }
        else
        {
            Particle.vLocalPos += Particle.vVelocity * g_EngineDT;
            Particle.vWorldPos += Particle.vVelocity * g_EngineDT;
        }
        
        // Scale ��⿡ ���� ���� ũ�� ���
        Particle.vWorldCurrentScale = Particle.vWorldInitScale;
        if (ScaleModule)
            Particle.vWorldCurrentScale = ((Module[0].EndScale - Module[0].StartScale) * Particle.NormalizedAge + Module[0].StartScale) * Particle.vWorldInitScale;
        
        
        if (DragModule)
        {
            if (Particle.NormalizedAge < Module[0].DestNormalizedAge)
            {
                // ����
                // (���� �ӷ� - ���� �ӷ�) / (���� NA - ���� NA)
                float Gradient = (Module[0].LimitSpeed - length(Particle.vVelocity)) / (Module[0].DestNormalizedAge - Particle.NormalizedAge);
                float NADT = g_EngineDT / Particle.Life;
            
                float NewSpeed = length(Particle.vVelocity) + (Gradient * NADT);
                Particle.vVelocity = normalize(Particle.vVelocity) * NewSpeed;
            }
            else
            {
                //if (Module[0].LimitSpeed < length(Particle.vVelocity))
                //{
                //    Particle.vVelocity = normalize(Particle.vVelocity) * Module[0].LimitSpeed;
                //}
            }
        }
        
        if (Render)
        {
            Particle.vColor.rgb = (Module[0].EndColor - Module[0].vSpawnColor.rgb) * Particle.NormalizedAge + Module[0].vSpawnColor.rgb;

            if (Module[0].FadeOut)
            {
                float fRatio = saturate(1.f - (Particle.NormalizedAge - Module[0].StartRatio) / (1.f - Module[0].StartRatio));
                Particle.vColor.a = fRatio;
            }
        }
        
        
        Particle.Age += g_EngineDT;
        Particle.NormalizedAge = Particle.Age / Particle.Life;
        if (Particle.Life <= Particle.Age)
        {
            Particle.Active = 0;
        }
    }
}
#endif