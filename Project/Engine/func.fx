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

void CalculateLight3D(int _LightIdx, float3 _ViewNormal, float3 _ViewPos, inout tLight _Light)
{
    tLightInfo LightInfo = g_Light3D[_LightIdx];
    
    float LightPow = 0.f;
    float SpecularPow = 0.f;
    float Ratio = 1.f;
    float SpecRatio = 1.f;
    
    // Directional Light
    if (LightInfo.Type == LIGHT_DIRECTIONAL)
    {
        // VS ���� ���� �븻������, ���� ���⸦ PS ���� ���� ���Ѵ��� ���� ���⸦ ����
        float3 vLightDir = normalize(mul(float4(LightInfo.WorldDir, 0.f), matView).xyz);
        LightPow = saturate(dot(-vLightDir, _ViewNormal));
            
        // �ݻ籤 ���
        // vR = vL + 2 * dot(-vL, vN) * vN;
        float3 vReflect = vLightDir + 2 * dot(-vLightDir, _ViewNormal) * _ViewNormal;
        vReflect = normalize(vReflect);
            
        // ī�޶󿡼� ��ü�� ���ϴ� vEye �� ���Ѵ�. ī�޶�� ������ �ִ�.
        // �ȼ��� �佺���̽� ��ġ�� �� ī�޶󿡼� ��ü�� ���ϴ� Eye �����̴�.
        float3 vEye = normalize(_ViewPos);
    
        // �ݻ� ����� �ü� ���͸� �����ؼ� �� ������ ������ ������ ���� cos ���� �ݻ籤�� ����� ����Ѵ�.
        SpecularPow = saturate(dot(vReflect, -vEye));
        SpecularPow = pow(SpecularPow, 15);
    }
    
    // Point Light
    else if (LightInfo.Type == LIGHT_POINT)
    {
        // ǥ�� ��ġ���� ������ ��ġ�� ����. �������� ǥ���� ���ϴ� ���⺤�͸� ���� �� �ִ�.
        float3 vLightViewPos = mul(float4(LightInfo.WorldPos, 1.f), matView).xyz;
        float3 vLightDir = normalize(_ViewPos - vLightViewPos);
        LightPow = saturate(dot(-vLightDir, _ViewNormal));
            
        // �ݻ籤 ���
        // vR = vL + 2 * dot(-vL, vN) * vN;
        float3 vReflect = vLightDir + 2 * dot(-vLightDir, _ViewNormal) * _ViewNormal;
        vReflect = normalize(vReflect);
            
        // ī�޶󿡼� ��ü�� ���ϴ� vEye �� ���Ѵ�. ī�޶�� ������ �ִ�.
        // �ȼ��� �佺���̽� ��ġ�� �� ī�޶󿡼� ��ü�� ���ϴ� Eye �����̴�.
        float3 vEye = normalize(_ViewPos);
    
        // �ݻ� ����� �ü� ���͸� �����ؼ� �� ������ ������ ������ ���� cos ���� �ݻ籤�� ����� ����Ѵ�.
        SpecularPow = saturate(dot(vReflect, -vEye));
        SpecularPow = pow(SpecularPow, 15);
             
        // �Ÿ��� ���� ���� ���� ���ҷ��� ����Ѵ�.
        float Distance = length(vLightViewPos - _ViewPos);
        float CamDistance = length(_ViewPos);
        //Ratio = saturate(1.f - (Distance / LightInfo.Radius));
        Ratio = saturate(cos((PI / 2.f) * saturate(Distance / LightInfo.Radius)));
        SpecRatio = saturate(cos((PI / 2.f) * saturate(CamDistance / LightInfo.Radius)));
    }
    else if (LightInfo.Type == LIGHT_SPOT)
    {
        // ǥ�� ��ġ���� ������ ��ġ�� ����. �������� ǥ���� ���ϴ� ���⺤�͸� ���� �� �ִ�.
        float3 vLightViewPos = mul(float4(LightInfo.WorldPos, 1.f), matView).xyz;
        float3 vLightDir = normalize(_ViewPos - vLightViewPos);
        float3 vSpotDir = normalize(mul(float4(LightInfo.WorldDir, 0.f), matView).xyz);

        // ����Ʈ����Ʈ ������ ���� ���� ���
        float cosAngle = dot(-vLightDir, vSpotDir);
        float SpotEffect = saturate((cosAngle - cos(radians(LightInfo.ConeOuter))) /
                                (cos(radians(LightInfo.ConeInner)) - cos(radians(LightInfo.ConeOuter))));
    
        // ������ ���� ���
        LightPow = saturate(dot(-vLightDir, _ViewNormal)) * SpotEffect;
    
        // �ݻ籤 ���
        float3 vReflect = vLightDir + 2 * dot(-vLightDir, _ViewNormal) * _ViewNormal;
        vReflect = normalize(vReflect);
        float3 vEye = normalize(_ViewPos);
    
        // �ݻ� ����� �ü� ���͸� �����ؼ� �� ������ ������ ������ ���� cos ���� �ݻ籤�� ����� ����Ѵ�.
        SpecularPow = saturate(dot(vReflect, -vEye));
        SpecularPow = pow(SpecularPow, 15);
    
        // �Ÿ��� ���� ���� ���� ���ҷ��� ����Ѵ�.
        float Distance = length(vLightViewPos - _ViewPos);
        Ratio = saturate(cos((PI / 2.f) * saturate(Distance / LightInfo.Radius)));
        SpecRatio = Ratio * SpotEffect;
    }

      
    // ǥ���� �޴� �� = ������ �� * ǥ���� �޴� ���� ���� * �Ÿ��� ���� ���� ���ҷ�
    _Light.Color += LightInfo.light.Color * LightPow * Ratio;
    _Light.Ambient += LightInfo.light.Ambient * Ratio;
    _Light.SpecCoefficient += LightInfo.light.SpecCoefficient * SpecularPow * SpecRatio;
}

