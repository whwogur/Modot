#ifndef _SKYBOX
#define _SKYBOX

#include "value.fx"
#define SKYBOX_SPHERE 0
#define SKYBOX_CUBE 1

struct VS_IN
{
    float3 vPos         : POSITION;
    float2 vUV          : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition    : SV_Position;
    float2 vUV          : TEXCOORD;
    float3 vLocalDir    : TEXCOORD1;
};

VS_OUT VS_SkyBox(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
            
    float4 vPos = mul(mul(float4(_in.vPos * 2.f, 0.f), matView), matProj);
    vPos.z = vPos.w;
    
    output.vPosition = vPos;
    output.vUV = _in.vUV;
    output.vLocalDir = normalize(_in.vPos);
    
    return output;
}

float4 PS_SkyBox(VS_OUT _in) : SV_Target
{
    float4 vColor = float4(0.7f, 0.7f, 0.7f, 1.f);
        
    // Sphere
    if (g_int_0 == SKYBOX_SPHERE)
    {
        if (g_btex_0)
            vColor = g_tex_0.Sample(g_AniWrapSampler, _in.vUV);
    }
    
    // Cube
    if (g_int_0 == SKYBOX_CUBE)
    {
        if (g_btexcube_0)
            vColor = g_texcube_0.Sample(g_AniWrapSampler, _in.vLocalDir);
    }
    
    return vColor;
}

#endif