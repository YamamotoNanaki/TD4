#include "EnemyHighlighting.hlsli"

Texture2D<float4> tex0 : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex1 : register(t1); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
    float4 enemycolor = tex0.Sample(smp, input.uv);
    float4 texcolor = float4(0, 0, 0, 0);
    if (enemycolor.a == 1)
    {
        texcolor = float4(1, 0.45f, 0.15f, 1);
    }
    //float4 player = tex1.Sample(smp, input.uv);
    //if (player.a==1)
    //{
    //    texcolor = player;
    //}
    return texcolor;
}