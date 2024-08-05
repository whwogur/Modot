#ifndef _PARTICLE
#define _PARTICLE

#include "value.fx"
#include "struct.fx"

StructuredBuffer<tParticle> ParticleBuffer : register(t20);
#define PARTICLE_IDX    g_int_0



struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};


struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};


VS_OUT VS_Particle(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    //ParticleBuffer[PARTICLE_IDX];

    return output;
}

float4 PS_Particle(VS_OUT _in) : SV_Target
{

    return float4(1.f, 0.f, 0.f, 1.f);
}




#endif