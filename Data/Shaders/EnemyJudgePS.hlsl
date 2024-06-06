#include "EnemyJudge.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

PSOutput main(GSOutput input) : SV_TARGET
{
    if (color.a == 0)
    {
        discard;
    }
    PSOutput output;
    if (enemy)
    {
        output.target0 = float4(1, 1, 1, 1);
        output.target1 = float4(0, 0, 0, 0);
    }
    else
    {
        output.target0 = float4(0, 0, 0, 0);
        output.target1 = tex.Sample(smp, input.uv);
    }
    return output;
}