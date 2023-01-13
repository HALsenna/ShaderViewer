struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 wPos : TEXCOORD1;
};

cbuffer Light : register(b0)
{
	float3 lightDir;
	float dummy;
	float4 lightColor;
	float smooth;
	float3 dummy2;
};

cbuffer Camera : register(b1)
{
	float4 cameraPos;
}

Texture2D tex : register(t0);		// テクスチャ
SamplerState samp : register(s0);	// サンプラー

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = tex.Sample(samp, pin.uv);
	//float4 color = float4(0.43f, 0.5f, 0.21f, 1.0f);
	color.x += float4(lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	float3 N = normalize(pin.normal);		// 法線の方向
	float3 L = normalize(lightDir);		// ライトの方向

	// 拡散反射光を計算する
	float diffuse = dot(N, -L);

	// 鏡面反射光を計算する
	// 反射ベクトルを求める
	float3 refVec = reflect(L, N);
	// 視点ベクトルを求める
	float3 eyeVec = normalize(cameraPos.xyz - pin.wPos.xyz);
	// 鏡面反射の強さを求める
	float specVal = max(0, dot(refVec, eyeVec));
	// 鏡面反射を絞る
	float smoothVal = max(0, 10.0f);
	specVal = pow(specVal, smoothVal);
	// 鏡面反射光を求める
	float3 specular = specVal;

	// 光を求める
	float3 light = diffuse + specular;

	light.x += 0.4f;
	light.y += 0.4f;
	light.z += 0.4f;

	color.xyz = color.xyz * light;

	return color;
}