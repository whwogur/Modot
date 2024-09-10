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
    // float3 �� float4 �� ������ ���߾��ش�.
    // ������ǥ�� 1 �� ����, ������� 4�࿡ ����ִ� �̵��� ����ްڴٴ� ��
    output.vPosition = mul(float4(_in.vPos, 1.f), matWVP);
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;

    output.vWorldPos = mul(float4(_in.vPos, 1.f), matWorld).xyz;

    return output;
}

float4 PS_Std2D(VTX_OUT _in) : SV_Target
{
    float4 vColor = float4(0.f, 0.f, 0.f, 1.f);

    // Animator �� ����Ѵ�.
    if (UseAnimator2D)
    {
        // _in.vUV : �������̸� ������ ��ġ�� ������ ȯ���� ��                
        float2 BackGroundLeftTop = LeftTopUV - (BackGroundUV - SliceUV) / 2.f;
        float2 vSpriteUV = BackGroundLeftTop + (_in.vUV * BackGroundUV);
        vSpriteUV -= OffsetUV;

        if (LeftTopUV.x <= vSpriteUV.x && vSpriteUV.x <= LeftTopUV.x + SliceUV.x
            && LeftTopUV.y <= vSpriteUV.y && vSpriteUV.y <= LeftTopUV.y + SliceUV.y)
        {
            vColor = g_AtlasTex.Sample(g_sam_1, vSpriteUV);
        }
        else
        {
            discard;
        }
    }

    // Animation �� ������� �ʴ´�.
    else
    {
        if (g_btex_0)
        {
            vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
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

    // ���� ����      
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
        vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
    }
    else
    {
        vColor = float4(1.f, 0.f, 1.f, 1.f);
    }

    return vColor;
}

float4 PS_Std2DSprite(VTX_OUT _in) : SV_Target
{
    float4 vColor = float4(0.f, 0.f, 0.f, 1.f);
            
    float2 BackGroundLeftTop = g_vec2_0 - (g_vec2_2 - g_vec2_1) / 2.f;
    float2 vSpriteUV = BackGroundLeftTop + (_in.vUV * g_vec2_2);
    vSpriteUV -= g_vec2_3;

    if (g_vec2_0.x <= vSpriteUV.x && vSpriteUV.x <= g_vec2_0.x + g_vec2_1.x
        && g_vec2_0.y <= vSpriteUV.y && vSpriteUV.y <= g_vec2_0.y + g_vec2_1.y)
    {
        vColor = g_tex_0.Sample(g_sam_1, vSpriteUV);
    }
    else
    {
        discard;
    }

    if (vColor.a == 0.f)
    {
        discard;
    }
    
    // ���� ����      
    tLight Light = (tLight) 0.f;

    for (int i = 0; i < g_Light2DCount; ++i)
    {
        CalculateLight2D(i, _in.vWorldPos, Light);
    }

    vColor.rgb = vColor.rgb * Light.Color.rgb
           + vColor.rgb * Light.Ambient.rgb;

    return vColor;
}

float4 PS_Std2DTint(VTX_OUT _in) : SV_Target
{
    float4 vColor = float4(0.f, 0.f, 0.f, 1.f);
    
    if (g_btex_0)
    {
        vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
    }
    else
    {
        vColor = float4(0.f, 0.f, 1.f, 1.f);
    }
    
    vColor *= g_vec4_0;

    if (vColor.a == 0.f)
    {
        discard;
    }

    // ���� ����      
    tLight Light = (tLight) 0.f;

    for (int i = 0; i < g_Light2DCount; ++i)
    {
        CalculateLight2D(i, _in.vWorldPos, Light);
    }

    vColor.rgb = vColor.rgb * Light.Color.rgb
               + vColor.rgb * Light.Ambient.rgb;

    return vColor;
}

VTX_OUT VS_UI(VTX_IN _in)
{
    VTX_OUT output;

    // ȭ�� �������� ��ȯ (z�� w �� �����ϰ� ����)
    output.vPosition = mul(float4(_in.vPos.xy, 0.f, 1.0f), matProj);
    output.vPosition.z = 0.5f;
    output.vPosition.w = 1.0f;

    output.vColor = _in.vColor;
    output.vUV = _in.vUV;

    return output;
}

float4 PS_StdUI(VTX_OUT _in) : SV_Target
{
    float4 vColor;
    
    if (g_btex_0)
    {
        vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
    }
    else
    {
        vColor = float4(1.f, 1.f, 0.f, 1.f);
    }

    if (vColor.a == 0.f)
    {
        discard;
    }

    return vColor;
}
#endif