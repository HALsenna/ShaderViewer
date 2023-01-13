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

cbuffer pointLight : register(b4)
{
	float3 ptPos;
	float ptDummy;
	float4 ptColor;
	float ptRange;
	float3 ptDummy2;
}

cbuffer time : register(b5)
{
	float time;
	float3 timeDummy;
}

cbuffer limlight : register(b6)
{
	float islimlight;
}

Texture2D tex : register(t0);
SamplerState samp : register(s0);

// 関数宣言
float3 calcPointLightDir(float4 world, float3 pointPos);		// 入射するポイントライトの光の向きを計算する
float calcAffect(float4 world, float3 pointPos, float ptRange);
float3 calcSpecular(float3 lightDir, float3 normal, float3 world);
float3 calclimlight(float3 normal, float3 view, float3 lightDir);

float4 main(PS_IN pin) : SV_TARGET
{
	//float4 color = float4(0.5f, 0.5f, 0.5f, 1.0f);		// ライトのカラー
	float4 color = tex.Sample(samp, pin.uv);
	color += float4(lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	float3 N = normalize(pin.normal);		// 法線の方向
	float3 L = normalize(lightDir);		// ライトの方向
	float3 V = normalize(cameraPos.xyz - pin.wPos.xyz);

	// 拡散反射光を計算する
	float diffuse = dot(N, -L);

	// 鏡面反射光を計算する
	float3 specular = calcSpecular(L, N, pin.wPos);

	// ポイントライトの方向を計算する
	float3 pointLightDir = normalize(calcPointLightDir(pin.wPos, ptPos));

	// ポイントライトをちらつかせる
	float lightPower = ptRange * sin(time / 60.0f);
	if (lightPower < 0.0f)
	{
		lightPower = -lightPower;
	}

	// ポイントライトの影響率を計算する
	float affect = calcAffect(pin.wPos, ptPos, lightPower);

	// リムライトの計算をする
	float3 limlight = 0.0f;
	if (islimlight != 0.0f)
	{
		limlight = calclimlight(N, V, L);
	}

	//return float4(limlight,  1.0f);

	// 光を求める
	float3 light = (diffuse + specular) + (affect * ptColor);

	light.x += 0.8f;
	light.y += 0.8f;
	light.z += 0.8f;
	
	color.xyz = color.xyz * light + limlight;

	return color;
}

// 鏡面反射光を計算する
float3 calcSpecular(float3 lightDir, float3 normal, float3 world)
{
	// 反射ベクトルを求める
	float3 refVec = reflect(lightDir, normal);
	// 視点ベクトルを求める
	float3 eyeVec = normalize(cameraPos.xyz - world.xyz);
	// 鏡面反射の強さを求める
	float specVal = max(0, dot(refVec, eyeVec));
	// 鏡面反射を絞る
	specVal = pow(specVal, 10.0f);
	// 鏡面反射光を求める
	float3 specular = specVal;
	
	return specular;
}

// ポイントライトの光の向きを計算する
float3 calcPointLightDir(float4 world, float3 pos)
{
	float3 _lightDir = world - pos;
	_lightDir = normalize(_lightDir);

	return _lightDir;
}

// 距離による影響率を計算する
float calcAffect(float4 world, float3 pos, float range)
{
	// 距離を計算する
	float distance = length(world - pos);

	// 影響率を計算する
	float affect = 1.0f - 1.0f / range * distance;

	if (affect < 0.0f)
	{
		affect = 0.0f;
	}

	affect = pow(affect, 3.0f);

	return affect;
}

// リムライトの計算
float3 calclimlight(float3 normal, float3 view, float3 lightDir)
{
	// 縁の計算
	float3 edgeColor = float3(0.0f, 1.0f, 0.0f);
	float edge = min(1.0f - dot(normal, view), 1.0f);
	edge = pow(edge, 3.0f);

	// リムライトの強度の計算
	float lim = max(0, -dot(lightDir, view));		// 負の値が出ないように丸め込む
	
	return edgeColor * edge * lim;
}