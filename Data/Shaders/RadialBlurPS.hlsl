#include "RadialBlur.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    float2 uv = input.uv;
    float2 dir = uv - center;
    float len = length(dir);
    float stepSize = blurAmount / 10.0;

    float4 color = float4(0, 0, 0, 0);

    for (float i = 0; i < 10; i++)
    {
        float scale = 1.0 - (i * stepSize / len);
        float2 sampleUV = center + dir * scale;
        color += tex.Sample(smp, sampleUV);
    }

    color /= 10.0;
    
    //float2 direction = input.uv - center;
    //for (int i = 0; i < 10; i++)
    //{
    //    float uv = input.uv + blurAmount * direction * float(i);
    //    color.rgb += tex.Sample(smp, uv).rgb;
    //}
    
    //color.rgb /= 10;
    //color.a = 1;
    return color;
}