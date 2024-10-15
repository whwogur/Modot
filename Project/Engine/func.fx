#ifndef _FUNC
#define _FUNC

#include "value.fx"
#include "struct.fx"

#define LIGHT_DIRECTIONAL   0  
#define LIGHT_POINT         1  
#define LIGHT_SPOT          2  

int IsBinding(Texture2D _tex)
{
    int width = 0, height = 0;
    _tex.GetDimensions(width, height);

    if (0 == width || height == 0)
        return false;

    return true;
}

void CalculateLight2D(int _LightIdx, float3 _WorldPos, inout tLight _Light)
{
    tLightInfo Info = g_Light2D[_LightIdx];

    // DirectionalLight �� ���
    if (LIGHT_DIRECTIONAL == Info.Type)
    {
        _Light.Color.rgb += Info.light.Color.rgb;
        _Light.Ambient.rgb += Info.light.Ambient.rgb;
    }

    // PointLight �� ���
    else if (LIGHT_POINT == Info.Type)
    {
        // �������� �ȼ������� �Ÿ�
        float fDist = distance(Info.WorldPos.xy, _WorldPos.xy);

        // �Ÿ����� ������ ġȯ�ؼ� �Ÿ��� ���� ���� ���Ⱑ �ڻ��� �׷��� ó�� ����������
        float fPow = saturate(cos(saturate(fDist / Info.Radius) * (PI / 2.f)));

        // �������κ��� ������ �Ÿ��� ���� ���� ����
        //float fPow2 = saturate(1.f - fDist / Info.Radius);

        // ���� ���� ��� = ���� �� * �Ÿ������� ����
        _Light.Color.rgb += Info.light.Color.rgb * fPow;
        _Light.Ambient.rgb += Info.light.Ambient.rgb;
    }

    // SpotLight �� ���
    else if (LIGHT_SPOT == Info.Type)
    {
        float2 LightToPixel = normalize(_WorldPos.xy - Info.WorldPos.xy);
        float Theta = dot(Info.WorldDir.xy, LightToPixel);
        float Degree = acos(Theta);
        
        if (Degree < radians(Info.Angle / 2.f))
        {
            float fPow = 1.f;
        
            float Dist = distance(Info.WorldPos.xy, _WorldPos.xy);
            if (Dist < Info.Radius)
            {
                float fTheta = (Dist / Info.Radius) * (PI / 2.f);
                fPow = saturate(cos(fTheta));
            }
            
            _Light.Color.rgb += Info.light.Color.rgb * fPow;
            _Light.Ambient.rgb += Info.light.Ambient.rgb;
        }
    }
}

float3 GetRandom(in Texture2D _NoiseTexture, uint _ID, uint _maxId)
{
    float2 vUV = (float2) 0.f;
    float fIDFactor = (float) _ID / (float) (_maxId - 1);
    
    vUV.x = fIDFactor * sin(g_EngineTime * 0.5f + fIDFactor * 10.0f);
    vUV.y = cos(fIDFactor * 20 * PI) * sin(g_EngineTime * 0.3f + fIDFactor * 5.0f) + g_EngineTime * 0.05f;
    
    // ���� �������� ������ ���ø�
    float3 vRandom = _NoiseTexture.SampleLevel(g_PointWrapSampler, vUV, fmod(g_EngineTime, 4)).xyz;

    return vRandom;
}

// �ݻ籤�� ���� ���� ��� �Լ�
void CalculateLightIntensity(float3 vLightDir, float3 _ViewNormal, float3 _ViewPos, out float LightPow, out float SpecularPow)
{
    LightPow = saturate(dot(-vLightDir, _ViewNormal));
    
    // �ݻ籤 ���
    float3 vReflect = vLightDir + 2 * dot(-vLightDir, _ViewNormal) * _ViewNormal;
    vReflect = normalize(vReflect);
    
    // �� ���� (ī�޶󿡼� ǥ������ ���ϴ� ����)
    float3 vEye = normalize(_ViewPos);
    
    // �ݻ籤�� ����
    SpecularPow = saturate(dot(vReflect, -vEye));
    SpecularPow = pow(SpecularPow, 15);
}

void CalculateAttenuation(float3 vLightViewPos, float3 _ViewPos, float LightRadius, out float Ratio, out float SpecRatio)
{
    float Distance = length(vLightViewPos - _ViewPos);
    float CamDistance = length(_ViewPos);
    
    // �Ÿ��� ���� ���� ���
    Ratio = saturate(cos((PI / 2.f) * saturate(Distance / LightRadius)));
    SpecRatio = saturate(cos((PI / 2.f) * saturate(CamDistance / LightRadius)));
}

float CalculateSpotAttenuation(float3 vLightDir, float3 spotDir, float coneOuter, float coneInner, float falloff)
{
    float spotCos = dot(-vLightDir, spotDir); // ����Ʈ����Ʈ�� ǥ�� ���� ���� ���� ����
    float spotFactor = saturate((spotCos - coneOuter) / (coneInner - coneOuter)); // ���� ���� ���
    return pow(spotFactor, falloff);
}

void CalculateLight3D(int _LightIdx, float3 _ViewNormal, float3 _ViewPos, inout tLight _Light)
{
    tLightInfo LightInfo = g_Light3D[_LightIdx];

    float LightPow = 0.f;
    float SpecularPow = 0.f;
    float Ratio = 1.f;
    float SpecRatio = 1.f;
    
    // Directional Light
    if (LIGHT_DIRECTIONAL == LightInfo.Type)
    {
        float3 vLightDir = normalize(mul(float4(LightInfo.WorldDir, 0.f), matView).xyz);
        CalculateLightIntensity(vLightDir, _ViewNormal, _ViewPos, LightPow, SpecularPow);
    }
    
    // Point Light
    else if (LIGHT_POINT == LightInfo.Type)
    {
        float3 vLightViewPos = mul(float4(LightInfo.WorldPos, 1.f), matView).xyz;
        float3 vLightDir = normalize(_ViewPos - vLightViewPos);
        
        CalculateLightIntensity(vLightDir, _ViewNormal, _ViewPos, LightPow, SpecularPow);
        CalculateAttenuation(vLightViewPos, _ViewPos, LightInfo.Radius, Ratio, SpecRatio);
    }
    
    // Spot Light
    else if (LIGHT_SPOT == LightInfo.Type)
    {
        float3 vLightViewPos = mul(float4(LightInfo.WorldPos, 1.f), matView).xyz;
        float3 vLightDir = normalize(_ViewPos - vLightViewPos);
        
        CalculateLightIntensity(vLightDir, _ViewNormal, _ViewPos, LightPow, SpecularPow);
        CalculateAttenuation(vLightViewPos, _ViewPos, LightInfo.Radius, Ratio, SpecRatio);

        // ����Ʈ����Ʈ ���� ���� ���
        float3 spotDir = normalize(mul(float4(LightInfo.WorldDir, 0.f), matView).xyz);
        float spotFactor = CalculateSpotAttenuation(vLightDir, spotDir, LightInfo.ConeOuter, LightInfo.ConeInner, LightInfo.Falloff);
        Ratio *= spotFactor; // ���� ���� ����
    }
    
    _Light.Color += LightInfo.light.Color * LightPow * Ratio;
    _Light.Ambient += LightInfo.light.Ambient * Ratio;
    _Light.SpecCoefficient += LightInfo.light.SpecCoefficient * SpecularPow * SpecRatio;
}
#endif