#ifndef _POSTPROCESS
#define POSTPROCESS

#include "value.fx"

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_Shockwave(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
  
    output.vPosition = mul(float4(_in.vPos, 1.f), matWVP);
    output.vUV = _in.vUV;
    
    return output;
}


float4 PS_Shockwave(VS_OUT _in) : SV_Target
{
    float adjustedTime = g_float_3 * 0.3;
    float offset = (adjustedTime - floor(adjustedTime)) / adjustedTime;
    float CurrentTime = (adjustedTime) * (offset);

    float3 WaveParams = float3(17, 1.7, 0.07); // TODO (하드코디드)

    float2 WaveCentre = mul(g_vec4_3, matWVP).xy / g_Resolution; // TODO (하드코디드)

    float2 texCoord = _in.vPosition.xy / g_Resolution;

    float Dist = distance(texCoord, WaveCentre);

    float4 Color = g_tex_0.Sample(g_sam_0, texCoord);

    // Only distort the pixels within the parameter distance from the centre
    if ((Dist <= (CurrentTime + WaveParams.z)) && (Dist >= (CurrentTime - WaveParams.z)))
    {
        // The pixel offset distance based on the input parameters
        float Diff = (Dist - CurrentTime);
        float ScaleDiff = (1.0 - pow(abs(Diff * WaveParams.x), WaveParams.y));
        float DiffTime = (Diff * ScaleDiff);

        // The direction of the distortion
        float2 DiffTexCoord = normalize(texCoord - WaveCentre);

        // Perform the distortion and reduce the effect over time
        texCoord += ((DiffTexCoord * DiffTime) / (CurrentTime * Dist * 10.0));
        Color = g_tex_0.Sample(g_sam_0, texCoord);

        // Blow out the color and reduce the effect over time
        Color += (Color * ScaleDiff) / (CurrentTime * Dist * 40.0);
    }

    return Color;
}


// 방사형 블러 샘플 개수. 많을수록 좋지만, 프레임 속도를 고려해야 함
static const float SAMPLES = 24.0;

// 2x1 해시 함수. 샘플을 불규칙하게 하는 데 사용.
float hash(float2 p)
{
    return frac(sin(dot(p, float2(41, 289))) * 45758.5453);
}

float3 lOff(float iTime)
{
    float2 u = sin(float2(1.57, 0) - iTime / 2.0);
    float2x2 a = float2x2(u.x, u.y, -u.y, u.x);
    
    float3 l = normalize(float3(1.5, 1.0, -0.5));
    l.xz = mul(a, l.xz);
    l.xy = mul(a, l.xy);
    
    return l;
}

