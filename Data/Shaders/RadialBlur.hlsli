cbuffer cbuff0 : register(b0)
{
    float2 center; // 中心点 (0.5, 0.5 など)
    float blurAmount; // ブラーの強さ
};

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
    float4 svpos : SV_POSITION; // システム用頂点座標
    float2 uv : TEXCOORD; // uv値
};
