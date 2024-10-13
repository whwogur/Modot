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

    // DirectionalLight 인 경우
    if (LIGHT_DIRECTIONAL == Info.Type)
    {
        _Light.Color.rgb += Info.light.Color.rgb;
        _Light.Ambient.rgb += Info.light.Ambient.rgb;
    }

    // PointLight 인 경우
    else if (LIGHT_POINT == Info.Type)
    {
        // 점광원과 픽셀까지의 거리
        float fDist = distance(Info.WorldPos.xy, _WorldPos.xy);

        // 거리값을 각도로 치환해서 거리에 따른 빛의 세기가 코사인 그래프 처럼 떨어지도록
        float fPow = saturate(cos(saturate(fDist / Info.Radius) * (PI / 2.f)));

        // 광원으로부터 떨어진 거리에 따른 빛의 세기
        //float fPow2 = saturate(1.f - fDist / Info.Radius);

        // 최종 색상 계산 = 빛의 색 * 거리에따른 세기
        _Light.Color.rgb += Info.light.Color.rgb * fPow;
        _Light.Ambient.rgb += Info.light.Ambient.rgb;
    }

    // SpotLight 인 경우
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
    vUV.x = ((float) _ID / (float) (_maxId - 1)) + g_EngineTime * 0.5f;
    vUV.y = sin(vUV.x * 20 * PI) * 0.5f + g_EngineTime * 0.1f;
    float3 vRandom = _NoiseTexture.SampleLevel(g_PointWrapSampler, vUV, 0).xyz;

    return vRandom;
}

void CalculateLight3D(int _LightIdx, float3 _ViewNormal, float3 _ViewPos, inout tLight _Light)
{
    tLightInfo LightInfo = g_Light3D[_LightIdx];
    
    float LightPow = 0.f;
    float SpecularPow = 0.f;
    float Ratio = 1.f;
    float SpecRatio = 1.f;
    
    if (LIGHT_DIRECTIONAL == LightInfo.Type)
    {
        float3 vLightDir = normalize(mul(float4(LightInfo.WorldDir, 0.f), matView).xyz);
        LightPow = saturate(dot(-vLightDir, _ViewNormal));
            
        float3 vReflect = vLightDir + 2 * dot(-vLightDir, _ViewNormal) * _ViewNormal;
        vReflect = normalize(vReflect);
            
        float3 vEye = normalize(_ViewPos);
        
        SpecularPow = saturate(dot(vReflect, -vEye));
        SpecularPow = pow(SpecularPow, 15);
    }
    
    // Point Light
    else if (LIGHT_POINT == LightInfo.Type)
    {
        // 표면 위치에서 광원의 위치를 뺀다. 광원에서 표면을 향하는 방향벡터를 구할 수 있다.
        float3 vLightViewPos = mul(float4(LightInfo.WorldPos, 1.f), matView).xyz;
        float3 vLightDir = normalize(_ViewPos - vLightViewPos);
        LightPow = saturate(dot(-vLightDir, _ViewNormal));
            
        // 반사광 계산
        // vR = vL + 2 * dot(-vL, vN) * vN;
        float3 vReflect = vLightDir + 2 * dot(-vLightDir, _ViewNormal) * _ViewNormal;
        vReflect = normalize(vReflect);
            
        // 카메라에서 물체를 향하는 vEye 를 구한다. 카메라는 원점에 있다.
        // 픽셀의 뷰스페이스 위치가 곧 카메라에서 물체를 향하는 Eye 방향이다.
        float3 vEye = normalize(_ViewPos);
    
        // 반사 방향과 시선 벡터를 내적해서 둘 사이의 벌어진 각도에 대한 cos 값을 반사광의 세기로 사용한다.
        SpecularPow = saturate(dot(vReflect, -vEye));
        SpecularPow = pow(SpecularPow, 15);
             
        // 거리에 따른 빛의 세기 감소량을 계산한다.
        float Distance = length(vLightViewPos - _ViewPos);
        float CamDistance = length(_ViewPos);
        //Ratio = saturate(1.f - (Distance / LightInfo.Radius));
        Ratio = saturate(cos((PI / 2.f) * saturate(Distance / LightInfo.Radius)));
        SpecRatio = saturate(cos((PI / 2.f) * saturate(CamDistance / LightInfo.Radius)));
    }
    
    else if (LIGHT_SPOT == LightInfo.Type)
    {
        // 스포트라이트의 뷰스페이스 좌표 계산
        float3 vLightViewPos = mul(float4(LightInfo.WorldPos, 1.f), matView).xyz;
        float3 vLightDir = normalize(_ViewPos - vLightViewPos); // 광원에서 표면을 향하는 방향
    
        // 빛의 강도 계산
        LightPow = saturate(dot(-vLightDir, _ViewNormal));

        // 반사광 계산
        float3 vReflect = vLightDir + 2 * dot(-vLightDir, _ViewNormal) * _ViewNormal;
        vReflect = normalize(vReflect);

        float3 vEye = normalize(_ViewPos);
    
        // 반사광의 세기
        SpecularPow = saturate(dot(vReflect, -vEye));
        SpecularPow = pow(SpecularPow, 15);
    
        // 거리에 따른 빛의 감소량 계산
        float Distance = length(vLightViewPos - _ViewPos);
        float CamDistance = length(_ViewPos);
        Ratio = saturate(1.f - (Distance / LightInfo.Radius)); // 빛이 거리에 따라 자연스럽게 감소하도록 수정
        SpecRatio = saturate(1.f - (CamDistance / LightInfo.Radius)); // 반사광의 거리 감쇠

        // 스포트라이트의 각도에 따른 감쇠 계산
        float3 spotDir = normalize(mul(float4(LightInfo.WorldDir, 0.f), matView).xyz);
        float spotCos = dot(-vLightDir, spotDir); // 스포트라이트와 표면 간의 방향 벡터 내적
        float spotFactor = saturate((spotCos - LightInfo.ConeOuter) / (LightInfo.ConeInner - LightInfo.ConeOuter));
        spotFactor = pow(spotFactor, LightInfo.Falloff); // 스포트라이트의 각도에 따른 감쇠 비율 계산
        Ratio *= spotFactor; // 각도 감쇠 적용
    }
      
    // 표면이 받는 빛 = 광원의 빛 * 표면이 받는 빛의 세기 * 거리에 따른 빛의 감소량
    _Light.Color += LightInfo.light.Color * LightPow * Ratio;
    _Light.Ambient += LightInfo.light.Ambient * Ratio;
    _Light.SpecCoefficient += LightInfo.light.SpecCoefficient * SpecularPow * SpecRatio;
}

#endif