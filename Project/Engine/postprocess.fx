#ifndef _POSTPROCESS
#define POSTPROCESS

#include "value.fx"

// ==========================
// GrayFilterShader
// Mesh     : RectMesh
// DSTYPE   : NO_TEST_NO_WRITE
// g_tex_0  : TargetCopyTex
// g_tex_1  : NoiseTexture 1
// g_tex_2  : NoiseTexture 2
// g_tex_3  : NoiseTexture 3
// ===========================
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

VS_OUT VS_GrayFilter(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    // Proj 행렬을 곱한 결과는 각 xyz 에 자신의 ViewZ 가 곱혀져있는 형태이다.
    // W 자리에 자신의 ViewZ 가 출력되기 때문에 이것으로 각 xyz 를 나누어야 실제 Proj 좌표가 나온다.
    // 따라서 Rasterizer State 에 투영행렬을 곱한 결과를 전달하면 각 xyz 를 w 로 나누어서 사용한다.    
    output.vPosition = float4(_in.vPos.xy * 2.f, 0.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}


float4 PS_GrayFilter(VS_OUT _in) : SV_Target
{
    // GrayFilter
    //float4 vColor = g_tex_0.Sample(g_sam_0, _in.vUV);    
    //float Average = (vColor.x + vColor.y + vColor.z) / 3.f;
    //vColor = float4(Average, Average, Average, 1.f); 
        
    // Cos Distortion
    /*float2 vUV = _in.vUV;    
    vUV.y += cos( (vUV.x + g_EngineTime * 0.1f) * PI * 12.f) * 0.01;    
    float4 vColor = g_tex_0.Sample(g_sam_0, vUV);*/
    
    // Noise
    float2 vUV = _in.vUV;
    vUV.x += g_EngineTime * 0.1;
    float4 vNoise = g_tex_3.Sample(g_sam_0, vUV);
    vNoise = (vNoise * 2.f - 1.f) * 0.01f;
    vUV = _in.vUV + vNoise.xy;
    float4 vColor = g_tex_0.Sample(g_sam_0, vUV);
    vColor.b *= 1.5f;
    
    return vColor;
}




// ==========================
// Distortion Shader
// Mesh     : RectMesh
// DSTYPE   : NO_TEST_NO_WRITE
// g_tex_0  : TargetCopyTex
// g_tex_1  : NoiseTexture 1
// g_tex_2  : NoiseTexture 2
// g_tex_3  : NoiseTexture 3
// ===========================

VS_OUT VS_Distortion(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), matWVP);
    output.vUV = _in.vUV;
    
    return output;
}


float4 PS_Distortion(VS_OUT _in) : SV_Target
{
    /*float2 vScreenUV = _in.vPosition.xy / g_Resolution;
        
    float4 vColor = g_tex_0.Sample(g_sam_0, vScreenUV);
    vColor.r *= 2.f;
    
    return vColor;*/
    
    float waveStrength = 0.2;
    float frequency = 10.0;
    float waveSpeed = 10.0;
    float4 HitColor = float4(1.0, 0.51, 0.45, 1.0);
    float sunlightStrength = 2.0;
    float centerLight = 2.0;
    float oblique = 0.25;

    float2 Point = _in.vPosition / g_Resolution;
    float2 uv = _in.vUV;
    float modifiedTime = g_EngineTime * waveSpeed;
    float aspectRatio = g_Resolution.x / g_Resolution.y;
    float2 distVec = uv - Point;
    distVec.x *= aspectRatio;
    float distance = length(distVec);

    float multiplier = (distance < 1.0) ? pow(distance - 1.0, 2.0) : 0.0;
    float addend = (sin(frequency * distance - modifiedTime) + centerLight) * waveStrength * multiplier;
    float2 newTexCoord = uv + addend * oblique;

    float4 colorToAdd = HitColor * sunlightStrength * addend;

    float4 fragColor = g_tex_0.Sample(g_sam_0, Point) + colorToAdd;

    return fragColor;
}


// ==========================
// Ripple Shader
// Mesh     : RectMesh
// DSTYPE   : NO_TEST_NO_WRITE
// g_tex_0  : TargetCopyTex
// g_tex_1  : NoiseTexture 1
// g_tex_2  : NoiseTexture 2
// g_tex_3  : NoiseTexture 3
// ===========================
VS_OUT VS_Ripple(VS_IN _in)
{
    VS_OUT output = (VS_OUT)0.f;

    output.vPosition = mul(float4(_in.vPos, 1.f), matWVP);
    output.vUV = _in.vUV;

    return output;
}


float4 PS_Ripple(VS_OUT _in) : SV_Target
{
    float intensity = 0.06;

    // 좌표 정규화
    float2 p = _in.vPosition.xy / g_Resolution.xy * 2.0 - 1.0;

    // 벡터 길이
    float cLength = length(p);

    // 물결효과
    float2 uv = (_in.vPosition.xy / g_Resolution.xy)
        + (p / cLength) * cos(cLength * 15.0 - g_EngineTime * 4.0) * intensity;

    // 텍스쳐 샘플링
    float3 col = smoothstep(0.1, 0.91, g_tex_0.Sample(g_sam_0, uv).xyz);

    return float4(col, 1.0);
}

// ==========================
// Small Ripple Shader
// Mesh     : RectMesh
// DSTYPE   : NO_TEST_NO_WRITE
// g_tex_0  : TargetCopyTex
// ===========================

VS_OUT VS_SmallRipple(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    output.vPosition = mul(float4(_in.vPos, 1.f), matWVP);
    output.vUV = _in.vUV;

    return output;
}

float4 PS_SmallRipple(VS_OUT _in) : SV_Target
{
    float shockwaveIntensity = 0.05;
    float shockwaveSpeed = 5.0;
    float shockwaveFrequency = 20.0;
    float shockwaveWidth = 0.1;

    // Normalized coordinates
    float2 p = _in.vPosition.xy / g_Resolution * 2.0 - 1.0;

    // Distance from the center
    float distanceFromCenter = length(p);

    // Shockwave effect
    float currentRadius = g_EngineTime * shockwaveSpeed;
    float wave = sin(distanceFromCenter * shockwaveFrequency - currentRadius) * shockwaveWidth;
    wave = wave * smoothstep(0.0, 1.0, 1.0 - distanceFromCenter / currentRadius);

    // Adjust UV coordinates with the wave effect
    float2 uv = (_in.vPosition.xy / g_Resolution) + (p / distanceFromCenter) * wave * shockwaveIntensity;

    // Sample the texture
    float3 col = g_tex_0.Sample(g_sam_0, uv).xyz;

    return float4(col, 1.0);
}
#endif