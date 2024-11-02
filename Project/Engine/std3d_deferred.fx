#ifndef _STD3D_DEFERRED
#define _STD3D_DEFERRED

#include "value.fx"
#include "func.fx"

#define ALBEDO_TEXTURE g_tex_0
#define NORMAL_TEXTURE g_tex_1
#define REFLECTION_TEXTURE g_texcube_0
struct VS_IN
{
    float3 vPos         : POSITION;
    float2 vUV          : TEXCOORD;
    float3 vTangent     : TANGENT;
    float3 vNormal      : NORMAL;
    float3 vBinormal    : BINORMAL;
};

struct VS_OUT
{
    float4 vPosition        : SV_Position;
    float2 vUV              : TEXCOORD;
    float3 vViewPos         : POSITION;
    float3 vViewTangent     : TANGENT;
    float3 vViewNormal      : NORMAL;
    float3 vViewBinormal    : BINORMAL;
};


VS_OUT VS_Std3D_Deferred(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
        
    output.vPosition = mul(float4(_in.vPos, 1.f), matWVP);
    output.vUV = _in.vUV;
   
    // ������ ViewSpace ��ǥ
    output.vViewPos = mul(float4(_in.vPos, 1.f), matWV).xyz;
    
    // �� Space ���� ���������� ǥ���� ���������� ���
    output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), matWV).xyz);
    output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), matWV).xyz);
    output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), matWV).xyz);
    
    return output;
}



struct PS_OUT
{
    float4 vAlbedo      : SV_Target0;
    float4 vNormal      : SV_Target1;
    float4 vPosition    : SV_Target2;
    float4 vEmissive    : SV_Target3;
    float4 vData        : SV_Target4;
};


PS_OUT PS_Std3D_Deferred(VS_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    output.vAlbedo = float4(0.7f, 0.7f, 0.7f, 1.f);
    output.vNormal = float4(_in.vViewNormal, 1.f);
    output.vPosition = float4(_in.vViewPos, 1.f);
    output.vEmissive = float4(0.f, 0.f, 0.f, 0.f);
    output.vData = float4(0.f, 0.f, 0.f, 0.f);
    
    if (g_btex_0)
        output.vAlbedo = ALBEDO_TEXTURE.Sample(g_AniWrapSampler, _in.vUV);
    
    if (g_btex_1)
    {
        float3 vNormal = NORMAL_TEXTURE.Sample(g_AniWrapSampler, _in.vUV).xyz;
        vNormal = vNormal * 2.f - 1.f;
                
        float3x3 matRot =
        {
            _in.vViewTangent,
            _in.vViewBinormal,
            _in.vViewNormal    
        };
        
        output.vNormal = float4(mul(vNormal, matRot), 1.f);
    }
    
    if (g_btexcube_0)
    {
        float3 vViewReflect = reflect(normalize(_in.vViewPos).xyz, output.vNormal.xyz);
        float3 vWorldReflect = normalize(mul(float4(vViewReflect, 0.f), matViewInv).xyz);
        float3 vRelfectColor = REFLECTION_TEXTURE.Sample(g_AniWrapSampler, vWorldReflect);
        output.vAlbedo.rgb *= vRelfectColor.rgb;
    }
    
    return output;
}





#endif