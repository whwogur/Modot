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
    float fIDFactor = (float) _ID / (float) (_maxId - 1);
    
    vUV.x = fIDFactor * sin(g_EngineTime * 0.5f + fIDFactor * 10.0f);
    vUV.y = cos(fIDFactor * 20 * PI) * sin(g_EngineTime * 0.3f + fIDFactor * 5.0f) + g_EngineTime * 0.05f;
    
    // 여러 레벨에서 노이즈 샘플링
    float3 vRandom = _NoiseTexture.SampleLevel(g_PointWrapSampler, vUV, fmod(g_EngineTime, 4)).xyz;

    return vRandom;
}

// 반사광과 빛의 강도 계산 함수
void CalculateLightIntensity(float3 vLightDir, float3 _ViewNormal, float3 _ViewPos, out float LightPow, out float SpecularPow)
{
    LightPow = saturate(dot(-vLightDir, _ViewNormal));
    
    // 반사광 계산
    float3 vReflect = vLightDir + 2 * dot(-vLightDir, _ViewNormal) * _ViewNormal;
    vReflect = normalize(vReflect);
    
    // 뷰 방향 (카메라에서 표면으로 향하는 방향)
    float3 vEye = normalize(_ViewPos);
    
    // 반사광의 세기
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
        // VS 에서 받은 노말값으로, 빛의 세기를 PS 에서 직접 구한다음 빛의 세기를 적용
        float3 vLightDir = normalize(mul(float4(LightInfo.WorldDir, 0.f), matView).xyz);
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
    }
    
    // Point Light
    else if (LightInfo.Type == LIGHT_POINT)
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
    else if (LightInfo.Type == LIGHT_SPOT)
    {
        // 표면 위치에서 광원의 위치를 뺀다. 광원에서 표면을 향하는 방향벡터를 구할 수 있다.
        float3 vLightViewPos = mul(float4(LightInfo.WorldPos, 1.f), matView).xyz;
        float3 vLightDir = normalize(_ViewPos - vLightViewPos);
        float3 vSpotDir = normalize(mul(float4(LightInfo.WorldDir, 0.f), matView).xyz);

        // 스포트라이트 각도에 따른 감쇠 계산
        float cosAngle = dot(-vLightDir, vSpotDir);
        float SpotEffect = saturate((cosAngle - cos(radians(LightInfo.ConeOuter))) /
                                (cos(radians(LightInfo.ConeInner)) - cos(radians(LightInfo.ConeOuter))));
    
        // 광원의 세기 계산
        LightPow = saturate(dot(-vLightDir, _ViewNormal)) * SpotEffect;
    
        // 반사광 계산
        float3 vReflect = vLightDir + 2 * dot(-vLightDir, _ViewNormal) * _ViewNormal;
        vReflect = normalize(vReflect);
        float3 vEye = normalize(_ViewPos);
    
        // 반사 방향과 시선 벡터를 내적해서 둘 사이의 벌어진 각도에 대한 cos 값을 반사광의 세기로 사용한다.
        SpecularPow = saturate(dot(vReflect, -vEye));
        SpecularPow = pow(SpecularPow, 15);
    
        // 거리에 따른 빛의 세기 감소량을 계산한다.
        float Distance = length(vLightViewPos - _ViewPos);
        Ratio = saturate(cos((PI / 2.f) * saturate(Distance / LightInfo.Radius)));
        SpecRatio = Ratio * SpotEffect;
    }

      
    // 표면이 받는 빛 = 광원의 빛 * 표면이 받는 빛의 세기 * 거리에 따른 빛의 감소량
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
    // 삼각형 표면 방향 벡터
    float3 Edge[2] = { (float3) 0.f, (float3) 0.f };
    Edge[0] = _Pos[1] - _Pos[0];
    Edge[1] = _Pos[2] - _Pos[0];
    
    // 삼각형에 수직방향인 법선(Normal) 벡터
    float3 Normal = normalize(cross(Edge[0], Edge[1]));
    
    // 삼각형 법선벡터와 Ray 의 Dir 을 내적
    // 광선에서 삼각형으로 향하는 수직벡터와, 광선의 방향벡터 사이의 cos 값
    float NdotD = -dot(Normal, _vDir);
        
    float3 vStoP0 = _vStart - _Pos[0];
    float VerticalDist = dot(Normal, vStoP0); // 광선을 지나는 한점에서 삼각형 평면으로의 수직 길이
            
    // 광선이 진행하는 방향으로, 삼각형을 포함하는 평면까지의 거리
    float RtoTriDist = VerticalDist / NdotD;
        
    // 광선이, 삼각형을 포함하는 평면을 지나는 교점
    float3 vCrossPoint = _vStart + RtoTriDist * _vDir;
        
    // 교점이 삼각형 내부인지 테스트
    float3 P0toCross = vCrossPoint - _Pos[0];
    
    float3 Full = cross(Edge[0], Edge[1]);
    float3 U = cross(Edge[0], P0toCross);
    float3 V = cross(Edge[1], P0toCross);
       
    // 직선과 삼각형 평면의 교점이 삼각형 1번과 2번 사이에 존재하는지 체크
    //      0
    //     /  \
    //    1 -- 2    
    if (dot(U, Full) < 0.f)
        return 0;
    
    // 교점이 삼각형 내부인지 체크
    if (length(Full) < length(U) + length(V))
        return 0;
        
    _CrossPos = vCrossPoint;
    _Dist = (uint) RtoTriDist;
    
    return 1;
}

matrix GetBoneMat(int _iBoneIdx, int _iRowIdx)
{
    return g_arrBoneMat[(g_iBoneCount * _iRowIdx) + _iBoneIdx];
}

void Skinning(inout float3 _vPos, inout float3 _vTangent, inout float3 _vBinormal, inout float3 _vNormal
    , inout float4 _vWeight, inout float4 _vIndices
    , int _iRowIdx)
{
    tSkinningInfo info = (tSkinningInfo) 0.f;
    if (_iRowIdx == -1)
        return;
    for (int i = 0; i < 4; ++i)
    {
        if (0.f == _vWeight[i])
            continue;
        matrix matBone = GetBoneMat((int) _vIndices[i], _iRowIdx);
        info.vPos += (mul(float4(_vPos, 1.f), matBone) * _vWeight[i]).xyz;
        info.vTangent += (mul(float4(_vTangent, 0.f), matBone) * _vWeight[i]).xyz;
        info.vBinormal += (mul(float4(_vBinormal, 0.f), matBone) * _vWeight[i]).xyz;
        info.vNormal += (mul(float4(_vNormal, 0.f), matBone) * _vWeight[i]).xyz;
    }
    _vPos = info.vPos;
    _vTangent = normalize(info.vTangent);
    _vBinormal = normalize(info.vBinormal);
    _vNormal = normalize(info.vNormal);
}

#endif