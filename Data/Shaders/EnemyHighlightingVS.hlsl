#include "EnemyHighlighting.hlsli"

VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
    VSOutput output; // ピクセルシェーダーに渡す値
    output.svpos = pos;
    output.uv = uv;
    return output;
}