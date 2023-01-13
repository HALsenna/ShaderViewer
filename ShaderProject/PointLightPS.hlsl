struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 wPos : TEXCOORD1;
};

cbuffer Camera : register(b0)
{
	float4 cameraPos;
}

cbuffer Light : register(b1)
{
	float4 lightPos;
	float4 lightColor;
}

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float4 viewPos = cameraPos;
	float3 N = pin.normal;
	float power = dot(cameraPos, N);
	color *= power;
	return color;
}