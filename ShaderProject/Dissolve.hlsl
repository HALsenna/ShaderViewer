struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 wPos : TEXCOORD1;
};

cbuffer Light : register(b0)
{
	float4 lightDir;
};

cbuffer Camera : register(b1)
{
	float4 camPos;
	float fogStart;
	float fogRange;
	float disRate;
	float dummy;
};

SamplerState samp : register(s0);		
Texture2D tex: register(t0);
Texture2D dissolveTex : register(t1);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャの貼り付け
	color.rgb = tex.Sample(samp, pin.uv);
	
	// 黒白の画像の該当箇所の色を拾ってくる
	float dissolve = dissolveTex.Sample(samp, pin.uv).r;
	// その色がrateと比べて暗いか明るいか
	// シェーダー→if文は重たい処理になるから極力避ける
	// step関数→1か0しか返さない(x >= a) ? 1:0
	float show = step(disRate, dissolve);

	color.rgb *= show;
	return color;
}