float GetTessFactor(float _MinLevel, float _MaxLevel
                  , float _MinRange, float _MaxRange
                  , float3 _CamPos, float3 _Pos)
{
    float D = distance(_CamPos, _Pos);
    
    if (D < _MaxRange)
        return pow(2.f, _MaxLevel);
    else if (_MinRange < D)
        return pow(2.f, _MinLevel);
    else
    {
        float fRatio = 1.f - (D - _MaxRange) / abs(_MaxRange - _MinRange);
        
        float Level = 1.f + fRatio * (_MaxLevel - _MinLevel - 1.f);
        
        return pow(2.f, Level);
    }
}

int IntersectsRay(float3 _Pos[3], float3 _vStart, float3 _vDir
                  , out float3 _CrossPos, out uint _Dist)
{
    // �ﰢ�� ǥ�� ���� ����
    float3 Edge[2] = { (float3) 0.f, (float3) 0.f };
    Edge[0] = _Pos[1] - _Pos[0];
    Edge[1] = _Pos[2] - _Pos[0];
    
    // �ﰢ���� ���������� ����(Normal) ����
    float3 Normal = normalize(cross(Edge[0], Edge[1]));
    
    // �ﰢ�� �������Ϳ� Ray �� Dir �� ����
    // �������� �ﰢ������ ���ϴ� �������Ϳ�, ������ ���⺤�� ������ cos ��
    float NdotD = -dot(Normal, _vDir);
        
    float3 vStoP0 = _vStart - _Pos[0];
    float VerticalDist = dot(Normal, vStoP0); // ������ ������ �������� �ﰢ�� ��������� ���� ����
            
    // ������ �����ϴ� ��������, �ﰢ���� �����ϴ� �������� �Ÿ�
    float RtoTriDist = VerticalDist / NdotD;
        
    // ������, �ﰢ���� �����ϴ� ����� ������ ����
    float3 vCrossPoint = _vStart + RtoTriDist * _vDir;
        
    // ������ �ﰢ�� �������� �׽�Ʈ
    float3 P0toCross = vCrossPoint - _Pos[0];
    
    float3 Full = cross(Edge[0], Edge[1]);
    float3 U = cross(Edge[0], P0toCross);
    float3 V = cross(Edge[1], P0toCross);
       
    // ������ �ﰢ�� ����� ������ �ﰢ�� 1���� 2�� ���̿� �����ϴ��� üũ
    //      0
    //     /  \
    //    1 -- 2    
    if (dot(U, Full) < 0.f)
        return 0;
    
    // ������ �ﰢ�� �������� üũ
    if (length(Full) < length(U) + length(V))
        return 0;
        
    _CrossPos = vCrossPoint;
    _Dist = (uint) RtoTriDist;
    
    return 1;
}

#endif