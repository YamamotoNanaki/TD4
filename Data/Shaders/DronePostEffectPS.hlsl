#include "DronePostEffect.hlsli"
#include "Noise.hlsli"

Texture2D<float4> tex0 : register(t0); // 0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1); // 1番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

bool isWhite(float4 color)
{
    return color.r == 1.0 && color.g == 1.0 && color.b == 1.0;
}

float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor;
    //if (drone)
    //{
        //float sb = (noisePosY);
        //if (sb > 1)
        //    sb -= 1;
        //float se = sb + noiseWidth;
        //float2 uv = float2(input.uv.x + sin(smoothstep(sb, se, input.uv.y) * 2 * 3.14159) * noisePower, input.uv.y);
        //input.uv = uv;
    //}
    float2 uv = input.uv;
    if (drone)
    {
        float2 center = float2(0.5, 0.5);
        float2 pos = input.uv - center;
        float len = length(pos);

        float t = time * -40;
        float noise = GradientNoiseFloat(float2(0, input.uv.y + t), 10);
        float mult = lerp(0, 0.75, pow(GradientNoiseFloat(time, 10), 20));
        uv += lerp(-0.025, 0.025, noise) * mult * (len / length(center));
    }
    texcolor = tex0.Sample(smp, uv);

    float2 g_TexelSize = { 1.0 / 1920.0, 1.0 / 1080.0 };
    float2 offsets[16] =
    {
        float2(-g_TexelSize.x, 0.0),
        float2(g_TexelSize.x, 0.0),
        float2(0.0, -g_TexelSize.y),
        float2(0.0, g_TexelSize.y),
        float2(-g_TexelSize.x, -g_TexelSize.y),
        float2(g_TexelSize.x, -g_TexelSize.y),
        float2(-g_TexelSize.x, g_TexelSize.y),
        float2(g_TexelSize.x, g_TexelSize.y),
        float2(-2.0 * g_TexelSize.x, 0.0),
        float2(2.0 * g_TexelSize.x, 0.0),
        float2(0.0, -2.0 * g_TexelSize.y),
        float2(0.0, 2.0 * g_TexelSize.y),
        float2(-2.0 * g_TexelSize.x, -2.0 * g_TexelSize.y),
        float2(2.0 * g_TexelSize.x, -2.0 * g_TexelSize.y),
        float2(-2.0 * g_TexelSize.x, 2.0 * g_TexelSize.y),
        float2(2.0 * g_TexelSize.x, 2.0 * g_TexelSize.y)
    };

    float4 centerColor = tex1.Sample(smp, input.uv);
    bool isEdge = (centerColor == float4(1, 1, 1, 1));

    if (isEdge == true)
    {
        isEdge = false;
        for (int i = 0; i < 16; ++i)
        {
            float4 sampleColor = tex1.Sample(smp, input.uv + offsets[i]);
            if (!isWhite(sampleColor))
            {
                isEdge = true;
            }
        }
    }

    if (isEdge == true)
    {
        texcolor.rgb = float3(1, 0.45f, 0.15f);
    }
    texcolor.a = 1;

    if (drone)
    {
        float t = time * -150;
        float s = sin((input.uv.y) * 1000 + t) / 2 + 0.5f;
        texcolor *= lerp(0.9f, 1.0f, s);
    }

    return texcolor;
}