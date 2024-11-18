#ifndef _WEIGHTMAP
#define _WEIGHTMAP
#include "value.fx"
#include "struct.fx"
RWStructuredBuffer<tWeight8> WEIGHT_MAP : register(u0);
StructuredBuffer<tRaycastOut> Raycast : register(t20);
#define WIDTH                   g_int_0
#define HEIGHT                  g_int_1
#define WEIGHT_IDX              g_int_2
#define BRUSH_TEX               g_tex_0
#define BRUSH_SCALE             g_vec2_0

[numthreads(32, 32, 1)]
void CS_WeightMap(int3 _ID : SV_DispatchThreadID)
{
    if (WIDTH <= _ID.x || HEIGHT <= _ID.y || !Raycast[0].Success)
        return;
    
    // ���콺 ��ŷ��ġ�� 2���� �ε����� ��ȯ
    int2 CenterPos = float2(WIDTH, HEIGHT) * Raycast[0].Location;
    
    // �귯�� ������ �ػ� ������ ����
    int2 Scale = float2(WIDTH, HEIGHT) * BRUSH_SCALE;
       
    // ���콺 ��ŷ ��ġ�� �߽����� �귯�� ���� ������ ��� ������ ����ϴ� ��������� �����Ѵ�.
    if (_ID.x < CenterPos.x - (Scale.x / 2) || CenterPos.x + (Scale.x / 2) < _ID.x
        || _ID.y < CenterPos.y - (Scale.y / 2) || CenterPos.y + (Scale.y / 2) < _ID.y)
    {
        return;
    }
        
    // (Thread ��ǥ - �»�� ��ǥ) / �귯�� ������ ==> �ش� �����尡 �����ؾ��ϴ� �귯���ؽ��� UV ��ġ
    int2 LeftTop = CenterPos - (Scale / 2);
    float2 vBrushUV = (float2) (_ID.xy - LeftTop) / (float2) Scale;
    float Alpha = BRUSH_TEX.SampleLevel(g_AniWrapSampler, vBrushUV, 0).a;
        
    // Thread �� ����� WEIGHT_MAP ������ 1 ���� �ε����� ���Ѵ�.
    int WeightMapIdx = WIDTH * _ID.y + _ID.x;
        
    // ����ġ �� Ȯ��
    float weight = WEIGHT_MAP[WeightMapIdx].arrWeight[WEIGHT_IDX];
    float Add = 2.f * g_EngineDT * Alpha;
    
    // ����ġ �߰�
    WEIGHT_MAP[WeightMapIdx].arrWeight[WEIGHT_IDX] = saturate(weight + Add);
        
    // �ʰ��� Ȯ��
    float Total = 0.f;
    for (int i = 0; i < 8; ++i)
    {
        Total += WEIGHT_MAP[WeightMapIdx].arrWeight[i];
    }
    
    if (1.f < Total)
    {
        float Over = Total - 1.f;
        
        for (int i = 0; i < 8; ++i)
        {
            if (WEIGHT_IDX == i)
                continue;
            
            float Ratio = WEIGHT_MAP[WeightMapIdx].arrWeight[i] / (Total - WEIGHT_MAP[WeightMapIdx].arrWeight[WEIGHT_IDX]);
            
            WEIGHT_MAP[WeightMapIdx].arrWeight[i] -= Ratio * Over;
            
            if (WEIGHT_MAP[WeightMapIdx].arrWeight[i] < 0.f)
                WEIGHT_MAP[WeightMapIdx].arrWeight[i] = 0.f;
        }
    }
}
#endif