

StructuredBuffer<float> inBuff0 : register(t0);

RWStructuredBuffer<float> outBuff : register(u0);
//id���������ނ�����CS
[numthreads(4, 4, 4)]
void main(uint3 dtid : SV_DispatchThreadID)
{
	//��������ID����
    outBuff[dtid.x * 8 * 8 + dtid.y * 8 + dtid.z] = dtid.x * 8 * 8 + dtid.y * 8 + dtid.z;

}
