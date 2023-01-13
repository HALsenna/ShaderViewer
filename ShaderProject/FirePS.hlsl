struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 wPos : TEXCOORD1;
};

cbuffer color : register(b0)
{
	float time;
	float3 timeDummy;
}

Texture2D tex : register(t0);		// テクスチャ
SamplerState samp : register(s0);	// サンプラー

float4 main(PS_IN pin) : SV_TARGET
{
	float val = sin(time / 60.0f);
	if (val < 0.0f)
	{
		val = -val;
	}
	float4 color = float4(1.0f, val * 0.3f , 0.0f, 1.0f);
	

	return color;
}