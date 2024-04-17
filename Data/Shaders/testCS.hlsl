

StructuredBuffer<float> inBuff0 : register(t0);

RWStructuredBuffer<float> outBuff : register(u0);
//id‚ğ‘‚«‚Ş‚¾‚¯‚ÌCS
[numthreads(4, 4, 4)]
void main(uint3 dtid : SV_DispatchThreadID)
{
	//‚½‚¾‚½‚¾ID‚ğ‘ã“ü
    outBuff[dtid.x * 8 * 8 + dtid.y * 8 + dtid.z] = dtid.x * 8 * 8 + dtid.y * 8 + dtid.z;

}
