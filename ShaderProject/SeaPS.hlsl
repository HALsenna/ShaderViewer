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
	//pin.uv.x += fTexPos.x;
	// �@���}�b�v�������̖@�������o��
	float3 N = normalMap.Sample(samp, pin.uv);
	N = N * 2.0f - 1.0f;		// 0�`1�̐F�f�[�^���A-1�`1�̃x�N�g���f�[�^�ɕϊ�
	N = normalize(N);

	float3 L = normalize(-pin.tangentL);

	float diffuse = dot(N, L);
	float ambient = 0.2f;

	float3 R = reflect(-L, N);
	float3 V = normalize(camPos.xyz - pin.wPos.xyz);

	// ���ʔ���(�X�y�L�����[:specular)
	// ���˃x�N�g���Ǝ����x�N�g���̈�v��Ŕ��˂̋�����\��
	float specular = max(0, dot(R, V));			// ���ς̌v�Z���ʂ��}�C�i�X�ɂȂ���

	// ���̂܂܂��ƑS�̂����邢�̂ŗݏ悷��Ƃ��������ɂȂ�
	specular = pow(specular, 30.0f);

	//color.rgb = color.rgb * diffuse + specular + ambient;
	color.rgb = color.rgb * diffuse;

	return color;
}