float4 PS_Godray(VS_OUT _in) : SV_Target
{
    if (g_int_3)
    {
        float2 uv = _in.vPosition.xy / g_Resolution.xy;

        // 방사형 블러 인자들.
        //
        // 외곽으로 방사되는 동안 감소.
        float decay = g_float_0;
        // 샘플 밀도를, 이는 샘플의 확산 제어.
        float density = g_float_1;
        // 샘플 가중치. 외곽으로 방사되는 동안 감소
        float weight = g_float_2;
    
        float3 l = lOff(g_EngineTime);
    
        // 텍스처 좌표 오프셋 (uv - 0.5), 다시 가짜 빛의 움직임으로 오프셋 적용.
        // 이 값은 블러 방향(일종의 방향 벡터)을 설정하는 데 사용되며, 
        // 이후 스포트라이트 중심을 설정하는 데 사용됨
        //
        // 범위는 0을 중심으로 하여, 모든 방향으로 퍼질 수 있도록 함
        // 즉, 방사형
        float2 tuv = uv - 0.5 - l.xy * 0.45;
    
        // 위의 방향 벡터를 샘플 개수와 밀도 인자로 나눈다
        // 밀도 값은 블러가 퍼지는 정도를 제어한다. 밀도가 높을수록 블러 반경이 커진다
        float2 dTuv = tuv * density / SAMPLES;
    
        // 초기 텍스처 샘플의 일부를 가져옵니다. 값이 클수록 장면이 더 선명해짐
        float4 col = g_tex_0.Sample(g_sam_2, uv) * 0.25;
    
        // 샘플이 불연속적으로 축적될 때 밴딩을 제거하기 위해 지터링 
        // 특히 샘플 레이어가 적을 때 매우 중요
        uv += dTuv * (hash(uv.xy + frac(g_EngineTime)) * 2.0 - 1.0);
    
        // 방사형 블러 루프 텍스처 샘플을 가져오고, 
        // 방사형 방향 벡터(dTuv) 방향으로 조금 이동한 후 
        // 가중치가 약간 적은 샘플을 추가. 이 과정을 샘플 개수만큼 반복
        for (float i = 0.0; i < SAMPLES; i++)
        {
            uv -= dTuv;
            col += g_tex_0.Sample(g_sam_2, uv) * weight;
            weight *= decay;
        }
    
        // 방사형 블러의 초점에 중심을 둔 스포트라이트와 최종 색상을 곱한다 
        col *= (1.0 - dot(tuv, tuv) * 0.75);
    
        // 최종 색상을 부드럽게 조정하고, 약간의 감마 보정
        return sqrt(smoothstep(0.0, 1.0, col));
    }
    else
    {
        return g_tex_0.Sample(g_sam_0, _in.vPosition.xy / g_Resolution.xy);
    }
}

// ==========================
// Blur Shader
// Mesh     : RectMesh
// DSTYPE   : NO_TEST_NO_WRITE
// g_tex_0  : Target
// ===========================

static float GaussianFilter[5][5] =
{
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.0219f, 0.0983f, 0.1621f, 0.0983f, 0.0219f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
};

static float CrossFilter[13] = { 0.0561f, 0.1353f, 0.278f, 0.4868f, 0.7261f, 0.9231f, 1.f, 0.9231f, 0.7261f, 0.4868f, 0.278f, 0.1353f, 0.0561f };
static float Total = 6.2108f;

struct VS_OUT_BLUR
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    float InstID : FOG;
};

VS_OUT_BLUR VS_Blur(VS_IN _in, uint _InstID : SV_InstanceID)
{
    VS_OUT_BLUR output = (VS_OUT_BLUR) 0.f;
    
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV = _in.vUV;
    output.InstID = _InstID;
    
    return output;
}

float4 PS_Blur(VS_OUT_BLUR _in) : SV_Target
{
    float4 vColor = float4(0.f, 0.f, 0.f, 0.f);
        
    float2 vUVStep = 1.f / g_Resolution;
    vUVStep *= 3.4f;
    
    if (_in.InstID == 0)
    {
        for (int i = 0; i < 13; ++i)
        {
            float2 vUV = _in.vUV + float2(vUVStep.x * (-6 + i), 0.f);
            vColor += g_tex_0.Sample(g_sam_3, vUV) * CrossFilter[i];
        }
    }
    else if (_in.InstID == 1)
    {
        for (int j = 0; j < 13; ++j)
        {
            float2 vUV = _in.vUV + float2(0.f, vUVStep.y * (-6 + j));
            vColor += g_tex_0.Sample(g_sam_3, vUV) * CrossFilter[j];
        }
    }
    vColor /= Total;
    
    return vColor;
}


VS_OUT VS_EffectMerge(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_EffectMerge(VS_OUT _in) : SV_Target
{
    float4 vColor = float4(0.f, 0.f, 0.f, 0.f);
    
    float4 vEffect = g_tex_0.Sample(g_sam_0, _in.vUV);
    float4 vEffectBlur = g_tex_1.Sample(g_sam_0, _in.vUV);
    
    float4 vBloom = pow(abs(pow(vEffect, 2.2f)) + abs(pow(vEffectBlur, 2.2f)), 1.f / 2.2f);
    vBloom = saturate(vBloom);
    
    return vBloom;
}
#endif