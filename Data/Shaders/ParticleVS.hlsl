#include "Particle.hlsli"

VSOutput main(float4 pos : POSITION,uint instanceID:SV_InstanceID)
{
	VSOutput output;//�s�N�Z���V�F�[�_�[�ɓn���l
	output.pos = pos;
    output.instanceID = instanceID;
	return output;
}