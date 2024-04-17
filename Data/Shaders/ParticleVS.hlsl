#include "Particle.hlsli"

VSOutput main(float4 pos : POSITION,uint instanceID:SV_InstanceID)
{
	VSOutput output;//ピクセルシェーダーに渡す値
	output.pos = pos;
    output.instanceID = instanceID;
	return output;
}