struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 wPos : TEXCOORD1;
};

SamplerState samp : register(s0);
Texture2D tex: register(t0);

cbuffer alpha : register(b0)
{
	float alpha;
	float3 dummy;
}

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1.0f, 1.0f, 1.0f, alpha);
	// テクスチャの貼り付け
	color.rgb = tex.Sample(samp, pin.uv);
	return color;
}