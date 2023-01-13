struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 wPos : TEXCOORD1;
};

//cbuffer Light : register(b0)
//{
//	float3 lightDir;
//	float dummy;
//	float4 lightColor;
//	float4 ptPos;
//	float4 ptColor;
//	float ptRange;
//	float3 _dummy;
//};

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
};

cbuffer condition : register(b2)
{
	float condition;
	float maxTime;
	float2 dummy4;
};

cbuffer specular : register(b3)
{
	float specular;
	float3 dummy3;
};

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

Texture2D tex : register(t0);
SamplerState samp : register(s0);

// 関数宣言
float3 calcPointLightDir(float4 world, float3 pointPos);		// 入射するポイントライトの光の向きを計算する
float calcAffect(float4 world, float3 pointPos, float ptRange);

float4 main(PS_IN pin) : SV_TARGET
{
	//return float4(sin(time / 60.0f), 0.0f, 0.0f, 0.0f);
	//return float4(ptPos.x, ptPos.y, ptPos.z, 1.0f);
	float3 decColor = float3(0.45f, 0.3f, 0.18f);
	float4 color = tex.Sample(samp, pin.uv);

	color += float4(lightColor.x, lightColor.y, lightColor.z, lightColor.w);

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
	float smoothVal = max(0, specular);
	specVal = pow(specVal, smoothVal);
	// 鏡面反射光を求める
	float3 specular = specVal;

	// --- ポイントライト --------------------------------------------------------
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
	// ------------------------------------------------------------------------
	//return float4(affect, 0.0f, 0.0f, 0.0f);

	// 光を求める
	//float4 pointColor = pow(ptColor, ptPower);
	//float4 pointColor = ptColor * ptPower;
	float3 light = (diffuse + specular) + (affect * ptColor);
	//float3 light = diffuse + specular;

	light.x += 0.4f;
	light.y += 0.4f;
	light.z += 0.4f;

	// 焼き色の設定
	float degree = condition;
	
	if(degree > 0.0f)
		color.rgb -= decColor * (degree / maxTime);
	
	color.xyz = color.xyz * light;

	return color;
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