cbuffer cbuff0 : register(b0)
{
    float2 center; // ���S�_ (0.5, 0.5 �Ȃ�)
    float blurAmount; // �u���[�̋���
};

// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
    float4 svpos : SV_POSITION; // �V�X�e���p���_���W
    float2 uv : TEXCOORD; // uv�l
};
