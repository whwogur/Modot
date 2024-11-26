#ifndef _STD3D
#define _STD3D
#include "value.fx"
#include "func.fx"

struct VS_IN
{
    float3 vPos             : POSITION;
    float2 vUV              : TEXCOORD;
    
    float3 vTangent         : TANGENT;
    float3 vNormal          : NORMAL;
    float3 vBinormal        : BINORMAL;
    float4 vWeights         : BLENDWEIGHT;
    float4 vIndices         : BLENDINDICES;
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

VS_OUT VS_Std3D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    if (g_iAnim)
    {
        Skinning(_in.vPos, _in.vTangent, _in.vBinormal, _in.vNormal
              , _in.vWeights, _in.vIndices, 0);
    }
    
    output.vPosition = mul(float4(_in.vPos, 1.f), matWVP);
    output.vUV = _in.vUV;
    
    // 정점의 ViewSpace 좌표
    output.vViewPos = mul(float4(_in.vPos, 1.f), matWV).xyz;
    
    output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), matWV).xyz);
    output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), matWV).xyz);
    output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), matWV).xyz);
    
    return output;
}

float4 PS_Std3D(VS_OUT _in) : SV_Target
{
    float4 vOutColor = float4(0.7f, 0.7f, 0.7f, 1.f);
    float3 vViewNormal = _in.vViewNormal;
    
    if (g_btex_0)
        vOutColor = g_tex_0.Sample(g_AniWrapSampler, _in.vUV);
    
    if (g_btex_1)
    {
        vViewNormal = g_tex_1.Sample(g_AniWrapSampler, _in.vUV).xyz;
        vViewNormal = vViewNormal * 2.f - 1.f;
        
        // 적용시킬 곳의 표면 (T, B, N)
        float3x3 matRot =
        {
            _in.vViewTangent,
            _in.vViewBinormal,
            _in.vViewNormal    
        };
        
        vViewNormal = mul(vViewNormal, matRot);
    }
    
    tLight light = (tLight) 0.f;
    
    for (int i = 0; i < g_Light3DCount; ++i)
    {
        CalculateLight3D(i, vViewNormal, _in.vViewPos, light);
    }
    
    // 계산한 빛을 물체의 색상에 적용
    vOutColor.xyz = vOutColor.xyz * light.Color.rgb
                    + vOutColor.xyz * light.Ambient.rgb
                    + g_Light3D[0].light.Color.rgb * light.SpecCoefficient;
        
    return vOutColor;
}
#endif