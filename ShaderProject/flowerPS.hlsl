struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 wPos : TEXCOORD1;
};

float4 color : register(b0);

float4 main(PS_IN pin) : SV_TARGET
{
	return color;
}