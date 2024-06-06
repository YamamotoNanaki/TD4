#include "EnemyJudgeAnim.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(GSOutput input) : SV_TARGET
{
    if (color.a == 0)
    {
        discard;
    }
    if (enemy)
        return float4(1, 1, 1, 1);
    return float4(0, 0, 0, 0);
}