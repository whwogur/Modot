#ifndef _STD2D
#define _STD2D

#include "value.fx"
#include "func.fx"

// Vertex Shader
struct VTX_IN
{
    float3 vPos : POSITION;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

struct VTX_OUT
{
    float4 vPosition : SV_Position;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

VTX_OUT VS_Std2D(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
 
    float3 vWorldPos = mul(float4(_in.vPos, 1.f), matWorld).xyz;
    float4 vViewPos = mul(float4(vWorldPos, 1.f), matView);
    float4 vProjPos = mul(vViewPos, matProj);
    
    output.vPosition = vProjPos;
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Std2D(VTX_OUT _in) : SV_Target
{
    float4 vColor = float4(0.f, 0.f, 0.f, 1.f);
     
    if (g_btex_0)
    {
        vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
        
        if (g_int_0)
        {
            vColor.r *= 2.f;
        }
    }
    else
    {
        vColor = float4(1.f, 0.f, 1.f, 1.f);
    }
  
    
    return vColor;
}

#endif