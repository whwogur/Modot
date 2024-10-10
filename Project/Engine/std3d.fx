#ifndef _STD3D
#define _STD3D
#include "value.fx"

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
    
    // ������ ViewSpace ��ǥ
    output.vViewPos = mul(float4(_in.vPos, 1.f), matWV).xyz;
    
    // �� Space ���� ���������� ǥ���� ��������
    output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), matWV).xyz);
    
    return output;
}

float4 PS_Std3D(VS_OUT _in) : SV_Target
{
    float4 vOutColor = float4(0.5f, 0.5f, 0.5f, 1.f);
    
    if (g_btex_0)
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    tLightInfo Light = g_Light3D[0]; // �ӽ�
        
    // VS ���� ���� �븻������, ���� ���⸦ PS ���� ���� ���Ѵ��� ���� ���⸦ ����
    float3 vLightDir = normalize(mul(float4(Light.WorldDir, 0.f), matView).xyz);
    float LightPow = saturate(dot(-vLightDir, _in.vViewNormal));
           
    // �ݻ籤 ���
    // vR = vL + 2 * dot(-vL, vN) * vN;
    float3 vReflect = vLightDir + 2 * dot(-vLightDir, _in.vViewNormal) * _in.vViewNormal;
    vReflect = normalize(vReflect);
    
        
    // ī�޶󿡼� ��ü�� ���ϴ� vEye �� ���Ѵ�. ī�޶�� ������ �ִ�.
    // �ȼ��� �佺���̽� ��ġ�� �� ī�޶󿡼� ��ü�� ���ϴ� Eye �����̴�.
    float3 vEye = normalize(_in.vViewPos);
    
    // �ݻ� ����� �ü� ���͸� �����ؼ� �� ������ ������ ������ ���� cos ���� �ݻ籤�� ����� ����Ѵ�.
    float SpecularPow = saturate(dot(vReflect, -vEye));
    SpecularPow = pow(SpecularPow, 20);
    
    vOutColor.xyz = vOutColor.xyz * LightPow * Light.light.Color.xyz
                        + vOutColor.xyz * Light.light.Ambient.xyz
                        + SpecularPow * Light.light.Color.xyz * Light.light.SpecCoefficient;

    return vOutColor;
}
#endif