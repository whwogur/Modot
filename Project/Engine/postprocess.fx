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

    float2 WaveCentre = mul(g_vec4_0, matWVP).xy / g_Resolution; // 화면상에서 벨의 위치를 찾아내서 보내주어야 할듯???

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

    float2 Point = _in.vPosition.xy / g_Resolution;
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
    // Calculate the distance from the center of the texture
    float2 center = float2(0.5, 0.5);
    float distance = length(_in.vUV.xy - center);

    // Create ripple effect using sine wave based on distance and time
    float rippleFrequency = 2.0; // Adjust for more or fewer ripples
    float rippleSpeed = 10.0; // Adjust for faster or slower ripples
    float ripple = sin(distance * rippleFrequency - g_EngineTime * rippleSpeed) * 0.5 + 0.5;

    // Calculate the color based on the ripple effect
    float intensity = saturate(1.0 - distance) * ripple;
    //float4 color = lerp(float4(0, 0, 0, 1), float4(1, 0, 0, 1), intensity);
    float4 color = lerp(float4(g_tex_0.Sample(g_sam_0, _in.vPosition.xy).xyz, 1.0), float4(1, 0, 0, 1), intensity);

    if (color.r < 0.02)
        discard;
    return color;
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