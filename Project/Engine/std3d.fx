#ifndef _STD3D
#define _STD3D
#include "value.fx"

static float3 g_LightDir = float3(1.f, -1.f, 1.f);
static float3 g_LightColor = float3(1.f, 1.f, 1.f);
static float3 g_Ambient = float3(0.15f, 0.15f, 0.15f);
static float g_SpecCoef = 0.3f; // 반사 계수

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    
    float3 vTangent : TANGENT;
    float3 vNormal : NORMAL;
    float3 vBinormal : BINORMAL;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    
    float3 vViewPos : POSITION;
    float3 vViewNormal : NORMAL;
};

VS_OUT VS_Std3D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
        
    output.vPosition = mul(float4(_in.vPos, 1.f), matWVP);
    output.vUV = _in.vUV;
    
    // 월드스페이스에서의 광원의 진행 방향
    float3 vLightDir = -normalize(g_LightDir);
    // 정점의 ViewSpace 좌표
    output.vViewPos = mul(float4(_in.vPos, 1.f), matWV).xyz;
    
    // 뷰 Space 에서 정점에서의 표면의 수직방향
    output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), matWV).xyz);
    
    return output;
}

float4 PS_Std3D(VS_OUT _in) : SV_Target
{
    float4 vOutColor = float4(0.5f, 0.5f, 0.5f, 1.f);
    
    if (g_btex_0)
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        
    // VS 에서 받은 노말값으로, 빛의 세기를 PS 에서 직접 구한다음 빛의 세기를 적용
    float3 vLightDir = normalize(mul(float4(g_LightDir, 0.f), matView).xyz);
    float LightPow = saturate(dot(-vLightDir, _in.vViewNormal));
           
    // 반사광 계산
    // vR = vL + 2 * dot(-vL, vN) * vN;
    float3 vReflect = vLightDir + 2 * dot(-vLightDir, _in.vViewNormal) * _in.vViewNormal;
    vReflect = normalize(vReflect);
    
        
    // 카메라에서 물체를 향하는 vEye 를 구한다. 카메라는 원점에 있다.
    // 픽셀의 뷰스페이스 위치가 곧 카메라에서 물체를 향하는 Eye 방향이다.
    float3 vEye = normalize(_in.vViewPos);
    
    // 반사 방향과 시선 벡터를 내적해서 둘 사이의 벌어진 각도에 대한 cos 값을 반사광의 세기로 사용한다.
    float SpecularPow = saturate(dot(vReflect, -vEye));
    SpecularPow = pow(SpecularPow, 20);
    
    vOutColor.xyz = vOutColor.xyz * LightPow * g_LightColor
                        + vOutColor.xyz * g_Ambient
                        + SpecularPow * g_LightColor * g_SpecCoef;

    return vOutColor;
}
#endif