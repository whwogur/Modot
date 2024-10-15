#ifndef _SKYBOX
#define _SKYBOX

#include "value.fx"
#define SKYBOX_SPHERE 0
#define SKYBOX_CUBE 1

struct VS_IN
{
    float3 vPos     : POSITION;
    float2 vUV      : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition    : SV_Position;
    float2 vUV          : TEXCOORD;
};

VS_OUT VS_SkyBox(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), matWVP);
    output.vUV = _in.vUV;
    
    
    
    return output;
}

float4 PS_SkyBox(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    if (g_int_0 == SKYBOX_SPHERE)
    {
        if (g_btex_0)
            vColor = g_tex_0.Sample(g_AniWrapSampler, _in.vUV);
    }
    else if (g_int_0 == SKYBOX_CUBE)
    {
        
    }
    
    
    return vColor;
}

#endif