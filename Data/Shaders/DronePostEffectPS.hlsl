#include "DronePostEffect.hlsli"

Texture2D<float4> tex0 : register(t0); // 0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1); // 1番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

bool isWhite(float4 color)
{
    return color.r == 1.0 && color.g == 1.0 && color.b == 1.0 && color.a == 1.0;
}

float2 randomVec(float2 fact)
{
    float2 angle = float2(dot(fact, float2(127.1, 311.7)), dot(fact, float2(269.5, 183.3)));
    return frac(sin(angle) * 43758.5453123) * 2 - 1;
}

float PerinNoise(float density, float2 uv)
{
    float2 uvFloor = floor(uv * density);
    float2 uvFrac = frac(uv * density);

    float2 v00 = randomVec(uvFloor + float2(0, 0));
    float2 v01 = randomVec(uvFloor + float2(0, 1));
    float2 v10 = randomVec(uvFloor + float2(1, 0));
    float2 v11 = randomVec(uvFloor + float2(1, 1));

    float c00 = dot(v00, uvFloor - float2(0, 0));
    float c01 = dot(v01, uvFloor - float2(0, 1));
    float c10 = dot(v10, uvFloor - float2(1, 0));
    float c11 = dot(v11, uvFloor - float2(1, 1));

    float2 u = uvFrac * uvFrac * (3 - 2 * uvFrac);

    float v0010 = lerp(c00, c10, u.x);
    float v0111 = lerp(c01, c11, u.x);

    return lerp(v0010, v0111, u.y) / 2 + 0.5;
}

float FractalSumNoise(float density, float2 uv)
{
    float fn;
    fn = PerinNoise(density * 1, uv) * 1.0 / 2;
    fn += PerinNoise(density * 2, uv) * 1.0 / 4;
    fn += PerinNoise(density * 4, uv) * 1.0 / 8;
    fn += PerinNoise(density * 8, uv) * 1.0 / 16;
    return fn;
}

float2 gradientNoiseDir(float2 p)
{
    p = p % 289;
    float x = (34 * p.x + 1) * p.x % 289 + p.y;
    x = (34 * x + 1) * x % 289;
    x = frac(x / 41) * 2 - 1;
    return normalize(float2(x - floor(x + 0.5), abs(x) - 0.5));
}

float gradientNoise(float2 p)
{
    float2 ip = floor(p);
    float2 fp = frac(p);
    float d00 = dot(gradientNoiseDir(ip), fp);
    float d01 = dot(gradientNoiseDir(ip + float2(0, 1)), fp - float2(0, 1));
    float d10 = dot(gradientNoiseDir(ip + float2(1, 0)), fp - float2(1, 0));
    float d11 = dot(gradientNoiseDir(ip + float2(1, 1)), fp - float2(1, 1));
    fp = fp * fp * fp * (fp * (fp * 6 - 15) + 10);
    return lerp(lerp(d00, d01, fp.y), lerp(d10, d11, fp.y), fp.x);
}

float GradientNoiseFloat(float2 UV, float Scale)
{
    return gradientNoise(UV * Scale) + 0.5;
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
        float t = time * -100;
        float s = sin((input.uv.y) * 1000 + t) / 2 + 0.5f;
        texcolor *= lerp(0.9f, 1.0f, s);
    }

    return texcolor;
}