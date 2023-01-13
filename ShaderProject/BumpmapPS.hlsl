struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 wPos : TEXCOORD1;
	float3 tangentL : TEXCOORD2;
};

cbuffer Light : register(b0)
{
	float4 lightDir;
};

cbuffer Camera : register(b1)
{
	float4 camPos;
}

SamplerState samp : register(s0);
Texture2D normalMap : register(t1);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1.0f, 1.0f, 0.0f, 1.0f);

	// モデルの法線をそのまま使う場合
	// float3 N = normalize(pin.normal);

	// 法線マップから代わりの法線を取り出す
	float3 N = normalMap.Sample(samp, pin.uv);
	N = N * 2.0f - 1.0f;		// 0～1の色データを、-1～1のベクトルデータに変換
	N = normalize(N);

	float3 L = normalize(-pin.tangentL);

	float diffuse = dot(N, L);
	float ambient = 0.5f;

	//N = normalize(pin.normal);

	float3 R = reflect(-L, N);
	float3 V = normalize(camPos.xyz - pin.wPos.xyz);

	// 鏡面反射(スペキュラー:specular)
	// 反射ベクトルと視線ベクトルの一致具合で反射の強さを表す
	float specular = max(0, dot(R, V));			// 内積の計算結果がマイナスになった

	// そのままだと全体が明るいので累乗するといい感じになる
	specular = pow(specular, 30.0f);

	color.rbg = diffuse * 0.45f + ambient + specular;

	return color;
}
