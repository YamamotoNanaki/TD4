cbuffer cbuff0:register(b0)
{
    float4 color[400]; //色(RGBA)
};

cbuffer ConstBufferDataTransform : register(b1)
{
    matrix mat[400]; //3D変換行列
    matrix matBillboard[400]; //ビルボード行列
};

struct VSOutput
{
	float4 pos : POSITION;	//頂点座標
    uint instanceID : TEXCOORD;
};

struct GSOutput
{
	float4 svpos : SV_POSITION;	//頂点座標
    float2 uv : TEXCOORD; //uv値
    uint instanceID : TEXCOORD2;
};

struct PSOutput
{
    float4 target0 : SV_TARGET0;
};
