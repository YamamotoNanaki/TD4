#include "DefaultPE.hlsli"
#include "Gaussian.hlsli"

Texture2D<float4> tex0 : register(t0); // 0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor = tex0.Sample(smp, input.uv);
    texcolor.a = 1;
    texcolor += GaussianBlur(input.uv, tex1, smp);
    return texcolor;
}