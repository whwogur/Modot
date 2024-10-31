#ifndef _DECAL
#define _DECAL

#include "value.fx"


// ================================
// Decal Shader
// MRT  : Decal ( Albedo, Emissive )
// Mesh : CubeMesh
// Rasterizer   : CULL_FRONT
// DepthStencil : NO_TEST_NO_WRITE
// BlendState   : Albedo - AlphaBlend, Emissive - One_One
// Parameter
#define POS_TARGET          g_tex_0
#define DECAL_TEXTURE       g_tex_1
#define EMISSION_TEXTURE    g_tex_2
#define EMISSION_MULTIPLIER g_float_0
// �� �����
// ���� �����
// ================================

struct VS_IN
{
    float3 vPos : POSITION;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
};

VS_OUT VS_Decal(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
        
    output.vPosition = mul(float4(_in.vPos, 1.f), matWVP);
    
    return output;
}

struct PS_OUT
{
    float4 vAlbedo : SV_Target0;
    float4 vEmissive : SV_Target1;
};

PS_OUT PS_Decal(VS_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    // �ȼ����̴��� ������ �������� Position ���� Ȯ���Ѵ�.    
    float2 vScreenUV = _in.vPosition.xy / g_Resolution;
    float4 vViewPos = POS_TARGET.Sample(g_AniWrapSampler, vScreenUV);
    
    // ������ �ε��� ��ü�� �������� �ʴ´�.
    if (0.f == vViewPos.w)
    {
        discard;
    }
    
    // �ش� ��ü�� �����޽� �������� ����
    // ������ ��ü�� ViewPos �� WorldPos �� �����Ѵ�.
    float3 vWorldPos = mul(float4(vViewPos.xyz, 1.f), matViewInv).xyz;
    
    // World �� �ִ� ��ü�� ��ǥ��, Volume Mesh �� ���� ������� ���ؼ� Local �������� ��������.
    float3 vLocalPos = mul(float4(vWorldPos, 1.f), matWorldInv).xyz;
    
    // ��ü�� �����޽�(Cube) ���� ���̶�� Decal �ߴ�
    if (0.5f < abs(vLocalPos.x) || 0.5f < abs(vLocalPos.y) || 0.5f < abs(vLocalPos.z))
    {
        discard;
    }
        
    float4 vColor = float4(0.f, 1.f, 0.f, 1.f);
    float4 vEmissive = float4(0.f, 0.f, 0.f, 0.f);
    
    float2 vUV = (float2) 0.f;
    vUV.x = vLocalPos.x + 0.5f;
    vUV.y = 1.f - (vLocalPos.z + 0.5f);
    
    if (g_btex_1)
    {
        vColor = DECAL_TEXTURE.Sample(g_AniWrapSampler, vUV);
    }
    
    if (g_btex_2)
    {
        vEmissive = EMISSION_TEXTURE.Sample(g_AniWrapSampler, vUV);
        vEmissive *= EMISSION_MULTIPLIER;
    }
    
    vColor.rgb *= vColor.a;
    output.vAlbedo = vColor;
    output.vEmissive = vEmissive;
   
    return output;
}




#endif