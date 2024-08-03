cbuffer cbuff0:register(b0)
{
    matrix viewPro;
    matrix world;
    float3 cameraPos;
};

cbuffer ConstBufferDataTransform : register(b1)
{
    float explosion;
    float polygonSize;
    float gravity;
    float rotation;
    bool lightFlag;
    bool toonFlag;
    bool bllomFlag;
};

cbuffer Material : register(b2)
{
    float4 color : packoffset(c0); //êF(RGBA)
	float3 ambient :packoffset(c1);
    float3 diffuse : packoffset(c2);
    float bloom : packoffset(c2.w);
	float3 specular : packoffset(c3);
	float alpha : packoffset(c3.w);
};

static const int LIGHT_MAX = 3;
static const int DLIGHT_NUM = LIGHT_MAX;

struct DLight
{
	float3 lightv;
	float3 lightcolor;
	uint active;
};

static const int PLIGHT_NUM = 100;

struct PLight
{
	float3 lightpos;
	float3 lightcolor;
	float3 lightatten;
	uint active;
};

static const int SLIGHT_NUM = 100;

struct SLight
{
	float3 lightv;
	float3 lightpos;
	float3 lightcolor;
	float3 lightatten;
	float2 lightFactorAngleCos;
	uint active;
};

static const int CSHADOW_NUM = 10;

struct CShadow
{
	float3 shadowv;
	float3 casterpos;
	float distanceCasterLight;
	float3 shadowatten;
	float2 shadowFactorAngleCos;
	uint active;
};

cbuffer LightBuff : register(b3)
{
	float3 ambientColor;
	DLight dLights[DLIGHT_NUM];
	PLight pLights[PLIGHT_NUM];
	SLight sLights[SLIGHT_NUM];
	CShadow cShadows[CSHADOW_NUM];
}

cbuffer Fog : register(b4)
{
    float4 m_FogColor;
    float m_Near;
    float m_Far;
    bool fogFlag;
}

static const int MAX_BONES = 128;

cbuffer skinning : register(b5)
{
    Matrix matSkinning[MAX_BONES];
}

cbuffer Judge : register(b6)
{
    bool enemy;
};

struct VSInput
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    uint4 boneIndices : BONEINDICES;
    float4 boneWeights : BONEWEIGHTS;
};

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float4 worldpos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct SkinOutput
{
    float4 pos;
	float3 normal;
};

struct GSOutput
{
	float4 svpos : SV_POSITION;
	float4 worldpos : POSITION;
	float3 normal :NORMAL;
	float2 uv :TEXCOORD;
};

struct PSOutput
{
    float4 target0 : SV_TARGET0;
    float4 target1 : SV_TARGET1;
};