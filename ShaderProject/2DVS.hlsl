// ����
struct VS_IN
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD;
};

// �o��
struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

VS_OUT main(VS_IN vin)
{
	VS_OUT vout = (VS_OUT)0;

	// �ϊ������ɏo��
	vout.pos = vin.pos;
	vout.uv = vin.uv;
	return vout;
}