#ifndef _PARTICLE
#define _PARTICLE

#include "value.fx"
#include "struct.fx"

StructuredBuffer<tParticle> ParticleBuffer : register(t20);


struct VS_IN
{
    float3 vPos : POSITION;
    uint InstID : SV_InstanceID;
};


struct VS_OUT
{
    float3 vLocalPos : POSITION;
    uint InstID : FOG;
};


VS_OUT VS_Particle(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vLocalPos = _in.vPos;
    output.InstID = _in.InstID;

    return output;
}

struct GS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    uint InstID : FOG;
};

[maxvertexcount(6)]
void GS_Particle(point VS_OUT _in[1], inout TriangleStream<GS_OUT> _OutStream)
{
    GS_OUT output[4] = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f };

    //  0 -- 1
    //  | \  |
    //  3 -- 2
    float3 scale = ParticleBuffer[_in[0].InstID].vWorldScale;
    float3 vWorldPos = ParticleBuffer[_in[0].InstID].vWorldPos;

    output[0].vPosition.xyz = _in[0].vLocalPos + float3(-scale.x / 2.f, scale.y / 2.f, 0.f) + vWorldPos;
    output[1].vPosition.xyz = _in[0].vLocalPos + float3(scale.x / 2.f, scale.y / 2.f, 0.f) + vWorldPos;
    output[2].vPosition.xyz = _in[0].vLocalPos + float3(scale.x / 2.f, -scale.y / 2.f, 0.f) + vWorldPos;
    output[3].vPosition.xyz = _in[0].vLocalPos + float3(-scale.x / 2.f, -scale.y / 2.f, 0.f) + vWorldPos;


    for (int i = 0; i < 4; ++i)
    {
        output[i].vPosition.w = 1.f;
        output[i].vPosition = mul(mul(output[i].vPosition, matView), matProj);
        output[i].InstID = _in[0].InstID;
    }

    output[0].vUV = float2(0.f, 0.f);
    output[1].vUV = float2(1.f, 0.f);
    output[2].vUV = float2(1.f, 1.f);
    output[3].vUV = float2(0.f, 1.f);


    // Stream Ãâ·Â
    _OutStream.Append(output[0]);
    _OutStream.Append(output[2]);
    _OutStream.Append(output[3]);
    _OutStream.RestartStrip();

    _OutStream.Append(output[0]);
    _OutStream.Append(output[1]);
    _OutStream.Append(output[2]);
    _OutStream.RestartStrip();
}

float4 PS_Particle(GS_OUT _in) : SV_Target
{

    float4 vParticleColor = float4(1.f, 0.f, 0.f, 1.f);

    if (g_btex_0)
    {
        vParticleColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        vParticleColor *= ParticleBuffer[_in.InstID].vColor;
    }

    return vParticleColor;
}




#endif