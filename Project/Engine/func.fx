#ifndef _FUNC
#define _FUNC

#include "value.fx"
#include "struct.fx"

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
    if (0 == Info.Type)
    {
        _Light.Color.rgb += Info.light.Color.rgb;
        _Light.Ambient.rgb += Info.light.Ambient.rgb;
    }

    // PointLight 인 경우
    else if (1 == Info.Type)
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
    else
    {
        // SpotLight와 픽셀 간의 벡터
        float3 toPixel = _WorldPos - Info.WorldPos;
        float distanceToPixel = length(toPixel);

        // SpotLight 방향과 픽셀 방향의 내적
        float3 lightDir = normalize(Info.WorldDir);
        float3 pixelDir = normalize(toPixel);

        float spotEffect = dot(lightDir, pixelDir);

        // InnerCone과 OuterCone 사이에서의 조명 강도 계산
        float innerCone = cos(Info.Angle); // Inner cone angle
        float outerCone = 0.5f * innerCone; // Outer cone angle as half of inner cone

        float spotIntensity = saturate((spotEffect - outerCone) / (innerCone - outerCone));

        // 거리 감쇠 계산 (PointLight와 동일)
        float distanceAttenuation = saturate(1.0f - (distanceToPixel / Info.Radius));

        // 최종 색상 계산
        float intensity = spotIntensity * distanceAttenuation;
        _Light.Color.rgb += Info.light.Color.rgb * intensity;
        _Light.Ambient.rgb += Info.light.Ambient.rgb * intensity;
    }
}

#endif