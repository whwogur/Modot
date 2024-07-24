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
    float offset = (g_EngineTime - floor(g_EngineTime)) / g_EngineTime;
    float CurrentTime = (g_EngineTime) * (offset);

    float3 WaveParams = float3(10.0, 0.8, 0.1);

    float ratio = 1.0;

    float2 WaveCentre = mul(g_vec4_0, matWV) / g_Resolution;

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
        texCoord += ((DiffTexCoord * DiffTime) / (CurrentTime * Dist * 40.0));
        Color = g_tex_0.Sample(g_sam_0, texCoord);

        // Blow out the color and reduce the effect over time
        Color += (Color * ScaleDiff) / (CurrentTime * Dist * 40.0);
    }

    return Color;
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
    float2 p = _in.vPosition.xy / g_Resolution;

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