cbuffer cbuff0:register(b0)
{
    float4 color[400]; //�F(RGBA)
};

cbuffer ConstBufferDataTransform : register(b1)
{
    matrix mat[400]; //3D�ϊ��s��
    matrix matBillboard[400]; //�r���{�[�h�s��
};

struct VSOutput
{
	float4 pos : POSITION;	//���_���W
    uint instanceID : TEXCOORD;
};

struct GSOutput
{
	float4 svpos : SV_POSITION;	//���_���W
    float2 uv : TEXCOORD; //uv�l
    uint instanceID : TEXCOORD2;
};

struct PSOutput
{
    float4 target0 : SV_TARGET0;
    float4 target1 : SV_TARGET1;
    float4 target2 : SV_TARGET2;
};
