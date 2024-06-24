#include "DronePostEffect.hlsli"

Texture2D<float4> tex0 : register(t0); // 0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor;
    if (drone)
    {
        float sb = (noisePosY);
        if (sb > 1)
            sb -= 1;
        float se = sb + noiseWidth;
        float2 uv = float2(input.uv.x + sin(smoothstep(sb, se, input.uv.y) * 2 * 3.14159) * noisePower, input.uv.y);
        input.uv = uv;
    }
    texcolor = tex0.Sample(smp, input.uv);
    if (tex1.Sample(smp, input.uv).r == 1)
    {
        texcolor.rgb = float3(1, 1, 1);
    }
    texcolor.a = 1;
    return texcolor;
}