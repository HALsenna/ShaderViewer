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
};

cbuffer Camera : register(b1)
{
	float4 cameraPos;
}

Texture2D tex : register(t0);		// �e�N�X�`��
Texture2D normalMap : register(t1);		// �@���}�b�v
SamplerState samp : register(s0);	// �T���v���[

float4 main(PS_IN pin) : SV_TARGET
{
	//float4 color = float4(0.5f, 0.5f, 0.5f, 1.0f);
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	color.rgb = tex.Sample(samp, pin.uv);
	//float4 color = float4(lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	color += float4(lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	float3 N = normalMap.Sample(samp, pin.uv);
	//float3 N = pin.normal;
	N = N * 2.0f - 1.0f;
	N = normalize(N);
	float3 L = normalize(lightDir);		// ���C�g�̕���

	// �g�U���ˌ����v�Z����
	float diffuse = dot(N, -L);

	// ���ʔ��ˌ����v�Z����
	// ���˃x�N�g�������߂�
	float3 refVec = reflect(L, N);
	// ���_�x�N�g�������߂�
	float3 eyeVec = normalize(cameraPos.xyz - pin.wPos.xyz);
	// ���ʔ��˂̋��������߂�
	float specVal = max(0, dot(refVec, eyeVec));
	// ���ʔ��˂��i��
	float smoothVal = max(0, 1.0f);
	specVal = pow(specVal, smoothVal);
	// ���ʔ��ˌ������߂�
	float3 specular = specVal;

	// �������߂�
	float light = diffuse + specular;

	color.xyz = color.xyz * light;

	return color;
}