#ifndef _SETCOLOR
#define _SETCOLOR

#include "value.fx"

RWTexture2D<float4> g_TargetTex : register(u0);

[numthreads(32, 32, 1)]
void CS_SetColor(int3 _ID : SV_DispatchThreadID)
{
     // �ȼ��� �ʰ��ؼ� �����ϴ� ������(�ʰ� ������ ������)
    if (g_int_0 <= _ID.x || g_int_1 <= _ID.y)
        return;

    // �ȼ� ��ǥ == _ID.xy    
    if (500 <= _ID.y && _ID.y <= 524)
        g_TargetTex[_ID.xy] = g_vec4_0;
    else if (500 <= _ID.x && _ID.x <= 524)
        g_TargetTex[_ID.xy] = g_vec4_0;
    else
        g_TargetTex[_ID.xy] = float4(0.f, 0.f, 0.f, 1.f);
}

#endif