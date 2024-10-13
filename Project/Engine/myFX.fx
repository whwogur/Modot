#ifndef _MYFX
#define _MYFX

#include "value.fx"

struct VS_IN
{
    float4 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vUV : TEXCOORD0;
    float2 texCoords1 : TEXCOORD1;
    float2 texCoords2 : TEXCOORD2;
    float2 texCoords3 : TEXCOORD3;
};

VS_OUT VS_Fire(VS_IN _in)
{
    VS_OUT output;
    
    output.vPosition = mul(_in.vPos, matWVP);

    // Store the texture coordinates for the pixel shader.
    output.vUV = _in.vUV;
    // Compute texture coordinates for first noise texture using the first scale and upward scrolling speed values.
    output.texCoords1 = (_in.vUV * g_vec4_1.x);
    output.texCoords1.y = output.texCoords1.y + (g_EngineTime * g_vec4_0.x);

    // Compute texture coordinates for second noise texture using the second scale and upward scrolling speed values.
    output.texCoords2 = (_in.vUV * g_vec4_1.y);
    output.texCoords2.y = output.texCoords2.y + (g_EngineTime * g_vec4_0.y);

    // Compute texture coordinates for third noise texture using the third scale and upward scrolling speed values.
    output.texCoords3 = (_in.vUV * g_vec4_1.z);
    output.texCoords3.y = output.texCoords3.y + (g_EngineTime * g_vec4_0.z);

    return output;
}

float4 PS_Fire(VS_OUT _in) : SV_TARGET
{
    float4 noise1;
    float4 noise2;
    float4 noise3;
    float4 finalNoise;
    float perturb;
    float2 noiseCoords;
    float4 fireColor;
    float4 alphaColor;

    // Sample the same noise texture using the three different texture coordinates to get three different noise scales.
    noise1 = g_tex_1.Sample(g_AniWrapSampler, _in.texCoords1);
    noise2 = g_tex_1.Sample(g_AniWrapSampler, _in.texCoords2);
    noise3 = g_tex_1.Sample(g_AniWrapSampler, _in.texCoords3);

    // Move the noise from the (0, 1) range to the (-1, +1) range.
    noise1 = (noise1 - 0.5f) * 2.0f;
    noise2 = (noise2 - 0.5f) * 2.0f;
    noise3 = (noise3 - 0.5f) * 2.0f;
    
    // Distort the three noise x and y coordinates by the three different distortion x and y values.
    noise1.xy = noise1.xy * g_vec2_0.xy;
    noise2.xy = noise2.xy * g_vec2_1.xy;
    noise3.xy = noise3.xy * g_vec2_2.xy;

    // Combine all three distorted noise results into a single noise result.
    finalNoise = noise1 + noise2 + noise3;

    // Perturb the _in texture Y coordinates by the distortion scale and bias values.  
    // The perturbation gets stronger as you move up the texture which creates the flame flickering at the top effect.
    perturb = ((1.0f - _in.vUV.y) * g_float_0) + g_float_1;

    // Now create the perturbed and distorted texture sampling coordinates that will be used to sample the fire color texture.
    noiseCoords.xy = (finalNoise.xy * perturb) + _in.vUV.xy;

    // Sample the color from the fire texture using the perturbed and distorted texture sampling coordinates.
    // Use the clamping sample state instead of the wrap sample state to prevent flames wrapping around.
    fireColor = g_tex_0.Sample(g_LinearClampSampler, noiseCoords.xy);

    // Sample the alpha value from the alpha texture using the perturbed and distorted texture sampling coordinates.
    // This will be used for transparency of the fire.
    // Use the clamping sample state instead of the wrap sample state to prevent flames wrapping around.
    alphaColor = g_tex_2.Sample(g_LinearClampSampler, noiseCoords.xy);

    // Set the alpha blending of the fire to the perturbed and distorted alpha texture value.
    fireColor.a = alphaColor.r;

    return fireColor;
}


#endif