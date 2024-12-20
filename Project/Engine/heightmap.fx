#ifndef _HEIGHTMAP
#define _HEIGHTMAP


#include "value.fx"
#include "struct.fx"

RWTexture2D<float> HeightMapTex : register(u0);
StructuredBuffer<tRaycastOut> BrushPos : register(t20);

#define WIDTH           g_int_0
#define HEIGHT          g_int_1

#define vBrushScale     g_vec2_0

#define HasBrushTex     g_btex_0
#define BRUSH_TEX       g_tex_0


[numthreads(32, 32, 1)]
void CS_HeightMap(int3 _ID : SV_DispatchThreadID)
{
    if (WIDTH <= _ID.x || HEIGHT <= _ID.y)
        return;
    
    float2 vBrushPos = BrushPos[0].LocationUV;
    float2 vScale = vBrushScale * float2(WIDTH, HEIGHT);
    
    float2 vCenter = float2(WIDTH, HEIGHT) * vBrushPos;
    float2 vLeftTop = vCenter - (vScale * 0.5f);
    float2 vRightBot = vCenter + (vScale * 0.5f);
        
    if (_ID.x < vLeftTop.x || vRightBot.x < _ID.x
        || _ID.y < vLeftTop.y || vRightBot.y < _ID.y)
    {
        return;
    }
        
    if (HasBrushTex)
    {
        float2 vBrushUV = (_ID.xy - vLeftTop) / vScale;
        float Alpha = BRUSH_TEX.SampleLevel(g_AniWrapSampler, vBrushUV, 0).a;
        HeightMapTex[_ID.xy].r += Alpha * g_EngineDT * 0.3f;
    }
    else
    {
        float Cos = cos((distance(vCenter, _ID.xy) / (vScale.x * 0.5f)) * PI * 0.5f);
        Cos = saturate(Cos);
        HeightMapTex[_ID.xy].r += Cos * g_EngineDT * 0.3f;
    }
}

#endif