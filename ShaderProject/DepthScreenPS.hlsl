struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float3 depthInView : TEXCOORD1;		// ƒJƒƒ‰‹óŠÔ‚Å‚ÌZ’l
};

SamplerState samp : register(s0);
Texture2D tex : register(t1);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1.0f, 1.0f, 0.0f, 1.0f);
	return color;
}