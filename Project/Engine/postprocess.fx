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
    float adjustedTime = g_float_3 * 0.3;
    float offset = (adjustedTime - floor(adjustedTime)) / adjustedTime;
    float CurrentTime = (adjustedTime) * (offset);

    float3 WaveParams = float3(17, 1.7, 0.07); // TODO (�ϵ��ڵ��)

    float2 WaveCentre = mul(g_vec4_3, matWVP).xy / g_Resolution; // TODO (�ϵ��ڵ��)

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
        texCoord += ((DiffTexCoord * DiffTime) / (CurrentTime * Dist * 10.0));
        Color = g_tex_0.Sample(g_sam_0, texCoord);

        // Blow out the color and reduce the effect over time
        Color += (Color * ScaleDiff) / (CurrentTime * Dist * 40.0);
    }

    return Color;
}


// ����� �� ���� ����. �������� ������, ������ �ӵ��� ����ؾ� ��
static const float SAMPLES = 24.0;

// 2x1 �ؽ� �Լ�. ������ �ұ�Ģ�ϰ� �ϴ� �� ���.
float hash(float2 p)
{
    return frac(sin(dot(p, float2(41, 289))) * 45758.5453);
}

float3 lOff(float iTime)
{
    float2 u = sin(float2(1.57, 0) - iTime / 2.0);
    float2x2 a = float2x2(u.x, u.y, -u.y, u.x);
    
    float3 l = normalize(float3(1.5, 1.0, -0.5));
    l.xz = mul(a, l.xz);
    l.xy = mul(a, l.xy);
    
    return l;
}

float4 PS_Godray(VS_OUT _in) : SV_Target
{
    if (g_int_3)
    {
        float2 uv = _in.vPosition.xy / g_Resolution.xy;

        // ����� �� ���ڵ�.
        //
        // �ܰ����� ���Ǵ� ���� ����.
        float decay = g_float_0;
        // ���� �е���, �̴� ������ Ȯ�� ����.
        float density = g_float_1;
        // ���� ����ġ. �ܰ����� ���Ǵ� ���� ����
        float weight = g_float_2;
    
        float3 l = lOff(g_EngineTime);
    
        // �ؽ�ó ��ǥ ������ (uv - 0.5), �ٽ� ��¥ ���� ���������� ������ ����.
        // �� ���� �� ����(������ ���� ����)�� �����ϴ� �� ���Ǹ�, 
        // ���� ����Ʈ����Ʈ �߽��� �����ϴ� �� ����
        //
        // ������ 0�� �߽����� �Ͽ�, ��� �������� ���� �� �ֵ��� ��
        // ��, �����
        float2 tuv = uv - 0.5 - l.xy * 0.45;
    
        // ���� ���� ���͸� ���� ������ �е� ���ڷ� ������
        // �е� ���� ���� ������ ������ �����Ѵ�. �е��� �������� �� �ݰ��� Ŀ����
        float2 dTuv = tuv * density / SAMPLES;
    
        // �ʱ� �ؽ�ó ������ �Ϻθ� �����ɴϴ�. ���� Ŭ���� ����� �� ��������
        float4 col = g_tex_0.Sample(g_sam_2, uv) * 0.25;
    
        // ������ �ҿ��������� ������ �� ����� �����ϱ� ���� ���͸� 
        // Ư�� ���� ���̾ ���� �� �ſ� �߿�
        uv += dTuv * (hash(uv.xy + frac(g_EngineTime)) * 2.0 - 1.0);
    
        // ����� �� ���� �ؽ�ó ������ ��������, 
        // ����� ���� ����(dTuv) �������� ���� �̵��� �� 
        // ����ġ�� �ణ ���� ������ �߰�. �� ������ ���� ������ŭ �ݺ�
        for (float i = 0.0; i < SAMPLES; i++)
        {
            uv -= dTuv;
            col += g_tex_0.Sample(g_sam_2, uv) * weight;
            weight *= decay;
        }
    
        // ����� ���� ������ �߽��� �� ����Ʈ����Ʈ�� ���� ������ ���Ѵ� 
        col *= (1.0 - dot(tuv, tuv) * 0.75);
    
        // ���� ������ �ε巴�� �����ϰ�, �ణ�� ���� ����
        return sqrt(smoothstep(0.0, 1.0, col));
    }
    else
    {
        return g_tex_0.Sample(g_sam_0, _in.vPosition.xy / g_Resolution.xy);
    }
}

#endif