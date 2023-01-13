struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

sampler samp : register(s0);
Texture2D tex : register(t0);

float4 main(PS_IN pin) : SV_TARGET
{
	return float4(1.0f, 0.0f, 1.0f, 1.0f);
	//return tex.Sample(samp, pin.uv);
}