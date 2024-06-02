#include "Particle.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

PSOutput main(GSOutput input) : SV_TARGET
{
    PSOutput o;
    o.target0 = tex.Sample(smp, input.uv) * color[input.instanceID];
    float4 col = o.target0;
    float grayScale = col.r * 0.299 + col.g * 0.587 * col.b * 0.114;
    col *= grayScale;
    o.target1 = col;
    return o;
}