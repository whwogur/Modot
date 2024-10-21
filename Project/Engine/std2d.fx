#ifndef _STD2D
#define _STD2D

#include "value.fx"
#include "func.fx"

// Vertex Shader
struct VTX_IN
{
    float3 vPos : POSITION;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

struct VTX_OUT
{
    float4 vPosition : SV_Position;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;

    float3 vWorldPos : POSITION;
};

VTX_OUT VS_Std2D(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT)0.f;

    // LocalSpace -> WorldSpace
    // float3 x float4x4(matrix)
    // float3 를 float4 로 차수를 맞추어준다.
    // 동차좌표를 1 로 설정, 상태행렬 4행에 들어있는 이동을 적용받겠다는 뜻
    output.vPosition = mul(float4(_in.vPos, 1.f), matWVP);
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;

    output.vWorldPos = mul(float4(_in.vPos, 1.f), matWorld).xyz;

    return output;
}

float4 PS_Std2D(VTX_OUT _in) : SV_Target
{
    float4 vColor = float4(0.f, 0.f, 0.f, 1.f);

    // Animator 을 사용한다.
    if (UseAnimator2D)
    {
        // _in.vUV : 스프라이를 참조할 위치를 비율로 환산한 값                
        float2 BackGroundLeftTop = LeftTopUV - (BackGroundUV - SliceUV) / 2.f;
        float2 vSpriteUV = BackGroundLeftTop + (_in.vUV * BackGroundUV);
        vSpriteUV -= OffsetUV;

        if (LeftTopUV.x <= vSpriteUV.x && vSpriteUV.x <= LeftTopUV.x + SliceUV.x
            && LeftTopUV.y <= vSpriteUV.y && vSpriteUV.y <= LeftTopUV.y + SliceUV.y)
        {
            vColor = g_AtlasTex.Sample(g_PointWrapSampler, vSpriteUV);
        }
        else
        {
            discard;
        }
    }

    // Animation 을 사용하지 않는다.
    else
    {
        if (g_btex_0)
        {
            vColor = g_tex_0.Sample(g_PointWrapSampler, _in.vUV);
        }
        else
        {
            vColor = float4(1.f, 0.f, 1.f, 1.f);
        }
    }

    if (vColor.a == 0.f)
    {
        discard;
    }

    if (g_vec4_3.g)
        vColor += g_vec4_3;
    
    // 광원 적용      
    tLight Light = (tLight)0.f;

    for (int i = 0; i < g_Light2DCount; ++i)
    {
        CalculateLight2D(i, _in.vWorldPos, Light);
    }

    vColor.rgb = vColor.rgb * Light.Color.rgb
               + vColor.rgb * Light.Ambient.rgb;

    return vColor;
}



float4 PS_Std2D_Alphablend(VTX_OUT _in) : SV_Target
{
    float4 vColor = float4(0.f, 0.f, 0.f, 1.f);

    if (g_btex_0)
    {
        vColor = g_tex_0.Sample(g_PointWrapSampler, _in.vUV);
    }
    else
    {
        vColor = float4(1.f, 0.f, 1.f, 1.f);
    }

    return vColor;
}

VTX_OUT VS_UI(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT)0.f;

    // 화면 공간으로 변환 (z와 w 값 적절하게 설정)
    output.vPosition = mul(float4(_in.vPos.xy, 0.f, 1.0f), matWorld);
    output.vPosition = mul(output.vPosition, matProj);
    
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;

    return output;
}

float4 PS_StdUI(VTX_OUT _in) : SV_Target
{
    float4 vColor;
    
    if (g_btex_0)
    {
        vColor = g_tex_0.Sample(g_PointWrapSampler, _in.vUV);
    }
    else
    {
        vColor = float4(1.f, 1.f, 0.f, 1.f);
    }

    vColor *= g_vec4_0;
    
    if (vColor.a == 0.f)
    {
        discard;
    }

    return vColor;
}

// ==============
// Effect Shader
// ==============
VTX_OUT VS_EffectUI(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos.xy, 0.f, 1.0f), matWorld);
    output.vPosition = mul(output.vPosition, matProj);
    output.vUV = _in.vUV;
    
    return output;
}

VTX_OUT VS_Effect(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;

    output.vPosition = mul(float4(_in.vPos, 1.f), matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Effect(VTX_OUT _in) : SV_Target
{
    float4 vColor;
    if (!g_btex_0)
    {
        vColor = float4(0.f, 1.f, 0.f, 1.f);
    }
    
    vColor = g_tex_0.Sample(g_AniWrapSampler, _in.vUV);
    if (0.f == vColor.a)
        discard;
    
    vColor.rgb = g_vec4_1.xyz;
    
    return vColor;
}

#endif