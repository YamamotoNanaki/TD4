#include "PostEffect.hlsli"

Texture2D<float4> tex0 : register(t0); // 0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1); // 1番スロットに設定されたテクスチャ
Texture2D<float4> tex2 : register(t2); // 2番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float Gaussian(float2 drawuv, float2 pickuv, float sigma)
{
    float d = distance(drawuv, pickuv);
    return exp(-(d * d) / (2 * sigma * sigma));
}

float4 GaussianBlur(float2 uv, Texture2D<float4> tex, float _sigma = 0.005)
{
    if (_sigma == 0)
    {
        return tex.Sample(smp, uv);
    }
    uv = clamp(uv, 0, 1);
    float totalWeight = 0, _stepWidth = 0.001;
    float4 col = float4(0, 0, 0, 1);

	[loop]
    for (float py = -_sigma * 2; py <= _sigma * 2; py += _stepWidth)
    {
        [loop]
        for (float px = -_sigma * 2; px <= _sigma * 2; px += _stepWidth)
        {
            float2 pickuv = uv + float2(px, py);
            if (pickuv.x < 0 || pickuv.y < 0 || pickuv.x > 1 || pickuv.y > 1)
                continue;
            float weight = Gaussian(uv, pickuv, _sigma);
            col += tex.Sample(smp, pickuv) * weight;
            totalWeight += weight;
        }
    }
    col.rgb = col.rgb / totalWeight;
    return col;
}

float4 GaussianBlurShift(float2 uv, Texture2D<float4> tex, float _sigma = 0.005)
{
    if (_sigma == 0)
    {
        return tex.Sample(smp, uv);
    }
    uv = clamp(uv, 0, 1);
    float totalWeight = 0, _stepWidth = 0.001;
    float4 col = float4(0, 0, 0, 1);

	[loop]
    for (float py = -_sigma * 2; py <= _sigma * 2; py += _stepWidth)
    {
        [loop]
        for (float px = -_sigma * 2; px <= _sigma * 2; px += _stepWidth)
        {
            float2 pickuv = uv + float2(px, py);
            if (pickuv.x < 0 || pickuv.y < 0 || pickuv.x > 1 || pickuv.y > 1)
                continue;
            float weight = Gaussian(uv, pickuv, _sigma);
            col.r += tex.Sample(smp, pickuv + float2(-_sigma / 2, _sigma / 2)).r * weight;
            col.g += tex.Sample(smp, pickuv + float2(0, -_sigma / 2)).g * weight;
            col.b += tex.Sample(smp, pickuv + float2(_sigma / 2, 0)).b * weight;
            totalWeight += weight;
        }
    }
    col.rgb = col.rgb / totalWeight;
    return col;
}

float4 GaussianDepthBlur(float2 uv, Texture2D<float4> tex, float focusWidth, float _FocusDepth, float _sigma = 0.005)
{
    float totalWeight = 0, _stepWidth = 0.001;
    float4 col = float4(0, 0, 0, 1);
	[loop]
    for (float py = -_sigma * 2; py <= _sigma * 2; py += _stepWidth)
    {
	    [unroll]
        for (float px = -_sigma * 2; px <= _sigma * 2; px += _stepWidth)
        {
            float2 pickuv = uv + float2(px, py);
            if (pickuv.x < 0 || pickuv.y < 0 || pickuv.x > 1 || pickuv.y > 1)
                continue;
            float pickDepth = tex1.Sample(smp, pickuv).r;
            float pickFocus = smoothstep(0, focusWidth, abs(pickDepth - _FocusDepth));
            float weight = Gaussian(uv, pickuv, _sigma) * pickFocus;
            col += tex.Sample(smp, pickuv) * weight;
            totalWeight += weight;
        }
    }
    col.rgb = col.rgb / totalWeight;
    return col;
}

float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor = tex0.Sample(smp, input.uv);
    //float depth = tex2.Sample(smp, input.uv).r;
    //float inFocus = 1 - smoothstep(0, _NFocusWidth, abs(depth - _FocusDepth));
    //float outFocus = smoothstep(_NFocusWidth, _FFocusWidth, abs(depth - _FocusDepth));
    //float middleFocus = 1 - inFocus - outFocus;
    //float4 inFocusColor;
    //float4 middleFocusColor;
    //float4 outFocusColor;

    //inFocusColor = tex0.Sample(smp, input.uv);
    //middleFocusColor = GaussianDepthBlur(input.uv, tex0, _NFocusWidth, _FocusDepth, 0.001);
    //outFocusColor = GaussianDepthBlur(input.uv, tex0, _FFocusWidth, _FocusDepth);

    //texcolor = inFocus * inFocusColor + middleFocus * middleFocusColor + outFocus * outFocusColor;
    texcolor = GaussianBlur(input.uv, tex0, shift);
    texcolor += GaussianBlur(input.uv, tex1);

    texcolor = pow(texcolor + float4(brightness.rrr,1), contrast);
    
    
    return texcolor;
}