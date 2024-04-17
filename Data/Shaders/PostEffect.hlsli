cbuffer cbuff0 : register(b0)
{
    float shift;
    float _NFocusWidth;
    float _FFocusWidth;
    float _FocusDepth;
    float contrast;
    float brightness;
};

// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
    float4 svpos : SV_POSITION; // �V�X�e���p���_���W
    float2 uv : TEXCOORD; // uv�l
};
