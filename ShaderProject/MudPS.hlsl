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

cbuffer Color : register(b2)
{
	float4 fColor;
}

cbuffer TexPos : register(b3)
{
	float4 fTexPos;
}

SamplerState samp : register(s0);
Texture2D normalMap : register(t1);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 fShadowColor = float4(1.0f, 0.0f, 0.0f, 1.0f);
	float4 color = fColor;
	pin.uv.x += fTexPos.x;
	// 法線マップから代わりの法線を取り出す
	float3 N = normalMap.Sample(samp, pin.uv);
	N = N * 2.0f - 1.0f;		// 0～1の色データを、-1～1のベクトルデータに変換
	N = normalize(N);

	float3 L = normalize(-pin.tangentL);

	float diffuse = dot(N, L);
	float ambient = 0.2f;

	float3 R = reflect(-L, N);
	float3 V = normalize(camPos.xyz - pin.wPos.xyz);

	// 鏡面反射(スペキュラー:specular)
	// 反射ベクトルと視線ベクトルの一致具合で反射の強さを表す
	float specular = max(0, dot(R, V));			// 内積の計算結果がマイナスになった

	// そのままだと全体が明るいので累乗するといい感じになる
	specular = pow(specular, 50.0f);

	//color.rgb = color.rgb * diffuse + specular + ambient;
	color.rgb = color.rgb * diffuse + specular;

	return color;
}
