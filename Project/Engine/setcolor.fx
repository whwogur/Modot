#ifndef _SETCOLOR
#define _SETCOLOR

#include "value.fx"

RWTexture2D<float4> g_TargetTex : register(u0);

[numthreads(32, 32, 1)]
void CS_SetColor(int3 _ID : SV_DispatchThreadID)
{
     // 픽셀을 초과해서 접근하는 스레드(초과 배정된 스레드)
    if (g_int_0 <= _ID.x || g_int_1 <= _ID.y)
        return;

    // 픽셀 좌표 == _ID.xy    
    if (500 <= _ID.y && _ID.y <= 524)
        g_TargetTex[_ID.xy] = g_vec4_0;
    else if (500 <= _ID.x && _ID.x <= 524)
        g_TargetTex[_ID.xy] = g_vec4_0;
    else
        g_TargetTex[_ID.xy] = float4(0.f, 0.f, 0.f, 1.f);
}

#endif