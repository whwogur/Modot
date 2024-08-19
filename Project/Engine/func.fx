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

    // DirectionalLight �� ���
    if (0 == Info.Type)
    {
        _Light.Color.rgb += Info.light.Color.rgb;
        _Light.Ambient.rgb += Info.light.Ambient.rgb;
    }

    // PointLight �� ���
    else if (1 == Info.Type)
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
    else
    {
        // SpotLight�� �ȼ� ���� ����
        float3 toPixel = _WorldPos - Info.WorldPos;
        float distanceToPixel = length(toPixel);

        // SpotLight ����� �ȼ� ������ ����
        float3 lightDir = normalize(Info.WorldDir);
        float3 pixelDir = normalize(toPixel);

        float spotEffect = dot(lightDir, pixelDir);

        // InnerCone�� OuterCone ���̿����� ���� ���� ���
        float innerCone = cos(Info.Angle); // Inner cone angle
        float outerCone = 0.5f * innerCone; // Outer cone angle as half of inner cone

        float spotIntensity = saturate((spotEffect - outerCone) / (innerCone - outerCone));

        // �Ÿ� ���� ��� (PointLight�� ����)
        float distanceAttenuation = saturate(1.0f - (distanceToPixel / Info.Radius));

        // ���� ���� ���
        float intensity = spotIntensity * distanceAttenuation;
        _Light.Color.rgb += Info.light.Color.rgb * intensity;
        _Light.Ambient.rgb += Info.light.Ambient.rgb * intensity;
    }
}

float3 GetRandom(in Texture2D _NoiseTexture, uint _ID, uint _maxId)
{
    float2 vUV = (float2) 0.f;
    vUV.x = ((float) _ID / (float) (_maxId - 1)) + g_EngineTime * 0.5f;
    vUV.y = sin(vUV.x * 20 * PI) * 0.5f + g_EngineTime * 0.1f;
    float3 vRandom = _NoiseTexture.SampleLevel(g_sam_1, vUV, 0).xyz;

    return vRandom;
}
#endif