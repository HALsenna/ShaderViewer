struct VS_IN
{
	float3 pos : POSITION0;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float3 tangent : TANGENT0;		// 接ベクトル情報
};
struct VS_OUT
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 wPos:TEXCOORD1;
	float3 tangentL : TEXCOORD2;		// 計算が重くなるので、法線ではなくライトを回転させる
};
cbuffer WVP : register(b0)
{
	float4x4 world;
	float4x4 view;
	float4x4 proj;
};
// ライトの情報
cbuffer Light : register(b1)
{
	float4 lightDir;
};

VS_OUT main(VS_IN vin)
{
	VS_OUT vout;
	vout.pos = float4(vin.pos, 1.0f);

	// ローカル座標 > ワールド座標 > ビュー座標 > プロジェクション座標へと変換をかける
	vout.pos = mul(vout.pos, world);
	vout.pos = mul(vout.pos, view);
	vout.pos = mul(vout.pos, proj);
	vout.wPos = vout.pos;

	vout.normal = vin.normal;
	vout.uv = vin.uv;

	// バンプマップのために、ライトを回転させる
	float3 T = normalize(vin.tangent);		// tangent
	float3 N = normalize(vout.normal);		// normal
	float3 B = normalize(cross(N, T));		// binormal
	float3x3 bumpRot = float3x3(
		T.x, B.x, N.x,
		T.y, B.y, N.y,
		T.z, B.z, N.z
		);
	float3 L = normalize(lightDir.xyz);
	vout.tangentL = mul(L, bumpRot);

	return vout;
}

